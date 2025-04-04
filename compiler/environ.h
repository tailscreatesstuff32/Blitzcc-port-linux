
#ifndef ENVIRON_H
#define ENVIRON_H



#include "type.h"
#include "decl.h"

class Environ {
public:


	Environ( const string &f,Type *r,int l,Environ *gs );
	~Environ();

	//Label *findLabel( const string &s );
	//Label *insertLabel( const string &s,int def,int src,int sz );
	string funcLabel,breakLabel;
};
#endif
