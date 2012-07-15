%module ml4r
%{
  #include "LinearRegression/LinearRegression.h"
  #include "LinearRegression/OLSLinearRegression.h"
%}

%include "std_vector.i"
%include "std_pair.i"
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
   %template(VectorVectorDouble) std::vector<std::vector<double> >;
   %template(PairVectorDoubleDouble) std::pair<std::vector<double>,double>;
}

%include "LinearRegression/LinearRegression.h"
%include "LinearRegression/OLSLinearRegression.h"
