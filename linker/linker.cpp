#include "std.h"
#include "linker.h"
#include "image_util.h"



class BBModule : public Module {

public:
    BBModule();
	BBModule( istream &in );
	~BBModule();

	void *link( Module *libs );
	bool createExe( const char *exe_file,const char *dll_file );

	int getPC();

	void emit( int byte );
	void emitw( int word );
	void emitd( int dword );
	void emitx( void *mem,int sz );
	bool addSymbol( const char *sym,int pc );
	bool addReloc( const char *dest_sym,int pc,bool pcrel );

	bool findSymbol( const char *sym,int *pc );

private:
    char *data = nullptr;
    int data_sz = 0, pc = 0;
    bool linked = false;

    map<string, int> symbols;
    map<int, string> rel_relocs, abs_relocs;

    bool findSym(const string &t, Module *libs, int *n) {
        if (findSymbol(t.c_str(), n)) return true;
        if (libs && libs->findSymbol(t.c_str(), n)) return true;
        cerr << "Blitz Linker Error: Symbol '" << t << "' not found" << endl;
        return false;
    }

    void ensure(int n) {
        if (pc + n <= data_sz) return;
        data_sz = data_sz / 2 + data_sz;
        if (data_sz < pc + n) data_sz = pc + n;
        char *old_data = data;
        data = new char[data_sz];
        if (old_data) {
            memcpy(data, old_data, pc);
            delete[] old_data;
        }
    }

};

BBModule::BBModule():data(0),data_sz(0),pc(0),linked(false){
}

BBModule::~BBModule(){
if (linked) {
munmap(data, data_sz);
}else{
delete[] data;
}



}




void *BBModule::link(Module *libs) {
    if (linked) return data;

    int dest;
    map<int, string>::iterator it;

    long page_size = sysconf(_SC_PAGESIZE);
    size_t alloc_size = (pc + page_size - 1) & ~(page_size - 1);

    char *p = reinterpret_cast<char*>(
        mmap(nullptr, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC,
             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
    );
    if (p == MAP_FAILED) return nullptr;

    memcpy(p, data, pc);
    delete[] data;
    data = p;
    linked = true;

    for (it = rel_relocs.begin(); it != rel_relocs.end(); ++it) {
        if (!findSym(it->second, libs, &dest)) return nullptr;
        int *ptr = reinterpret_cast<int*>(data + it->first);
        *ptr += (dest - reinterpret_cast<intptr_t>(ptr));
    }

    for (it = abs_relocs.begin(); it != abs_relocs.end(); ++it) {
        if (!findSym(it->second, libs, &dest)) return nullptr;
        int *ptr = reinterpret_cast<int*>(data + it->first);
        *ptr += dest;
    }

    return data;
}

int BBModule::getPC(){
	return pc;
}




//NOT USED
/*BBModule::BBModule( istream &in ){

}*/

void BBModule::emit( int byte ){
	ensure(1);data[pc++]=byte;
}

void BBModule::emitw( int word ){
	ensure(2);*(short*)(data+pc)=word;pc+=2;
}

void BBModule::emitd( int dword ){
	ensure(4);*(int*)(data+pc)=dword;pc+=4;
}

void BBModule::emitx( void *mem,int sz ){
	ensure(sz);memcpy( data+pc,mem,sz );pc+=sz;
}
bool BBModule::addSymbol( const char *sym,int pc ){
	string t(sym);
	if( symbols.find( t )!=symbols.end() ) return false;
	symbols[t]=pc;return true;
}

bool BBModule::addReloc( const char *dest_sym,int pc,bool pcrel ){
	map<int,string> &rel=pcrel ? rel_relocs : abs_relocs;
	if( rel.find( pc )!=rel.end() ) return false;
	rel[pc]=string(dest_sym);return true;
}


bool BBModule::findSymbol( const char *sym,int *pc ){
        string t=string(sym);
        map<string,int>::iterator it=symbols.find( t );
        if( it==symbols.end() ) return false;
        // *pc=it->second + (int)data;
        *pc = it->second + reinterpret_cast<intptr_t>(data);
        return true;

}

int Linker::version(){
	return VERSION;
}


bool Linker::canCreateExe(){
#ifdef DEMO
        return false;
#else
        return true;
#endif
}

Module *Linker::createModule(){
       return d_new BBModule();
}

void Linker::deleteModule( Module *mod ){
        delete mod;
}

Linker * linkerGetLinker(){
	static Linker linker;return &linker;
}

bool BBModule::createExe( const char *exe_file,const char *dll_file )   {

#ifdef DEMO
    return false;
#else

void *handle = dlopen(dll_file, RTLD_LAZY);
    if (!handle) return false;

    void *proc = dlsym(handle, "bbMain");  // Adjust "bbMain" to match actual symbol
    if (!proc) {
        dlclose(handle);
        return false;
    }

    int entry = (char *)proc - (char *)handle;
    dlclose(handle);

    if (!copyFile(dll_file, exe_file)) return false;

    if (!openImage(exe_file)) return false;

    makeExe( entry );


	//create module
	//code size: code...
	//num_syms:  name,val...
	//num_rels:  name,val...
	//num_abss:  name,val...
	//


    qstreambuf buf;
    iostream out( &buf );

	map<string,int>::iterator it;
	map<int,string>::iterator rit;

	//write the code
	int sz=pc;out.write( (char*)&sz,4 );out.write( data,pc );

	//write symbols
	sz=symbols.size();out.write( (char*)&sz,4 );
	for( it=symbols.begin();it!=symbols.end();++it ){
		string t=it->first+'\0';
		out.write( t.data(),t.size() );
		sz=it->second;out.write( (char*)&sz,4 );
	}

	//write relative relocs
	sz=rel_relocs.size();out.write( (char*)&sz,4 );
	for( rit=rel_relocs.begin();rit!=rel_relocs.end();++rit ){
		string t=rit->second+'\0';
		out.write( t.data(),t.size() );
		sz=rit->first;out.write( (char*)&sz,4 );
	}

	//write absolute relocs
	sz=abs_relocs.size();out.write( (char*)&sz,4 );
	for( rit=abs_relocs.begin();rit!=abs_relocs.end();++rit ){
		string t=rit->second+'\0';
		out.write( t.data(),t.size() );
		sz=rit->first;out.write( (char*)&sz,4 );
	}

	replaceRsrc( 10,1111,1033,buf.data(),buf.size() );

	closeImage();

	return true;

#endif

 }
