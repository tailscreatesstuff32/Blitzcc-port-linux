
#include "stdutil.h"
#include <iostream>

#include <set>
#include <math.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


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
const int MIN_SIZE=256;

qstreambuf::qstreambuf(){
	buf=d_new char[MIN_SIZE];
	setg( buf,buf,buf );
	setp(buf, buf + MIN_SIZE); // originally setp(buf, buf, buf + MIN_SIZE) in MSVC}
}

qstreambuf::~qstreambuf(){
	delete buf;
}

int qstreambuf::size(){
	return pptr()-gptr();
}

char *qstreambuf::data(){
	return gptr();
}



qstreambuf::int_type qstreambuf::underflow(){
return 0;
}

qstreambuf::int_type qstreambuf::overflow( qstreambuf::int_type c ){
return 0;
}




bool copyFile(const char *src, const char *dst) {
    const int buf_size = 4096;
    char buffer[buf_size];

    int in = open(src, O_RDONLY);
    if (in < 0) return false;

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        close(in);
        return false;
    }

    ssize_t bytes;
    while ((bytes = read(in, buffer, buf_size)) > 0) {
        if (write(out, buffer, bytes) != bytes) {
            close(in);
            close(out);
            return false;
        }
    }

    close(in);
    close(out);
    return bytes == 0;
}
