#include "std.h"
#include "environ.h"

Environ::Environ( const string &f,Type *r,int l,Environ *gs ) {
}

Environ::~Environ(){
}


string Environ::setBreak( const string &s ){
	string t=breakLabel;
	breakLabel=s;return t;
}