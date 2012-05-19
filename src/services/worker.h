#ifndef _SERVICES_WORKER_H
#define _SERVICES_WORKER_H

#define MAX_SVN_WORKERS 20
#define ERROR_NO_SVN_WORKERS_FREE -1

namespace engine {

    struct svn_worker_info {
        bool active;
        int revision;
        char* type;
        char* repositoryname;
        char* path;
        char* mime_type;
        char* action;
        // char* message;
    };

}

#endif
