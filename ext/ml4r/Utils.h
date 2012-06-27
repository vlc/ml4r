#ifndef __Utils_h__
#define __Utils_h__

#include <vector>

class Utils
{
public:
	template<typename T>
	static T vectorSum(std::vector<T>& vec)
	{
		if (vec.size() == 0)
			return 0.0;

		T sum = vec.front();
		for (unsigned int i=1; i < vec.size(); ++i)
			sum += vec.at(i);

		return sum;
	}

private: 
    Utils()  {}
    ~Utils() {}	
};

#endif 