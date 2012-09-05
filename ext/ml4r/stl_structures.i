%include "std_vector.i"
%include "std_pair.i"
%include "std_string.i"
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
   %template(VectorVectorDouble) std::vector<std::vector<double> >;
   %template(PairVectorDoubleDouble) std::pair<std::vector<double>,double>;
   %template(StringVector) vector<string>;
}