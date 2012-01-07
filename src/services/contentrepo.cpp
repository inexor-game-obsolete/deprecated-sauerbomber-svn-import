#include "engine.h"
#include "SDL.h"
#include "SDL_thread.h"
#include <sys/stat.h>
#include <sys/types.h>

namespace engine {

    SVARP(repositoriesdir, "repositories");

    extern bool svn_checkout(char *folder, char *url);
    extern int svn_update(void *folderPtr);
    extern char* svn_lastmessage;

    void repositorycheckout(char *repositoryname, char *url) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        defformatstring(dataurl)("%s/data", url);
        defformatstring(packagesurl)("%s/packages", url);
        defformatstring(msg)("checkout repository %s from %s", repositoryname, url);
        conoutf(msg);
        mkdir(localpath, S_IREAD | S_IWRITE | S_IEXEC);
        if (svn_checkout(datapath, dataurl) && svn_checkout(packagespath, packagesurl)) {
            addpackagedir(localpath);
        } else {
            conoutf("svn checkout failed");
        }
    }
    ICOMMAND(repositorycheckout, "ss", (char *repositoryname, char *url), repositorycheckout(repositoryname, url));

    int repositoryupdateworker(void *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        defformatstring(msg)("updating repositories in \"%s\" and \"%s\"", datapath, packagespath);
        conoutf(msg);

        SDL_Thread *thread_data, *thread_packages;
        int thread_data_result, thread_packages_result;
        thread_data = SDL_CreateThread(svn_update, (void*) datapath);
        SDL_WaitThread(thread_data, &thread_data_result);
        int success = true;
        if (!thread_data_result) {
            defformatstring(msg)("updating repository %s failed", datapath);
            conoutf(msg);
            success = false;
        }
        thread_packages = SDL_CreateThread(svn_update, (void*) packagespath);
        SDL_WaitThread(thread_packages, &thread_packages_result);
        if (!thread_packages_result) {
            defformatstring(msg)("updating repository %s failed", packagespath);
            conoutf(msg);
            success = false;
        }
        return success;
    }

    void repositoryupdate(char *repositoryname) {
        SDL_Thread *thread_repositoryupdate = SDL_CreateThread(repositoryupdateworker, (void*) repositoryname);
        SDL_Delay(500);
    }
    ICOMMAND(repositoryupdate, "s", (char *repositoryname), repositoryupdate(repositoryname));

    void repositorydelete(char *repositoryname) {
        // mkdir(localpath, S_IREAD | S_IWRITE | S_IEXEC);
    }
    ICOMMAND(repositorydelete, "s", (char *repositoryname), repositorydelete(repositoryname));

    void getrepositorymsg() {
        commandret->setstr(newstring(svn_lastmessage));
    }
    COMMAND(getrepositorymsg, "");
}
