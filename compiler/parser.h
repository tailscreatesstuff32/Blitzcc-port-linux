



#ifndef PARSER_H
#define PARSER_H


#include "toker.h"
#include "nodes.h"


class Parser{
public:

    Parser( Toker &t );
    ProgNode *parse( const string &main );

private:
	string incfile;
	Toker *toker,*main_toker;
	map<string,DimNode*> arrayDecls;
	

VarNode *parseVar();

VarNode *parseVar( const string &ident,const string &tag );


        string parseIdent();
	void parseChar( int c );
	string parseTypeTag();
        ExprSeqNode *parseExprSeq();
	StmtSeqNode *parseStmtSeq( int scope );
	void parseStmtSeq( StmtSeqNode *stmts,int scope );
	void ex( const string &s );
	void exp( const string &s );
	IfNode *parseIf();

	DeclSeqNode *consts;
	DeclSeqNode *structs;
	DeclSeqNode *funcs;
	DeclSeqNode *datas;

	ExprNode *parseExpr( bool opt );
	ExprNode *parseExpr1( bool opt );	//And, Or, Eor
	ExprNode *parseExpr2( bool opt );	//<,=,>,<=,<>,>=
	ExprNode *parseExpr3( bool opt );	//+,-
	ExprNode *parseExpr4( bool opt );	//Lsr,Lsr,Asr
	ExprNode *parseExpr5( bool opt );	//*,/,Mod
	ExprNode *parseExpr6( bool opt );	//^
	ExprNode *parseUniExpr( bool opt );	//+,-,Not,~
	ExprNode *parsePrimary( bool opt );


	
};

#endif
