#ifndef __Utils_h__
#define __Utils_h__

#include <vector>
#include <stdexcept>
#include <map>
#include <boost/foreach.hpp>
using std::map;
using std::vector;

namespace Utils
{
    template<class Container, class T>
    bool hasElement(const Container& m, T element)
    {
        return find(m.begin(), m.end(), element) != m.end();
    }

    template<class T, class U>
    bool hasElement(const std::map<T,U>& m, T element)
    {
        return m.find(element) != m.end();
    }

    template<typename T> 
    int vectorIndex(vector<T>& c, T element)
    {
        if (find(c.begin(), c.end(), element) == c.end())
            return -1;
        return (int) (find(c.begin(), c.end(), element) - c.begin()); 
    }

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

    template<class T> vector<T>
    vectorRange(T start, T end, T stepSize = 1)
    {
        int numSteps = (end - start) / stepSize + 1;
        vector<T> returnValue;
        returnValue.resize(numSteps);

        for (int i = 0; i < numSteps; ++i)
            returnValue.at(i) = start + i * stepSize; 

        return returnValue;
    }

    template<class T> vector<T>
    vectorRepeat(vector<T>& vec, int totalDesiredLength)
    {
        vector<T> returnValue;
        returnValue.resize(totalDesiredLength);
        int vectorSize = vec.size();

        for (int i = 0; i < totalDesiredLength; ++i)
        {
            int index = i % vectorSize;
            returnValue.at(i) = vec.at(index);
        }
        return returnValue;
    }

    template<class T> vector<T>
    vectorShuffle(vector<T>& vec)
    {

        vector<int> sortVector = vectorOfRandomInt(vec.size());
        vector<T> returnValue  = vectorSortUsingOtherVector(vec, sortVector);
        return returnValue;
    }

    vector<int> vectorOfRandomInt(int length);
    
    template<class T, class U>
    vector<T> vectorSortUsingOtherVector(vector<T>& vec, vector<U>& otherVector)
    {
        if (otherVector.size() != vec.size())
            throw std::runtime_error("[] - vec and otherVector must be of equal size.");

        vector<U> otherVectorSorted = otherVector;
        sort(otherVectorSorted.begin(), otherVectorSorted.end());
        map<U, int> newPosition;
        int index = -1;
        BOOST_FOREACH(U& e, otherVectorSorted)
        {
            ++index;
            if (index == 0 || e != otherVectorSorted.at(index-1))
                newPosition[e] = index;
        }

        vector<T> returnValue;
        returnValue.resize(vec.size());

        index = -1;
        BOOST_FOREACH(T& e, vec)
        {
            ++index;
            U sortValue = otherVector.at(index);
            int newIndex = newPosition[sortValue]++; // ++ happens after newIndex is assigned
            returnValue[newIndex] = e;
        }
        return returnValue;
    }
    
    template<class T>
    std::vector<T> vectorAbs(const std::vector<T>& vec)
    {
        std::vector<T> absVec;
        absVec.reserve(vec.size());

        for (unsigned int i=0; i < vec.size(); ++i)
        {
            absVec.push_back((T)abs(vec.at(i))); 
        }
        return absVec; 
    }

    template<class T> std::vector<T>& operator+=(std::vector<T>& vec, const std::vector<T>& vec2)
    {
        if (vec.size() != vec2.size())
            throw std::runtime_error("Can't add vectors of different sizes");
        for (unsigned int i=0; i < vec.size(); ++i)
            vec.at(i) += vec2.at(i);
        return vec;
    }

    template <class T> 
    std::vector<T>& operator+=(std::vector<T>& vec, const float offset)
    {
        for (unsigned int i=0; i < vec.size(); ++i)
            vec.at(i) += offset;
        return vec;
    }

    template <class T> 
    std::vector<T>& operator*=(vector<T>& vec, const float factor)
    {
        BOOST_FOREACH(T& t, vec) t *= factor;
        return vec;
    }

    template<class T>
    T vectorMax(vector<T>& vec)
    {
        // setup the initial value to test against
        T currentMax = (vec.size() > 0) ? vec.at(0) : 0; 

        for (int i = 0; i < vec.size(); ++i)
            if (vec[i] > currentMax)
                currentMax = vec[i];
        return currentMax; 
    }

    template<class T>
    int vectorMaxIndex(vector<T>& vec)
    {
        if (vec.size() == 0) return -1;  //? vec.at(0) : 0;  
        T currentMax = vec[0];
        int currentMaxIndex = 0; 
        for (unsigned int index = 1; index < vec.size(); ++index)
        {
            if (vec[index] <= currentMax) continue; 
            currentMax      = vec[index]; 
            currentMaxIndex = index; 
        }
        return currentMaxIndex; 
    }
};

#endif 