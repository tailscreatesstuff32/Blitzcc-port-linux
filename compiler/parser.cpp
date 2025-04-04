
#include "std.h"
#include <cstdlib>
#include "parser.h"

static bool isTerm( int c ){ 
	return c==':' || c=='\n'; 
}

#ifdef DEMO
static const int TEXTLIMIT=16384;
#else
static const int TEXTLIMIT=1024*1024-1;
#endif

#define HexIdentCurText(a, b, c) cout << "0x" << setw(b) << setfill('0') << uppercase << hex << a << nouppercase << dec  << " " << c  << "\n"



enum{
	STMTS_PROG,STMTS_BLOCK,STMTS_LINE
};

Parser::Parser( Toker &t ):toker(&t),main_toker(&t) {
};

//INCOMPLETE
ProgNode *Parser::parse( const string &main ){

	incfile=main;

	consts=d_new DeclSeqNode();
	structs=d_new DeclSeqNode();
	funcs=d_new DeclSeqNode();
	datas=d_new DeclSeqNode();
	StmtSeqNode *stmts=0;

	try{
		stmts=parseStmtSeq( STMTS_PROG );
		if( toker->curr()!=EOF ) exp( "end-of-file" );
	}catch( Ex ){


		if (stmts) delete stmts;
		if (datas) delete datas;
		if (funcs) delete funcs;
		if (structs) delete structs;
		if (consts) delete consts;
		throw;
	}

	return d_new ProgNode(consts, structs, funcs, datas, stmts);
}


string Parser::parseIdent(){
	if( toker->curr()!=IDENT ) exp( "identifier" );
	string t=toker->text();
	toker->next();
	return t;
}


VarNode *Parser::parseVar(){

 return NULL;
}

VarNode *Parser::parseVar( const string &ident,const string &tag ){

 return NULL;

}



void Parser::ex( const string &s ){
	throw Ex( s,toker->pos(),incfile );
}



void Parser::exp( const string &s ){
	switch( toker->curr() ){
	case NEXT:ex( "'Next' without 'For'" );
	case WEND:ex( "'Wend' without 'While'" );
	case ELSE:case ELSEIF:ex( "'Else' without 'If'" );
	case ENDIF:ex( "'Endif' without 'If'" );
	case ENDFUNCTION:ex( "'End Function' without 'Function'" );
	case UNTIL:ex( "'Until' without 'Repeat'" );
	case FOREVER:ex( "'Forever' without 'Repeat'" );
	case CASE:ex( "'Case' without 'Select'" );
	case ENDSELECT:ex( "'End Select' without 'Select'" );
	}
	ex( "Expecting "+s );
 }



StmtSeqNode *Parser::parseStmtSeq( int scope ){
	a_ptr<StmtSeqNode> stmts( d_new StmtSeqNode( incfile ) );
	parseStmtSeq( stmts,scope );
	return stmts.release();
}

