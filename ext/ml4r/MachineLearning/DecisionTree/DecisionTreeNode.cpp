#include "MachineLearning/DecisionTree/DecisionTreeNode.h"
#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include "MachineLearning/DecisionTree/DecisionTreeExperiment.h"
#include "utils/Utils.h"

#include <stdexcept>
using std::runtime_error;

bool   DecisionTreeNode::m_missingValueDefined  = false;
double DecisionTreeNode::m_missingValue         = -1.0;

DecisionTreeNode::DecisionTreeNode( vector<shared_ptr<DecisionTreeExperiment> > experiments, 
                                    double sumZ,
                                    double sumW,
                                    Partition partition,
                                    shared_ptr<SplitDefinition> parentSplitDefinition)
    : m_experiments(experiments), m_nodeHasChildren(false), m_sumZ(sumZ), m_sumW(sumW),
      m_whichPartitionAmI(partition), m_parentSplitDefinition(parentSplitDefinition)
{
    
}

DecisionTreeNode::~DecisionTreeNode()
{

}

shared_ptr<DecisionTreeNode> DecisionTreeNode::getTerminalNodeForExperiment(shared_ptr<DecisionTreeExperiment> experiment)
{
    if (!m_nodeHasChildren)
        throw std::runtime_error("Node is a terminal node, so you shouldn't ask it for a terminal node!");

    if (m_splitDefinition.get() == 0)
        throw std::runtime_error("Node has children, but split definition is empty");

    shared_ptr<DecisionTreeNode> childForExperiment = chooseChild(experiment);

    if (childForExperiment.get() == 0)
        return childForExperiment;
    else if (childForExperiment->getSumW() == 0)
    {
        // this likely means that the value is missing, but there weren't any missing values in the
        // bagged training set.  Therefore, there is no weight in the missing child.
        // return an empty pointer, and this DecisionTreeNode will become the one chosen.
        return shared_ptr<DecisionTreeNode>();
    }
    else if (childForExperiment->isTerminalNode())
        return childForExperiment;
    else
    {
        shared_ptr<DecisionTreeNode> terminalNode = childForExperiment->getTerminalNodeForExperiment(experiment);
        if (terminalNode.get() == 0)
        {
            // we have encountered a NEW category...therefore we couldn't split on childForExperiment.  
            // therefore, return the child itself.
            return childForExperiment;
        }
        else
            return terminalNode;
    }

}

shared_ptr<DecisionTreeNode> DecisionTreeNode::chooseChild(shared_ptr<DecisionTreeExperiment> experiment)
{
    if (!m_nodeHasChildren)
        throw std::runtime_error("[DecisionTreeNode::chooseChild] - this Decision Tree has no children!");

    double featureValue = experiment->getFeatureValue(m_splitDefinition->getFeatureIndex());

    if (m_missingValueDefined && m_missingValue == featureValue)
        return m_missingChild;

    if (m_splitDefinition->isCategorical()) // categorical variable
    {
        if (Utils::hasElement(m_splitDefinition->getLhsCategories(), featureValue))
            return m_lhsChild;
        else if (Utils::hasElement(m_splitDefinition->getRhsCategories(), featureValue))
            return m_rhsChild;
        else
        {
            // it's not missing, but not in left or right.  Therefore, we have a NEW category.  
            // We should return an empty pointer, and let the parent handle it.
            return shared_ptr<DecisionTreeNode>();
        }
    }
    else // continuous variable 
    {
        double splitValue = m_splitDefinition->getSplitValue();
        if (m_missingValueDefined && m_missingValue == splitValue)
        {
            // complicated logic.  Our split value equals the missing value.  Therefore, we split off missing versus 
            // everything else (which gets put in the rhsChild).  As our feature value is not the missing value, we choose
            // the rhsChild.
            return m_rhsChild;
        }
        else if (featureValue < splitValue)
            return m_lhsChild;
        else
            return m_rhsChild;
    }
}

void DecisionTreeNode::defineSplit( shared_ptr<SplitDefinition> splitDefinition,
                                    shared_ptr<DecisionTreeNode> lhsChild, 
                                    shared_ptr<DecisionTreeNode> rhsChild, 
                                    shared_ptr<DecisionTreeNode> missingChild)
{
    setChildren(lhsChild, rhsChild, missingChild);
    m_splitDefinition = splitDefinition;
}

void DecisionTreeNode::setChildren( shared_ptr<DecisionTreeNode> lhsChild, 
                                    shared_ptr<DecisionTreeNode> rhsChild,
                                    shared_ptr<DecisionTreeNode> missingChild)
{
    m_nodeHasChildren   = true;
    m_lhsChild          = lhsChild;
    m_rhsChild          = rhsChild;
    m_missingChild      = missingChild;
}

vector<shared_ptr<DecisionTreeExperiment> > DecisionTreeNode::getExperiments()
{
    return m_experiments;
}

bool DecisionTreeNode::isTerminalNode()
{
    return !m_nodeHasChildren;
}

void DecisionTreeNode::clearExperimentsWithinTree()
{
    m_experiments.clear();
    if (m_nodeHasChildren)
    {
        m_lhsChild->clearExperimentsWithinTree();
        m_rhsChild->clearExperimentsWithinTree();
        m_missingChild->clearExperimentsWithinTree();
    }
}

double DecisionTreeNode::getSumZ()
{
    return m_sumZ;
}

double DecisionTreeNode::getSumW()
{
    return m_sumW;
}

void DecisionTreeNode::setMissingValue( double missingValue )
{
    m_missingValue = missingValue;
    m_missingValueDefined = true;
}

shared_ptr<SplitDefinition> DecisionTreeNode::getSplitDefinition()
{
    return m_splitDefinition;
}

shared_ptr<SplitDefinition> DecisionTreeNode::getParentSplitDefinition()
{
    return m_parentSplitDefinition;
}

Partition DecisionTreeNode::getPartition()
{
    return m_whichPartitionAmI;
}

void DecisionTreeNode::setSumZ( double sumZ )
{
    m_sumZ = sumZ;
}

void DecisionTreeNode::setSumW( double sumW )
{
    m_sumW = sumW;
}

void DecisionTreeNode::updateSums()
{
    m_sumW = 0.0;
    m_sumZ = 0.0;
    for (unsigned int i=0; i<m_experiments.size(); ++i)
    {
        double w = m_experiments.at(i)->getWeight();
        m_sumW += w;
        m_sumZ += w * m_experiments.at(i)->getZ();
    }
}
