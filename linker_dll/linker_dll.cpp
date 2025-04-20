#include <iostream>
#include "../linker/linker.h"


extern "C" {

// This runs when the .so is loaded (like DLL_PROCESS_ATTACH)
__attribute__((constructor))
void library_init() {
	linkerGetLinker();
    std::cout << "Linker Library loaded!" << std::endl;
}

// This runs when the .so is unloaded (like DLL_PROCESS_DETACH)
__attribute__((destructor))
void library_fini() {
    std::cout << "Linker Library unloaded!" << std::endl;
}

}
