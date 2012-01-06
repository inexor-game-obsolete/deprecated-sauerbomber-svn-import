#include "engine.h"

namespace engine {

    SVARP(repositoriesdir, "repositories");

    extern bool svn_checkout(char *folder, char *url);
    extern bool svn_update(char *folder);

    void fetchrepository(char *repositoryname, char *url) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(msg)("checkout repository %s from %s", repositoryname, url);
        conoutf(msg);
        if (svn_checkout(localpath, url)) {
            addpackagedir(localpath);
        } else {
            conoutf("svn checkout failed");
        }
    }
    ICOMMAND(fetchrepository, "ss", (char *repositoryname, char *url), fetchrepository(repositoryname, url));

    void repositoryupdate(char *repositoryname) {
        defformatstring(localpath)("%s/%s", repositoriesdir, repositoryname);
        defformatstring(msg)("updating repository %s", repositoryname);
        conoutf(msg);
        if (!svn_update(localpath)) {
            conoutf("svn update failed");
        }
    }
    ICOMMAND(repositoryupdate, "ss", (char *repositoryname), repositoryupdate(repositoryname));

}
