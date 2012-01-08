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
    extern int svn_workers;

    struct checkoutinfo {
        char *repositoryname;
        char *url;
        // char repositoryname[255];
        // char url[255];
    };

    int repositorycheckoutworker(void *checkoutinfosPtr) {
        svn_workers++;
        checkoutinfo* checkoutinfos = (checkoutinfo*)checkoutinfosPtr;
        defformatstring(dataurl)("%s/data", (checkoutinfo*)checkoutinfos->url);
        defformatstring(packagesurl)("%s/packages", (checkoutinfo*)checkoutinfos->url);
        defformatstring(localpath)("%s/%s", repositoriesdir, (checkoutinfo*)checkoutinfos->repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        // defformatstring(msg)("checkout repository %s from %s", (checkoutinfo*)checkoutinfos->repositoryname, (checkoutinfo*)checkoutinfos->url);
        // conoutf(msg);
        #ifdef WIN32
            CreateDirectory(localpath, NULL);
        #else
            mkdir(localpath, S_IREAD | S_IWRITE | S_IEXEC);
        #endif
        if (svn_checkout(datapath, dataurl) && svn_checkout(packagespath, packagesurl)) {
            addpackagedir(localpath);
        } else {
            conoutf("svn checkout failed");
        }
        SDL_Delay(1000);
        svn_workers--;
    }

    /*
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
    */
    void repositorycheckout(char *repositoryname, char *url) {
        checkoutinfo *checkoutinfos = new checkoutinfo;
        // defformatstring(checkoutinfos->repositoryname)("%s", repositoryname);
        checkoutinfos->repositoryname = repositoryname;
        checkoutinfos->url = url;
        SDL_Thread *thread_repositorycheckout = SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(500);
    }
    ICOMMAND(repositorycheckout, "ss", (char *repositoryname, char *url), repositorycheckout(repositoryname, url));

    int repositoryupdateworker(void *repositoryname) {
        svn_workers++;
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        // defformatstring(msg)("updating repositories in \"%s\" and \"%s\"", datapath, packagespath);
        // conoutf(msg);

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

    void getrepositoryworkers() {
        intret(svn_workers);
    }
    COMMANDN(getrepositoryworkers, getrepositoryworkers, "i");

}
