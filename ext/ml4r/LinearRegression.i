%module ml4r
%{
  #include "LinearRegression.h"
  #include "OLSLinearRegression.h"
%}

%include "std_vector.i"
%include "std_pair.i"
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
   %template(VectorVectorDouble) std::vector<std::vector<double> >;
   %template(PairVectorDoubleDouble) std::pair<std::vector<double>,double>;
}

%include "LinearRegression.h"
%include "OLSLinearRegression.h"
