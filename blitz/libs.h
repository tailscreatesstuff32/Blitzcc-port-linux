#ifndef LIBS_H
#define LIBS_H

#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#include "../linker/linker.h"
#include "../compiler/environ.h"
#include "../compiler/parser.h"

extern int bcc_ver;
extern int lnk_ver;
extern int run_ver;
extern int dbg_ver;

extern Environ *runtimeEnviron;

#endif
