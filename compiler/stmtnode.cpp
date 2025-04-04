#include "std.h"
#include "nodes.h"


////////////////////////
// Statement Sequence //
////////////////////////
void StmtSeqNode::semant( Environ *e ){
	for( int k=0;k<stmts.size();++k ){
		try{ stmts[k]->semant( e ); }
		catch( Ex &x ){ 
			if( x.pos<0 ) x.pos=stmts[k]->pos;
			if( !x.file.size() ) x.file=file;
			throw; 
		}
	}
}

///////////
// Break //
///////////
void ExitNode::semant( Environ *e ){
	sem_brk=e->breakLabel;
	if( !sem_brk.size() ) ex( "break must appear inside a loop" );
}

void ExitNode::translate( Codegen *g ){
	//g->code( d_new TNode( IR_JUMP,0,0,sem_brk ) );
}

////////////////
// user label //
////////////////
void LabelNode::semant( Environ *e ){

}


void LabelNode::translate( Codegen *g ){


}


void WhileNode::semant(Environ *e){



}

void WhileNode::translate( Codegen *g ){

 }
