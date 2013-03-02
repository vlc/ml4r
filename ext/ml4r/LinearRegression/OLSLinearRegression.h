#ifndef OLSLinearRegression_h__
#define OLSLinearRegression_h__

#include "LinearRegression.h"

class OLSLinearRegression : public LinearRegression
{
public:
	OLSLinearRegression(std::vector<std::vector<double> > xs, std::vector<double> ys);
	~OLSLinearRegression();

    void calculate();

    void EstimateBs();

protected:
	
private:
};



#endif // OLSLinearRegression_h__