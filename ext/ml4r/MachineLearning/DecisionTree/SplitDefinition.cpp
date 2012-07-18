#include "MachineLearning/DecisionTree/SplitDefinition.h"
#include <stdexcept>
using std::runtime_error;

SplitDefinition::SplitDefinition(   shared_ptr<DecisionTreeNode> nodeToSplit, 
                                    int featureIndex, 
                                    set<double>& lhsCategories,
                                    set<double>& rhsCategories,
                                    double    lhsSumZ,
                                    double    lhsSumW,
                                    int       lhsCount,
                                    double    rhsSumZ,
                                    double    rhsSumW,
                                    int       rhsCount,
                                    double    missingSumZ,
                                    double    missingSumW,
                                    int       missingCount,
                                    double improvement)
: m_nodeToSplit(nodeToSplit), m_splitFeatureIndex(featureIndex), m_lhsCategories(lhsCategories), m_rhsCategories(rhsCategories),
m_lhsSumZ(lhsSumZ), m_lhsSumW(lhsSumW), m_lhsCount(lhsCount), 
m_rhsSumZ(rhsSumZ), m_rhsSumW(rhsSumW), m_rhsCount(rhsCount), 
m_missingSumZ(missingSumZ), m_missingSumW(missingSumW), m_missingCount(missingCount), 
m_improvement(improvement)
{
    m_splitValue = -1;
    m_featureIsCategorical = true;
}

SplitDefinition::SplitDefinition(   shared_ptr<DecisionTreeNode> nodeToSplit, 
                                    int featureIndex, 
                                    double splitValue, 
                                    double    lhsSumZ,
                                    double    lhsSumW,
                                    int       lhsCount,
                                    double    rhsSumZ,
                                    double    rhsSumW,
                                    int       rhsCount,
                                    double    missingSumZ,
                                    double    missingSumW,
                                    int       missingCount,
                                    double improvement)
: m_nodeToSplit(nodeToSplit), m_splitFeatureIndex(featureIndex), m_splitValue(splitValue), 
m_lhsSumZ(lhsSumZ), m_lhsSumW(lhsSumW), m_lhsCount(lhsCount), 
m_rhsSumZ(rhsSumZ), m_rhsSumW(rhsSumW), m_rhsCount(rhsCount), 
m_missingSumZ(missingSumZ), m_missingSumW(missingSumW), m_missingCount(missingCount), 
m_improvement(improvement)
{
    m_featureIsCategorical = false;
}

SplitDefinition::~SplitDefinition()
{

}

double SplitDefinition::getImprovement()
{
    return m_improvement;
}

shared_ptr<DecisionTreeNode> SplitDefinition::getNodeToSplit()
{
    return m_nodeToSplit;
}

int SplitDefinition::getFeatureIndex()
{
    return m_splitFeatureIndex;
}

set<double>& SplitDefinition::getLhsCategories()
{
    if (m_lhsCategories.empty() && m_missingCount == 0)
        throw std::runtime_error("LhsCategories are empty!");

    return m_lhsCategories;
}

set<double>& SplitDefinition::getRhsCategories()
{
    if (m_rhsCategories.empty() && m_missingCount == 0)
        throw std::runtime_error("RhsCategories are empty!");

    return m_rhsCategories;
}

double SplitDefinition::getSplitValue()
{
    return m_splitValue;
}

int SplitDefinition::getLhsExperimentCount()
{
    return m_lhsCount;
}

bool SplitDefinition::isCategorical()
{
    return m_featureIsCategorical;
}

int SplitDefinition::getRhsExperimentCount()
{
    return m_rhsCount;
}

int SplitDefinition::getMissingExperimentCount()
{
    return m_missingCount;
}

double SplitDefinition::getLhsSumZ()
{
    return m_lhsSumZ;
}

double SplitDefinition::getLhsSumW()
{
    return m_lhsSumW;
}

double SplitDefinition::getRhsSumZ()
{
    return m_rhsSumZ;
}

double SplitDefinition::getRhsSumW()
{
    return m_rhsSumW;
}

double SplitDefinition::getMissingSumZ()
{
    return m_missingSumZ;
}

double SplitDefinition::getMissingSumW()
{
    return m_missingSumW;
}


