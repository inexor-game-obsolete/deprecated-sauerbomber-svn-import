#include "engine.h"

namespace engine {

#ifdef SERVMODE
    void getclientvar(char *var) {
        // ;

    }
    ICOMMAND(getclientvar, "ss", (char *var), getclientvar(var));
#else
    struct servervar {
        int lastupdate;
        char* value;
    };
    hashtable<char*, servervar> servervars;

    void getservervar(char *var) {
        // result(servervars.);
    }
    ICOMMAND(getservervar, "ss", (char *var), getservervar(var));
#endif

}
