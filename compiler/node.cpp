#include "std.h"
#include "nodes.h"

set<string> Node::usedfuncs;



///////////////////////////////
// generic exception thrower //
///////////////////////////////
void Node::ex(){
	ex( "INTERNAL COMPILER ERROR" );
}

void Node::ex( const string &e ){
	throw Ex( e,-1,"" );
}

void Node::ex( const string &e,int pos ){
	throw Ex( e,pos,"" );
}

void Node::ex( const string &e,int pos,const string &f ){
	throw Ex( e,pos,f );
}


///////////////////////////////
// Generate a fresh ASM label //
////////////////////////////////
string Node::genLabel(){
	static int cnt;
	return "_"+itoa( ++cnt & 0x7fffffff );
}
