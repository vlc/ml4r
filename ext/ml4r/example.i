%module ml4r
%{
extern double Foo;
int fact(int n);
#include "LinearRegression.h"

class Jamie
{
public:
    Jamie()  {}	
    ~Jamie() {}
    int foo() { return 42; }
};

%}

extern double Foo;
int fact(int n);
#include "LinearRegression.h"
class Jamie
{
public:
    Jamie()  {}	
    ~Jamie() {}
    int foo() { return 42; }
};
