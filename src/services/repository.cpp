#include "repository.h"

namespace engine {

    void initrepositoryworkers() {
        for (int worker_id=0; worker_id < MAX_SVN_WORKERS; worker_id++) {
            svn_worker[worker_id].active = false;
            svn_worker[worker_id].type = newstring("");
            svn_worker[worker_id].repositoryname = newstring("");
            svn_worker[worker_id].revision = -1;
            svn_worker[worker_id].action = newstring("");
            svn_worker[worker_id].path = newstring("");
            svn_worker[worker_id].mime_type = newstring("");
        }
    }

    int repositorycheckoutworker(void *checkoutinfosPtr) {
        // conoutf("rcw1");
        svn_checkout_info* checkoutinfos = (svn_checkout_info*)checkoutinfosPtr;
        int revision = checkoutinfos->revision;
        bool updateable = checkoutinfos->updateable;
        int worker_id = svn_workers;
        int success = true;
        svn_workers++;
        svn_worker[worker_id].active = true;
        svn_worker[worker_id].type = newstring("checkout");
        svn_worker[worker_id].repositoryname = newstring(checkoutinfos->repositoryname);
        svn_worker[worker_id].revision = checkoutinfos->revision;
        svn_worker[worker_id].action = newstring("");
        svn_worker[worker_id].path = newstring("");
        svn_worker[worker_id].mime_type = newstring("");
        defformatstring(dataurl)("%s/data", (svn_checkout_info*)checkoutinfos->url);
        defformatstring(packagesurl)("%s/packages", (svn_checkout_info*)checkoutinfos->url);
        defformatstring(localpath)("%s/%s", repositoriesdir, (svn_checkout_info*)checkoutinfos->repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        // conoutf("rcw2");
        #ifdef WIN32
        CreateDirectory(repositoriesdir, NULL);
        CreateDirectory(localpath, NULL);
        #else
        mkdir(repositoriesdir, S_IREAD | S_IWRITE | S_IEXEC);
        mkdir(localpath, S_IREAD | S_IWRITE | S_IEXEC);
        #endif
        // conoutf("rcw3");
        if (svn_checkout(worker_id, datapath, dataurl, revision) && svn_checkout(worker_id, packagespath, packagesurl, revision)) {
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
            success = false;
        }
        // conoutf("rcw4");
        SDL_Delay(3000);
        svn_worker[worker_id].active = false;
        SDL_Delay(1000);
        svn_workers--;
        // conoutf("rcw5");
        return success;
    }

    int repositoryupdateworker(void *repositoryname) {
        int worker_id = svn_workers;
        svn_workers++;
        svn_worker[worker_id].active = true;
        svn_worker[worker_id].type = newstring("update");
        svn_worker[worker_id].repositoryname = newstring((const char*)repositoryname);
        svn_worker[worker_id].revision = -1;
        svn_worker[worker_id].action = newstring("");
        svn_worker[worker_id].path = newstring("");
        svn_worker[worker_id].mime_type = newstring("");
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        int success = true;
        if (svn_update(worker_id, datapath) && svn_update(worker_id, packagespath)) {

        } else {
            conoutf("svn update failed");
            success = false;
        }
        SDL_Delay(3000);
        svn_worker[worker_id].active = false;
        SDL_Delay(1000);
        svn_workers--;
        return success;
    }

    void addrepository(const char *repositoryname, const char *url) {
        svn_checkout_info *checkoutinfos = new svn_checkout_info;
        checkoutinfos->repositoryname = newstring(const_cast<char*>(repositoryname));
        checkoutinfos->url = newstring(const_cast<char*>(url));
        checkoutinfos->revision = 0;
        checkoutinfos->updateable = false;
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(100);
    }
    ICOMMAND(addrepository, "ss", (const char *repositoryname, const char *url), addrepository(repositoryname, url));

    void repositorycheckout(const char *repositoryname, const char *url) {
      // conoutf("rc1");
        svn_checkout_info *checkoutinfos = new svn_checkout_info;
        checkoutinfos->repositoryname = newstring(const_cast<char*>(repositoryname));
        checkoutinfos->url = newstring(const_cast<char*>(url));
        checkoutinfos->revision = -1;
        checkoutinfos->updateable = true;
        // conoutf("rc2");
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        // conoutf("rc3");
        SDL_Delay(100);
        // conoutf("rc4");
    }
    ICOMMAND(repositorycheckout, "ss", (const char *repositoryname, const char *url), repositorycheckout(repositoryname, url));

    void repositorycheckoutonce(const char *repositoryname, const char *url, int revision) {
        svn_checkout_info *checkoutinfos = new svn_checkout_info;
        checkoutinfos->repositoryname = newstring(const_cast<char*>(repositoryname));
        checkoutinfos->url = newstring(const_cast<char*>(url));
        checkoutinfos->revision = revision;
        checkoutinfos->updateable = false;
        SDL_CreateThread(repositorycheckoutworker, (void*) checkoutinfos);
        SDL_Delay(100);
    }
    ICOMMAND(repositorycheckoutonce, "ssi", (const char *repositoryname, const char *url, int *revision), repositorycheckoutonce(repositoryname, url, *revision));

    void repositoryupdate(const char *repositoryname) {
        SDL_CreateThread(repositoryupdateworker, (void*) repositoryname);
        SDL_Delay(100);
    }
    ICOMMAND(repositoryupdate, "s", (const char *repositoryname), repositoryupdate(repositoryname));

    void repositoryunlock(const char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        svn_unlock(datapath);
        svn_unlock(packagespath);
    }
    ICOMMAND(repositoryunlock, "s", (const char *repositoryname), repositoryunlock(repositoryname));

    void removerepository(const char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(packagespath)("%s/packages", repositoriesdir, repositoryname);
        // TODO: delete or move to trash
        // rename(localpath, localpath);
    }
    ICOMMAND(removerepository, "s", (const char *repositoryname), removerepository(repositoryname));

    void setrepositoryfor(const char *path, const char *repositoryname) {
        // TODO
    }
    ICOMMAND(setrepositoryfor, "ss", (const char *path, const char *repositoryname), setrepositoryfor(path, repositoryname));

    void getrepositoriesfor(const char *path) {
        // TODO
    }
    ICOMMAND(getrepositoriesfor, "s", (const char *path), getrepositoriesfor(path));

    void _getrepositoryworkers() {
        intret(svn_workers);
    }
    COMMANDN(getrepositoryworkers, _getrepositoryworkers, "i");

    void _getmaxrepositoryworkers() {
        intret(MAX_SVN_WORKERS);
    }
    COMMANDN(getmaxrepositoryworkers, _getmaxrepositoryworkers, "i");

    ICOMMAND(isworkeractive, "i", (int *worker_id), {
        if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
        intret(svn_worker[*worker_id].active);
    });

    ICOMMAND(workerrepositorytype, "i", (int *worker_id), {
        if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
        commandret->setstr(newstring(svn_worker[*worker_id].type));
    });

    ICOMMAND(workerrepositoryname, "i", (int *worker_id), {
        if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
        commandret->setstr(newstring(svn_worker[*worker_id].repositoryname));
    });

    ICOMMAND(workerrepositoryaction, "i", (int *worker_id), {
        if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
        commandret->setstr(newstring(svn_worker[*worker_id].action));
    });

    void _workerrepositoryrevision(int *worker_id) {
      if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
      intret(svn_worker[*worker_id].revision);
    }
    COMMANDN(workerrepositoryrevision, _workerrepositoryrevision, "i");

    ICOMMAND(workerrepositorypath, "i", (int *worker_id), {
        if (*worker_id < 0 || *worker_id >= MAX_SVN_WORKERS) return;
        commandret->setstr(newstring(svn_worker[*worker_id].path));
    });


}

