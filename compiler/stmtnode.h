#ifndef STMTNODE_H
#define STMTNODE_H

#include "node.h"


struct StmtNode : public Node{

		int pos;	//offset in source stream
	StmtNode():pos(-1){}

	virtual void semant( Environ *e ){}
	//virtual void translate( Codegen *g ){}

};


struct StmtSeqNode : public Node{

	string file;
	vector<StmtNode*> stmts;
	StmtSeqNode( const string &f ):file(f){}
	void push_back( StmtNode *s ){ stmts.push_back( s ); }
	void semant( Environ *e );
	void translate( Codegen *g );
};



struct DimNode : public StmtNode{
	string ident,tag;
 	ExprSeqNode *exprs;
//	ArrayType *sem_type;
//	Decl *sem_decl;
	DimNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~DimNode(){ delete exprs; }
	void semant( Environ *e );
	void translate( Codegen *g );
};

struct ExitNode : public StmtNode{
	string sem_brk;
	void semant( Environ *e );
	void translate( Codegen *g );
};

struct IfNode : public StmtNode{
	ExprNode *expr;
	StmtSeqNode *stmts,*elseOpt;
	IfNode( ExprNode *e,StmtSeqNode *s,StmtSeqNode *o ):expr(e),stmts(s),elseOpt(o){}
	~IfNode(){ delete expr;delete stmts;delete elseOpt; }

	void semant( );
	void translate( Codegen *g );

};


struct LabelNode : public StmtNode{
	string ident;
	int data_sz;
	LabelNode( const string &s,int sz ):ident(s),data_sz(sz){}
	void semant( Environ *e );
	void translate( Codegen *g );
};

struct GotoNode : public StmtNode{
	string ident;
	GotoNode( const string &s ):ident(s){}
	void semant( Environ *e );
	void translate( Codegen *g );
};

struct GosubNode : public StmtNode{
	string ident;
	GosubNode( const string &s ):ident(s){}
	void semant( Environ *e );
	void translate( Codegen *g );
};

struct WhileNode : public StmtNode{
	int wendPos;
	ExprNode *expr;
	StmtSeqNode *stmts;
	string sem_brk;
	WhileNode( ExprNode *e,StmtSeqNode *s,int wp ):expr(e),stmts(s),wendPos(wp){}
	~WhileNode(){ delete expr;delete stmts; }
	void semant( Environ *e );
	void translate( Codegen *g );
};



#endif
