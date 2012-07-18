#ifndef __CATEGORY_INFO_H__
#define __CATEGORY_INFO_H__

struct CategoryInfo
{
    CategoryInfo()
     : sumZ(0), sumW(0), countN(0), meanZ(0), category(-1)
    {

    }

    vector<shared_ptr<DecisionTreeExperiment> > experiments;
    double sumW;  
    double sumZ;  
    int    countN;
    double meanZ; 

    double    category;
    void addExperiment(shared_ptr<DecisionTreeExperiment> experiment)
    {
        experiments.push_back(experiment);
        sumW += experiment->getWeight();
        sumZ += experiment->getWeight() * experiment->getZ();
        countN += 1;
        meanZ = sumZ / sumW;
    }

    // define operator to sort on meanZ
    bool operator<(const CategoryInfo& rhs) const { return this->meanZ < rhs.meanZ; }
};

#endif // __CATEGORY_INFO_H__