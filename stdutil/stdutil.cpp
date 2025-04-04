
#include "stdutil.h"
#include <iostream>

#include <set>
#include <math.h>
#include <stdlib.h>





#include <string>

using namespace std;


string tolower( const string &s ){
        string t=s;
        for( int k=0;k<t.size();++k ) t[k]=tolower(t[k]);
        return t;
}

string itoa( int n ){
    char buff[32];
//    itoa( n,buff,10 );
 snprintf(buff, sizeof(buff), "%d", n);
  return string( buff );
}
