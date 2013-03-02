%include "std_vector.i"
%include "std_pair.i"
namespace std {
   %template(IntVector)              vector<int>;
   %template(DoubleVector)           vector<double>;
   %template(FloatVector)            vector<float>;
   %template(VectorVectorDouble)     std::vector<std::vector<double> >;
   %template(PairVectorDoubleDouble) std::pair<std::vector<double>,double>;
}

%module ml4r
%include LinearRegression/LinearRegression.i
%include utils/utils.i