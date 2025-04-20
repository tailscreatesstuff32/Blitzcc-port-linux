#ifndef LIBS_H
#define LIBS_H

#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#include "../linker/linker.h"
#include "../config/config.h"

#include <dlfcn.h>

extern int bcc_ver;
extern int lnk_ver;

//openLibs
extern Linker *linkerLib;

const char *openLibs();

void closeLibs();

#endif