//INCOMPLETE
void Parser::parseStmtSeq(StmtSeqNode *stmts, int scope) {
    for (;;) {
        // Skip colons and newlines
        while (toker->curr() == ':' || (scope != STMTS_LINE && toker->curr() == '\n')) {
            toker->next();
        }

        StmtNode *result = nullptr;
        int pos = toker->pos();

#ifdef DEMO
        if (Toker::chars_toked > TEXTLIMIT) {
            ex("Demo version source limit exceeded");
        }
#endif

        switch (toker->curr()) {
        case UNION: {
	cout << "Union\n";
           if( scope!=STMTS_PROG ) ex( "'Union' can only appear in main program" );
           toker->next();
			}

		case GOTO:
			{
				toker->next();string t=parseIdent();  //result=d_new GotoNode( t );
			}
			break;
		case GOSUB:
			{
				toker->next();string t=parseIdent(); //result=d_new GosubNode( t );
			}
			break;
		case RETURN:
			{
				toker->next();  //result=d_new ReturnNode( parseExpr( true ) );
			}
			break;
		case '.':
			{
				toker->next();string t=parseIdent();
				result=d_new LabelNode( t,datas->size() );
			}
			break;
			
            case EXIT: {
                string ident = toker->text();
                HexIdentCurText(toker->curr() ,4, ident);
                toker->next();
           
                result = d_new ExitNode();
            }

            break;

	  case WHILE:
		{
			toker->next();
			a_ptr<ExprNode> expr( parseExpr( false ) );
			a_ptr<StmtSeqNode> stmts( parseStmtSeq( STMTS_BLOCK ) );
			int pos=toker->pos();
			if( toker->curr()!=WEND ) exp( "'Wend'" );
			toker->next();
			result=d_new WhileNode( expr.release(),stmts.release(),pos );
	 	}
		
		break;



            case IDENT: {
				string ident=toker->text();
                HexIdentCurText(toker->curr() ,4, ident);
				toker->next();string tag=parseTypeTag();
				if( arrayDecls.find(ident)==arrayDecls.end() 
					&& toker->curr()!='=' && toker->curr()!='\\' && toker->curr()!='[' ){
					//must be a function
					ExprSeqNode *exprs;
					if( toker->curr()=='(' ){
						//ugly lookahead for optional '()' around statement params
						int nest=1,k;
						for( k=1;;++k ){
							int c=toker->lookAhead( k );
							if( isTerm( c ) ) ex( "Mismatched brackets" );
							else if( c=='(' ) ++nest;
							else if( c==')' && !--nest ) break;
						}
						if( isTerm( toker->lookAhead( ++k ) ) ){
							toker->next();
							exprs=parseExprSeq();
							if( toker->curr()!=')' ) exp( "')'" );
							toker->next();
						}else exprs=parseExprSeq();
					}else exprs=parseExprSeq();
				}
			}
			break;

        case IF:
            {
                string ident = toker->text();
                HexIdentCurText(toker->curr() ,4, ident);
                toker->next();HexIdentCurText(toker->curr() ,4, toker->text());result=parseIf();
                

                if( toker->curr()==ENDIF ) toker->next();
            }
            break;



            default:
                return;
        }

        // If a result was parsed, set its position and add it to the statement sequence
        if (result) {
            result->pos = pos;
            stmts->push_back(result);
        }
    }
}

IfNode *Parser::parseIf(){
    a_ptr<ExprNode> expr;
    a_ptr<StmtSeqNode> stmts,elseOpt;

    expr=parseExpr( false );
    if( toker->curr()==THEN ) {
    			HexIdentCurText(toker->curr() ,4, toker->text() );
        toker->next();
    }else{
      //  ex( "'If' without 'then'" );
    }


    bool blkif=isTerm( toker->curr() );
    stmts=parseStmtSeq( blkif ? STMTS_BLOCK : STMTS_LINE );

if( toker->curr()==ELSEIF ){

		HexIdentCurText(toker->curr() ,4, toker->text() );


		int pos=toker->pos();
		toker->next();
		HexIdentCurText(toker->curr() ,4, toker->text() );
		IfNode *ifnode=parseIf();
		ifnode->pos=pos;
		elseOpt=d_new StmtSeqNode( incfile );
		elseOpt->push_back( ifnode );
}else if( toker->curr()==ELSE ){
		HexIdentCurText(toker->curr() ,4, toker->text() );
		toker->next();
		elseOpt=parseStmtSeq( blkif ? STMTS_BLOCK : STMTS_LINE );

}

    if( blkif ){
        if( toker->curr()!=ENDIF ) exp( "'EndIf'" );

    }else if( toker->curr()!='\n' ) exp( "end-of-line" );

return d_new IfNode( expr.release(),stmts.release(),elseOpt.release() );
}

string Parser::parseTypeTag(){
	switch( toker->curr() ){
	case '%':toker->next();return "%";
	case '#':toker->next();return "#";
	case '$':toker->next();return "$";
	case '.':toker->next();return parseIdent();
	}
	return "";
}

ExprSeqNode *Parser::parseExprSeq() {
    a_ptr<ExprSeqNode> exprs(d_new ExprSeqNode());
    bool opt = true;
	//cout << "parseExprSeq\n";
     while (ExprNode *e = parseExpr(opt)) {
         exprs->push_back(e);

        if (toker->curr() != ',') {
           break;
         }

         cout << toker->curr() << "\n";
         toker->next();
         opt = false;
     }

    return exprs.release();
}

