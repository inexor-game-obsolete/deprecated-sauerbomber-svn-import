#ifndef _SERVICES_SVN_H
#define _SERVICES_SVN_H

#include "callbacklistener.h"
#include "worker.h"
#include "SDL.h"
#include "SDL_thread.h"

namespace engine {

    int svn_workers = 0;
    svn_worker_info svn_worker[MAX_SVN_WORKERS];

}

#endif
