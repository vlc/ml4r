%module ml4r
%include LinearRegression.i
%{


  class Jamie
  {
  public:
      Jamie()  {}	
      ~Jamie() {}
      int foo() { return 42; }
  };
%}


class Jamie
{
public:
    Jamie()  {}	
    ~Jamie() {}
    int foo() { return 42; }
};
