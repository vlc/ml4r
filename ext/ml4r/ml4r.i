%module ml4r
%include LinearRegression.i
%inline %{

  class Jamie
  {
  public:
      Jamie()  {}	
      ~Jamie() {}
      int foo() { return 42; }
  };
%}


//class Jamie
//{
//public:
//    Jamie()  {}	
//     ~Jamie() {}
//     int foo() { return 42; }
// };
