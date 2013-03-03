#ifndef OLSLinearRegression_h__
#define OLSLinearRegression_h__

#include "LinearRegression.h"

class OLSLinearRegression : public LinearRegression
{
public:

	OLSLinearRegression(std::vector<std::vector<double> > xs, std::vector<double> ys,
	                    std::vector<double> weights = std::vector<double>());
    OLSLinearRegression(std::vector<double> xs, std::vector<double> ys,
    	                std::vector<double> weights = std::vector<double>());
    OLSLinearRegression(std::vector<std::vector<double> > xs, std::vector<double> ys, double fixedConstant,
                        std::vector<double> weights = std::vector<double>());
	~OLSLinearRegression();

    void calculate();

    void EstimateBs();

protected:
	
private:
};



#endif // OLSLinearRegression_h__