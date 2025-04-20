#ifndef VARNODE_H
#define VARNODE_H
#include "varnode.h"
struct VarNode : public Node{
	//Type *sem_type;

	//get set var
	//TNode *load( Codegen *g );
	///virtual TNode *store( Codegen *g,TNode *n );
	//virtual bool isObjParam();

	//addr of var
	virtual void semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;
};

#endif