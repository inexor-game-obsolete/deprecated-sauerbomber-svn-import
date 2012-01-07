#include "engine.h"
#include <sys/stat.h>
#include <sys/types.h>

namespace engine {

    SVARP(repositoriesdir, "repositories");

    extern bool svn_checkout(char *folder, char *url);
    extern bool svn_update(char *folder);

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

    void repositoryupdate(char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(datapath)("%s/data", localpath);
        defformatstring(packagespath)("%s/packages", localpath);
        defformatstring(msg)("updating repository %s", repositoryname);
        conoutf(msg);
        if (!svn_update(datapath) || !svn_update(packagespath)) {
            conoutf("svn update failed");
        }
    }
    ICOMMAND(repositoryupdate, "ss", (char *repositoryname), repositoryupdate(repositoryname));

}
