#ifndef DECLNODE_H
#define DECLNODE_H

struct DeclNode : public Node{
	int pos;
	string file;
	DeclNode():pos(-1){}
	virtual void proto( DeclSeq *d,Environ *e ){}
	virtual void semant( Environ *e ){}
	virtual void translate( Codegen *g ){}
	virtual void transdata( Codegen *g ){}
};

struct DeclSeqNode : public Node{
	vector<DeclNode*> decls;
	DeclSeqNode(){}
	~DeclSeqNode(){ for(;decls.size();decls.pop_back())delete decls.back(); }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );
	void transdata( Codegen *g );
	void push_back( DeclNode *d ){ decls.push_back( d ); }
	int  size(){ return decls.size(); }
};

#endif