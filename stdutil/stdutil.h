#ifndef STDUTIL_H
#define STDUTIL_H

//#pragma warning(disable:4786)

#ifdef _MSC_VER
  #pragma warning(disable:4786)  // MSVC-specific
#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wunknown-pragmas"  // Ignore unknown pragmas in GCC
#endif

#include <string>
#include <iostream>


#define d_new new

std::string tolower( const std:: string &s);

std::string itoa( int n );

//lazy version of auto_ptr
template<class T>
class a_ptr{
public:
	a_ptr(T *t=0):t(t){}
	~a_ptr(){delete t;}
	a_ptr &operator=(T *t){this->t=t;return *this;}
	T &operator*()const{return *t;}
	T *operator->()const{return t;}
	operator T&()const{return *t;}
	operator T*()const{return t;}
	T *release(){ T *tt=t;t=0;return tt; }
private:
	T *t;
};



#endif
