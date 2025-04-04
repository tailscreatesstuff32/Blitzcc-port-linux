#ifndef NODE_H
#define NODE_H

#include "ex.h"
#include "toker.h"
#include "environ.h"
#include "codegen.h"
struct Node{

	//used user funcs...
	static set<string> usedfuncs;
	
	virtual ~Node(){}
	//helper funcs
	static void ex();
	static void ex( const string &e );
	static void ex( const string &e,int pos );
	static void ex( const string &e,int pos,const string &f );
	static string genLabel();
};

	
#endif
