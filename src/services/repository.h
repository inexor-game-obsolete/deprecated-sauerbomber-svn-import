#include "engine.h"
#include "SDL.h"
#include "SDL_thread.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "worker.h"

namespace engine {

    extern bool svn_checkout(int worker_id, const char *folder, const char *url, int revision);
    extern bool svn_update(int worker_id, void *folderPtr);
    extern int svn_unlock(const char *folder);

    // extern char* svn_lastmessage;
    // extern char* svn_repositoryname;
    // extern int svn_repositoryrevision;

    extern int svn_workers;
    extern svn_worker_info svn_worker[20];

    struct svn_checkout_info {
        char* repositoryname;
        char* url;
        int revision;
        bool updateable;
    };

}
