#ifdef _MSC_VER
  #pragma warning(disable:4786)  // MSVC-specific
#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wunknown-pragmas"  // Ignore unknown pragmas in GCC
#endif


#include "libs.h"

#include <iostream>
#include <fstream>

#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include <math.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

#include "../config/config.h"
#include "../stdutil/stdutil.h"

#include "../linker/linker.h"
#include "../compiler/environ.h"
#include "../compiler/parser.h"




static string verstr( int ver ){
	return itoa((ver&65535)/100)+"."+itoa((ver&65535)%100);
}


static void versInfo(){
	cout<<"Compiler version:"<<verstr(bcc_ver)<<endl;
	cout<<"Runtime version:"<<verstr(run_ver)<<endl;
	cout<<"Debugger version:"<<verstr(dbg_ver)<<endl;
	cout<<"Linker version:"<<verstr(lnk_ver)<<endl;
}


static string quickhelp(const string &kw) {

return 0;


}





static void dumpKeys( bool lang,bool mod,bool help) {







}

static void err( const string &t ){
	cout<<t<<endl;
	exit(-1);
}

static void usageErr(){
	err( "Usage error" );
}

static void showInfo(){
    const int major=(VERSION&0xffff)/100, minor=(VERSION&0xffff)%100;
    cout << "BlitzCC V" << major << "." << minor << endl;
    cout << "(C)opyright 2000-2003 Blitz Research Ltd" << endl;
//    cout << "Special thanks to ChatGPT for assistance with debugging and understanding the code!" << endl;
}

//static void showInfo(){
//	const int major=(VERSION&0xffff)/100,minor=(VERSION&0xffff)%100;
//	cout<<"BlitzCC V"<<major<<"."<<minor<<endl;
//	cout<<"(C)opyright 2000-2003 Blitz Research Ltd"<<endl;
//}

static void showUsage(){
	cout<<"Usage: blitzcc [-h|-q|+q|-c|-d|-k|+k|-v|-o exefile] [sourcefile.bb]"<<endl;
}

static void showHelp(){
	showUsage();
	cout<<"-h         : show this help"<<endl;
	cout<<"-q         : quiet mode"<<endl;
	cout<<"+q		  : very quiet mode"<<endl;
	cout<<"-c         : compile only"<<endl;
	cout<<"-d         : debug compile"<<endl;
	cout<<"-k         : dump keywords"<<endl;
	cout<<"+k         : dump keywords and syntax"<<endl;
	cout<<"-v		  : version info"<<endl;
	cout<<"-o exefile : generate executable"<<endl;

}


static void demoError(){
	cout<<"Compiler can not be used standalone in demo version."<<endl;
	exit(0);
}


int _cdecl main(int argc,char *argv[]) {

string out_file;
string in_file;
string args;

bool showhelp=false;
bool versinfo=false;
bool quiet=false;
bool veryquiet=false;
bool dumphelp=false;
bool dumpkeys=false;
bool debug=false;
bool dumpasm=false;
bool compileonly=false;

for( int k=1;k<argc;++k ){

	string t=argv[k];
	t=tolower(t);
	
	if (t=="-h") {
	showhelp=true;
	}else if (t=="-a") {
	dumpasm=true;
	}else if (t=="-q") {
	quiet=true;
	}else if (t=="+q") {
	veryquiet=quiet=true;
	}else if (t=="-c") {
	compileonly=true;
	}else if (t=="-d") {
	debug=true;
	}else if (t=="-k") {
	dumpkeys=true;
	}else if (t=="+k") {
	dumpkeys=dumphelp=true;
	}else if (t=="-v") {
	versinfo=true;
	}else if (t=="-o") {
		if (out_file.size() || k==argc-1)  usageErr();
		out_file=argv[++k];	
	}else{
		if( in_file.size() || t[0]=='-' || t[0]=='+' ) usageErr();
		in_file=argv[k];
		for( ++k;k<argc;++k ){
			string t=argv[k];
			if( t.find(' ')!=string::npos ) t='\"'+t+'\"';
			if( args.size() ) args+=' ';
			args+=t;
		}	
	}

}

if( out_file.size() && !in_file.size() ) usageErr();

//if( const char *er=openLibs() ) err( er );

//if( const char *er=linkLibs() ) err( er );


//TODO figure out why or if its normal that . is  being read as a file

if (showhelp) showHelp();
if (dumpkeys) dumpKeys(true, true, dumphelp);
if (versinfo) versInfo();

if (!in_file.size()) { cout << "in file empty"<< endl; return 0;}

#ifdef DEMO
    if (!getenv("blitzide")) demoError();
#endif

    if (in_file[0] == '\"') {
        if (in_file.size() < 3 || in_file[in_file.size() - 1] != '\"') usageErr();
        in_file = in_file.substr(1, in_file.size() - 2);
    }

    ifstream in(in_file.c_str());
    if (!in) err("Unable to open input file");
    if (!quiet) {
        showInfo();
        cout << "Compiling \"" << in_file << "\"" << endl;
    }

    int n = in_file.rfind('/');
    if (n == string::npos) n = in_file.rfind('\\');
    if (n != string::npos) {
        if (!n || in_file[n - 1] == ':') ++n;
     
    string dir = in_file.substr(0, n);  // Extract the directory part of the path
    cout << "Directory checked: " << dir << endl;  // Print the directory
    chdir(in_file.substr(0, n).c_str());
    }

//    string dir = in_file.substr(0, n);  // Extract the directory part o>
//    cout << "Directory checked: " << dir << endl;  // Print the directo>

    ProgNode *prog=0;
    Environ *environ=0;
    Module *module=0;

   try {
		//parse
	
	if( !veryquiet ) cout<<"Parsing..."<<endl;
		Toker toker( in );
        Parser parser(toker);
        prog=parser.parse( in_file );

		//semant
        if( !veryquiet ) cout<<"Generating..."<<endl;
        environ=prog->semant( runtimeEnviron );
		//translate
		if( !veryquiet ) cout<<"Translating..."<<endl;

    	}catch(Ex &x){
        //cout << x.ex <<endl;
    	string file='\"'+x.file+'\"';
		int row=((x.pos>>16)&65535)+1,col=(x.pos&65535)+1;
		cout<<file<<":"<<row<<":"<<col<<":"<<row<<":"<<col<<":"<<x.ex<<endl;

        exit(-1);
    }
if (prog) delete prog;

return 0;

}


