#include "engine.h"
#include "SDL.h"
#include "SDL_thread.h"
#include <sys/stat.h>
#include <sys/types.h>

namespace engine {

    extern bool svn_checkout(char *folder, char *url, int revision);
    extern int svn_update(void *folderPtr);
    extern int svn_unlock(char *folder);
    extern char* svn_lastmessage;
    extern char* svn_repositoryname;
    extern int svn_repositoryrevision;
    extern int svn_workers;

    struct checkoutinfo {
        char *repositoryname;
        char *url;
        int revision;
        bool updateable;
    };

    int repositorycheckoutworker(void *checkoutinfosPtr) {
        svn_workers++;
        checkoutinfo* checkoutinfos = (checkoutinfo*)checkoutinfosPtr;
        int revision = checkoutinfos->revision;
        bool updateable = checkoutinfos->updateable;
        defformatstring(dataurl)("%s/data", (checkoutinfo*)checkoutinfos->url);
        defformatstring(packagesurl)("%s/packages", (checkoutinfo*)checkoutinfos->url);
        defformatstring(localpath)("%s/%s", repositoriesdir, (checkoutinfo*)checkoutinfos->repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        #ifdef WIN32
            CreateDirectory(localpath, NULL);
        #else
            mkdir(localpath, S_IREAD | S_IWRITE | S_IEXEC);
        #endif
        if (svn_checkout(datapath, dataurl, revision) && svn_checkout(packagespath, packagesurl, revision)) {
            addpackagedir(localpath);
            if (!updateable) {
                defformatstring(datasvn)("%s/.svn", datapath);
                defformatstring(datasvnprotected)("%s/.svn_protected", datapath);
                defformatstring(packagessvn)("%s/.svn", packagespath);
                defformatstring(packagessvnprotected)("%s/.svn_protected", packagespath);
                rename(datasvn, datasvnprotected);
                rename(packagessvn, packagessvnprotected);
            }
        } else {
            conoutf("svn checkout failed");
        }
        SDL_Delay(1000);
        svn_workers--;
        return 0;
    }

    void repositorycheckout(char *repositoryname, char *url) {
        checkoutinfo *checkoutinfos = new checkoutinfo;
        checkoutinfos->repositoryname = repositoryname;
        checkoutinfos->url = url;
        checkoutinfos->revision = -1;
        checkoutinfos->updateable = true;
        // SDL_Thread *thread_repositorycheckout =
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(500);
    }
    ICOMMAND(repositorycheckout, "ss", (char *repositoryname, char *url), repositorycheckout(repositoryname, url));

    void repositorycheckoutonce(char *repositoryname, char *url, int revision) {
        checkoutinfo *checkoutinfos = new checkoutinfo;
        checkoutinfos->repositoryname = repositoryname;
        checkoutinfos->url = url;
        checkoutinfos->revision = revision;
        checkoutinfos->updateable = false;
        // SDL_Thread *thread_repositorycheckout =
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(500);
    }
    ICOMMAND(repositorycheckoutonce, "ssi", (char *repositoryname, char *url, int *revision), repositorycheckoutonce(repositoryname, url, *revision));

    void addrepository(char *repositoryname, char *url) {
        checkoutinfo *checkoutinfos = new checkoutinfo;
        checkoutinfos->repositoryname = repositoryname;
        checkoutinfos->url = url;
        checkoutinfos->revision = 0;
        checkoutinfos->updateable = false;
        // SDL_Thread *thread_repositorycheckout =
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(500);
    }
    ICOMMAND(addrepository, "ss", (char *repositoryname, char *url), addrepository(repositoryname, url));

    int repositoryupdateworker(void *repositoryname) {
        svn_workers++;
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);

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
        SDL_Delay(1000);
        svn_workers--;
        return success;
    }

    void repositoryupdate(char *repositoryname) {
        // SDL_Thread *thread_repositoryupdate =
        SDL_CreateThread(repositoryupdateworker, (void*) repositoryname);
        SDL_Delay(500);
    }
    ICOMMAND(repositoryupdate, "s", (char *repositoryname), repositoryupdate(repositoryname));

    void repositoryunlock(char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        svn_unlock(datapath);
        svn_unlock(packagespath);
    }
    ICOMMAND(repositoryunlock, "s", (char *repositoryname), repositoryunlock(repositoryname));

    void removerepository(char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(packagespath)("%s/packages", repositoriesdir, repositoryname);
        // TODO: delete or move to trash
        // rename(localpath, localpath);
    }
    ICOMMAND(removerepository, "s", (char *repositoryname), removerepository(repositoryname));

    void setrepositoryfor(char *path, char *repositoryname) {
        // TODO
    }
    ICOMMAND(setrepositoryfor, "ss", (char *path, char *repositoryname), setrepositoryfor(path, repositoryname));

    void getrepositoriesfor(char *path) {
        // TODO
    }
    ICOMMAND(getrepositoriesfor, "s", (char *path), getrepositoriesfor(path));

    void getrepositoryname() {
        commandret->setstr(newstring(svn_repositoryname));
    }
    COMMAND(getrepositoryname, "");

    void getrepositoryrevision() {
        intret(svn_repositoryrevision);
    }
    COMMANDN(getrepositoryrevision, getrepositoryrevision, "i");

    void getrepositorymsg() {
        commandret->setstr(newstring(svn_lastmessage));
    }
    COMMAND(getrepositorymsg, "");

    void getrepositoryworkers() {
        intret(svn_workers);
    }
    COMMANDN(getrepositoryworkers, getrepositoryworkers, "i");

}
