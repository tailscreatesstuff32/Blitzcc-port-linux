
#ifndef PROGNODE_H
#define PROGNODE_H





#include "node.h"
#include "codegen.h"


struct UserFunc{
	string ident,proc,lib;
	UserFunc( const UserFunc &t ):ident(t.ident),proc(t.proc),lib(t.lib){}
	UserFunc( const string &id,const string &pr,const string &lb ):ident(id),proc(pr),lib(lb){}
};


struct ProgNode : Node{



	DeclSeqNode *consts;
	DeclSeqNode *structs;
	DeclSeqNode *funcs;
	DeclSeqNode *datas;
	StmtSeqNode *stmts;
	

Environ *semant( Environ *e );



	ProgNode( DeclSeqNode *c,DeclSeqNode *s,DeclSeqNode *f,DeclSeqNode *d,StmtSeqNode *ss ):consts(c),structs(s),funcs(f),datas(d),stmts(ss){}
	~ProgNode(){ 
		if (stmts) delete stmts;
		if (datas) delete datas;
		if (funcs) delete funcs;
		if (structs) delete structs;
		if (consts) delete consts;
	}





	
};

#endif