ExprNode *Parser::parseExpr(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr1(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr1(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr2(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr1" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr2(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr3(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr2" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr3(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr4(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr3" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr4(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr5(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr4" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr5(bool opt) {
    a_ptr<ExprNode> lhs(parseExpr6(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr5" << "\n";
    return lhs.release();
}

ExprNode *Parser::parseExpr6(bool opt) {
    a_ptr<ExprNode> lhs(parseUniExpr(opt));

    if (!lhs) {
        cout << "NULL" << "\n";
        return nullptr;
    }

    cout << "parseExpr6" << "\n";
    return lhs.release();
}



ExprNode *Parser::parseUniExpr( bool opt ){
    ExprNode *result = 0;
    string t;

    int c = toker->curr();
    switch( c ){
        case BBINT:
            cout << "BBINT\n";
            toker->next();
            break;
        case BBFLOAT:
            cout << "BBFLOAT\n";
            toker->next();
            break;
        case BBSTR:
            cout << "BBSTR\n";
            toker->next();
            break;
        case OBJECT:
            cout << "OBJECT\n";
            toker->next();
            break;
        case BBHANDLE:
            cout << "BBHANDLE\n";
            toker->next();
            break;
        case BEFORE:
            cout << "BEFORE\n";
            toker->next();
            break;
        case AFTER:
            cout << "AFTER\n";
            toker->next();
            break;
       // case '+': case '-': case '~': case ABS: case SGN:
        //    cout << "Unary Operator: " << (char)c << "\n";
           // break;
    	 case '+': case '-': case '~':
    		cout << "Unary Operator: " << (char)c << "\n";
    	        toker->next();	
                break;
		case ABS:
    		cout << "Unary Operator: ABS\n";
    		toker->next();
                break;
		case SGN:
   		toker->next();
                cout << "Unary Operator: SGN\n";
    		break;
        default:
           // cout << "Unhandled token: " << c << "\n";
            result = parsePrimary(opt);
            break;
    }

    return result;
}

ExprNode *Parser::parsePrimary( bool opt ){
	a_ptr<ExprNode> expr;
	string t,ident,tag;
	ExprNode *result=0;
	int n,k;
    
    switch( toker->curr() ){
        case '(':
         cout << "Left Parenthesis: (\n";
         toker->next();
	 expr=parseExpr( false );
         if( toker->curr()!=')' ) exp( "')'" );
	 toker->next();
	// result=expr.release();  
	break;
        case BBNEW:
            cout << "BBNEW\n";
            toker->next();
	    break;
        case FIRST:
            cout << "FIRST\n";
            toker->next();
	    break;
        case LAST:
            cout << "LAST\n";
            toker->next();
	    break;
        case BBNULL:
            cout << "BBNULL\n";
            toker->next();
	    break;
        case INTCONST:
            cout << "INTCONST\n";
            toker->next();
	    break;
        case FLOATCONST:
            cout << "FLOATCONST\n";
            toker->next();
	     break;
        case STRINGCONST:
            t=toker->text();
            cout << "STRINGCONST" <<  " " << t << "\n";
            toker->next();
            break;
        case BINCONST:
            cout << "BINCONST\n";
            toker->next();
            break;
        case HEXCONST:
            cout << "HEXCONST\n";
            toker->next();
            break;
        case PI:
            cout << "PI\n";
            toker->next();
            break;
        case BBTRUE:
             cout << "BBTRUE\n";
             toker->next();
             break;
        case BBFALSE:
	    cout << "BBFALSE\n";
            toker->next();
             break;
        case IDENT:
		ident=toker->text();
		toker->next();tag=parseTypeTag();
		if( toker->curr()=='(' && arrayDecls.find(ident)==arrayDecls.end() ){
			//must be a func
			toker->next();
			a_ptr<ExprSeqNode> exprs( parseExprSeq() );
			if( toker->curr()!=')' ) exp( "')'" );
			toker->next();
			result=d_new CallNode( ident,tag,exprs.release() );
		}else{
			//must be a var
			VarNode *var=parseVar( ident,tag );
			result=d_new VarExprNode( var );
			//cout << "VAR" << "\n";
		}
		break;
        default:
            if( !opt ) exp( "expression" );
    }
    
    return result;
}
