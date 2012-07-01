#ifndef __Utils_h__
#define __Utils_h__

#include <vector>
#include <stdexcept>

namespace Utils
{
    template<typename T>
    T vectorSum(const std::vector<T>& vec)
    {
        if (vec.size() == 0)
            return 0.0;

        T sum = vec.front();
        for (unsigned int i=1; i < vec.size(); ++i)
            sum += vec.at(i);

        return sum;
    }

    template<class T>
    vector<T> vectorAbs(const vector<T>& vec)
    {
        vector<T> absVec;
        absVec.reserve(vec.size());

        for (unsigned int i=0; i < vec.size(); ++i)
        {
            absVec.push_back((T)abs(vec.at(i))); 
        }
        return absVec; 
    }

    template<class T> vector<T>& operator+=(vector<T>& vec, const vector<T>& vec2)
    {
        if (vec.size() != vec2.size())
            throw std::runtime_error("Can't add vectors of different sizes");
        for (unsigned int i=0; i < vec.size(); ++i)
            vec.at(i) += vec2.at(i);
        return vec;
    }

    template <class T> 
    vector<T>& operator+=(vector<T>& vec, const float offset)
    {
        for (unsigned int i=0; i < vec.size(); ++i)
            vec.at(i) += offset;
        return vec;
    }
};

#endif 