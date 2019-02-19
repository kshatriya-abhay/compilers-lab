
#include <bits/stdc++.h> // This is a header file
using namespace std;

class Point;

class Geeks 
{ 
    public: 
    string geekname;   
    void printname(){   cout << "Geekname is: " << geekname;    } 
}; 

class A 
{ 
public: 
  A()  { cout << "A's constructor called " << endl; } 
}; 
  
class B 
{ 
public: 
  B()  { cout << "B's constructor called " << endl; } 
}; 
  
class C: public B, public A  // Note the order 
{ 
public: 
  C()  { cout << "C's constructor called " << endl; } 
}; 

    
class Enclosing {       
   private:    
       int x; 
     
   /* start of Nested class declaration */  
   class Nested { 
      int y;    
      void NestedFun(Enclosing *e) { 
        // cout<<e.x;  // works fine: nested class can access  
                     // private members of Enclosing class 
      }        
   }; // declaration Nested class ends here 
}; // declaration Enclosing class ends here 

////
void fun()
{ 
      class Test  // local to fun 
      { 
        /* members of Test class */
      }; 
}      

class Rectangle {
    int width, height;
  public:
    Rectangle() {}
    void set_values (int,int);
    Rectangle (int x, int y) : width(x), height(y) {}
    int area() {return width*height;}
    friend Rectangle duplicate (const Rectangle&);
} rect;

Rectangle duplicate (const Rectangle& param)
{
  Rectangle res;
  res.width = param.width*2;
  res.height = param.height*2;
  return res;
}

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}

class Point {
private:
  int x, y;
public:
  Point() : x(0), y(0) { }
  Point& operator()(int dx, int dy) {
    x += dx;
    y += dy;
    return *this;
  }
};

class complx
{
      double real,
             imag;
public:
      complx( double real = 0., double imag = 0.); // constructor
      complx operator+(const complx&) const;       // operator+()
};

// define constructor
complx::complx( double r, double i )
{
      real = r; imag = i;
}

// define overloaded + (plus) operator
complx complx::operator+ (const complx& c) const
{
      complx result;
      result.real = (this->real + c.real);
      result.imag = (this->imag + c.imag);
      return result;
}

class Distance {
   private:
      int feet;             // 0 to infinite
      int inches;           // 0 to 12
      
   public:
      // required constructors
      Distance() {
         feet = 0;
         inches = 0;
      }
      Distance(int f, int i) {
         feet = f;
         inches = i;
      }
      
      // overload function call
      Distance operator()(int a, int b, int c) {
         Distance D;
         
         // just put random calculation
         D.feet = a + c + 10;
         D.inches = b + c + 100 ;
         return D;
      }
      
      // method to display distance
      void displayDistance() {
         cout << " F: " << feet << " I: " << inches <<endl;
      }   
};

class Mother {
  public:
    Mother ()
      { cout << " Mother: no parameters\n " ; }
    Mother (int a)
      { cout << " Mother: int parameter\n " ; }
};

class Daughter : public Mother {
  public:
    Daughter (int a)
      { cout << " Daughter: int parameter\n\n " ; }
};

class Son : public Mother {
  public:
    Son (int a) : Mother (a)
      { cout << " Son: int parameter\n\n " ; }             //error
};

//deconstructor
class String 
{ 
private: 
    char *s; 
    int size; 
public: 
    String(char *); // constructor                          //error
    ~String();      // destructor 
}; 
  
String::String(char *c)                                    //error
{ 
    size = strlen(c); 
    s = new char[size+1]; 
    strcpy(s,c); 
} 
  
String::~String() 
{ 
    delete []s; 
}


//Overloading Operator in C++
class Time
{
    int hr, min, sec;
    public:
    // default constructor
    Time()
    {
        hr=0, min=0; sec=0;                                           //error
    }
    
    // overloaded constructor
    Time(int h, int m, int s)
    {
        hr=h, min=m; sec=s;                                          // error
    }
    
    //overloading '==' Operator                                       // error
    friend bool operator==(Time &t1, Time &t2);                       //error   2 error as operator defined twice line 227 236 and two object not detected 
};

/* 
    Defining the overloading operator function
    Here we are simply comparing the hour, minute and
    second values of two different Time objects to compare
    their values
*/
bool operator== (Time &t1, Time &t2)                                    //error
{
    return ( t1.hr == t2.hr && t1.min == t2.min && t1.sec == t2.sec );   //error
}

int main(){

  int Test;
  Geeks er;
  B tg;
  Distance tt;
  int a;
  printf( " The value of a is %d ",a);

  obj1.setData(10); 
  obj1.print(); 

  rect.set_values (3,4);
  cout << " area: " << rect.area();

  Rectangle rt;
  Rectangle rt1 (2,3);                                                       //error
  rt = duplicate (rt1);
  cout << rt.area() << '\n';

  Rectangle *bar = new Rectangle();                                           //error
  Rectangle *foo = new Rectangle;

  Daughter kelly(0);                                                           //error
  Son bud(0);                                                                  //error

  Time t1(3,15,45);                                                             //error
    Time t2(4,15,45);                                                           //error
    if(t1 == t2)
    {
        cout << " Both the time values are equal " ;
    }   
    else 
    {
        cout << " Both the time values are not equal " ;
    }

    return 0;
}
