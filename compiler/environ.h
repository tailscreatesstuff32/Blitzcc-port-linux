
#ifndef ENVIRON_H
#define ENVIRON_H



#include "type.h"
#include "decl.h"
//#include "label.h"
class Environ {
public:
	//int level;
	//DeclSeq *decls;
	//DeclSeq *funcDecls;
	//DeclSeq *typeDecls;

	//vector<Type*> types;

	//vector<Label*> labels;
	//Environ *globals;
	//Type *returnType;
	string funcLabel,breakLabel;
	//list<Environ*> children;		//for delete!

	Environ( const string &f,Type *r,int l,Environ *gs );
	~Environ();
string setBreak( const string &s );
	//Label *findLabel( const string &s );
	//Label *insertLabel( const string &s,int def,int src,int sz );

};
#endif
