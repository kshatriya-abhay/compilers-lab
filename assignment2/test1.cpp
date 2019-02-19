
// C++ program to demonstrate constructors 
  
#include <bits/stdc++.h> 
using namespace std; 
class Geeks 
{ 
    public: 
    int id; 
      
    //Default Constructor 
    Geeks() 
    { 
        cout << "Default Constructor called" << endl;  
        id=-1; 
    } 
      
    //Parametrized Constructor 
    Geeks(int x) 
    { 
        cout << "Parametrized Constructor called" << endl; 
        id=x; 
    } 
}; 

//Base class 
class Parent 
{ 
    public: 
      int id_p; 
}; 
   
// Sub class inheriting from Base Class(Parent) 
class Child : public Parent 
{ 
    public: 
      int id_c; 
}; 

class Complex { 
private: 
    int real, imag; 
public: 
    Complex(int r = 0, int i =0)  {real = r;   imag = i;} 
      
    // This is automatically called when '+' is used with 
    // between two Complex objects 
    Complex operator + (Complex const &obj) { 
         Complex res; 
         res.real = real + obj.real; 
         res.imag = imag + obj.imag; 
         return res; 
    } 
    void print() { cout << real << " + i" << imag << endl; } 
}; 


int main() { 
      
    // obj1 will call Default Constructor 
    Geeks obj1; 
    cout << "Geek id is: " <<obj1.id << endl; 
      
    // obj1 will call Parametrized Constructor 
    Geeks obj2(21); 
    cout << "Geek id is: " <<obj2.id << endl; 
    return 0; 
} 

