



#include "libs.h"

#include <iostream>

#include "../config/config.h"

#include "../linker/linker.h"
#include "../stdutil/stdutil.h"

using namespace std;

static void err( const string &t ){
	cout<<t<<endl;
	closeLibs();
	exit(-1);
}

int main(){


	if( const char *er=openLibs() ) err( er );

	closeLibs();

	return 0;

}
