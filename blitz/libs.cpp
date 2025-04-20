#include "libs.h"

#include <iostream>
#include <dlfcn.h>


Linker* linkerLib;
int bcc_ver;
int lnk_ver;

static void* linkerHMOD;
static void* runtimeHMOD;


const char* openLibs() {
    linkerHMOD = dlopen("./linker.so", RTLD_LAZY);
    if (!linkerHMOD) {
        std::cerr << "Unable to open linker.so: " << dlerror() << std::endl;
        closeLibs();
        exit(-1);
    }

    typedef Linker* (*GetLinker)();
    GetLinker gl = (GetLinker)(dlsym(linkerHMOD, "linkerGetLinker"));
    if (!gl) {
        std::cerr << "Error in linker.so: " << dlerror() << std::endl;
        closeLibs();
        exit(-1);
    }

    linkerLib = gl();
    runtimeHMOD = dlopen("./runtime.so", RTLD_LAZY);
    if( !runtimeHMOD ) {
       std::cerr << "Unable to open runtime.so: " << dlerror() << std::endl;
       closeLibs();
       exit(-1);
    }


    std::cout << linkerLib->version() << std::endl;


    bcc_ver = VERSION;
    lnk_ver = linkerLib->version();

    if ((lnk_ver >> 16) != (bcc_ver >> 16)) {
        return "Library version error";
    }

    return nullptr;
}

void closeLibs() {
    if (linkerHMOD) dlclose(linkerHMOD);
    if (runtimeHMOD) dlclose(runtimeHMOD);

    linkerHMOD = nullptr;
    linkerLib = nullptr;
}
