#ifndef OLSLinearRegression_h__
#define OLSLinearRegression_h__

#include "LinearRegression.h"

class OLSLinearRegression : public LinearRegression
{
public:
	  OLSLinearRegression();
	  ~OLSLinearRegression();

    void Execute();

    void EstimateBs();

protected:
	
private:
};



#endif // OLSLinearRegression_h__