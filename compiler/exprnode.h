#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "node.h"

struct ExprNode : public Node{

};

struct ExprSeqNode : public Node{



	vector<ExprNode*> exprs;
	~ExprSeqNode(){ for( ;exprs.size();exprs.pop_back() ) delete exprs.back(); }
	void push_back( ExprNode *e ){ exprs.push_back( e ); }





};
#include "varnode.h"

struct NullNode : public ExprNode{
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
};


struct CallNode : public ExprNode{
	string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	CallNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~CallNode(){ delete exprs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
};

struct VarExprNode : public ExprNode{
	VarNode *var;
	VarExprNode( VarNode *v ):var(v){}
	~VarExprNode(){ delete var; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
};

#endif
