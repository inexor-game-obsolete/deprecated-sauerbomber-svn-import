#include <svncpp/client.hpp>
#include "callbacklistener.h"
#include <string>
#include <iostream>

using namespace std;

namespace engine {

    char* svn_lastmessage = new char[255];
    int svn_workers = 0;

    void set_lastmessage(const char* message) {
        sprintf(svn_lastmessage, "  %s", message);
        cout << svn_lastmessage << endl;
    }

    /**
     * Checkout of a new repository.
     */
    bool svn_checkout(char *folder, char *url) {
        svn::Context* context = new svn::Context(std::string(".svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* l = new callbacklistener(&set_lastmessage);
        context->setListener(l);
        svn::Path destFolder(folder);
        try {
            sprintf(svn_lastmessage, "starting checkout \"%s\" to %s", url, folder);
            svnClient->checkout(url, destFolder, svn::Revision::HEAD, true);
            sprintf(svn_lastmessage, "successfully checked out \"%s\" to %s", url, folder);
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "checkout failed: %s", err.message());
        } catch(...) {
            sprintf(svn_lastmessage, "checkout failed: unknown");
        }
        delete svnClient;
        delete context;
        return true;
    }

    /**
     * Updates the repository <repositoryname>
     */
    int svn_update(void *folderPtr) {
        char *folder = (char*)folderPtr;
        // cout << "svn_update: \"" << folder << "\"" << endl;
        svn::Context* context = new svn::Context(std::string(".svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* l = new callbacklistener(&set_lastmessage);
        context->setListener(l);
        svn::Path destFolder(folder);
        try {
            sprintf(svn_lastmessage, "updating %s", folder);
            svnClient->update(destFolder, svn::Revision::HEAD, true, true);
            sprintf(svn_lastmessage, "successfully updated %s", folder);
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "update of %s failed: %s", folder, err.message());
        } catch(...) {
            sprintf(svn_lastmessage, "update of %s failed: unknown", folder);
        }
        delete svnClient;
        delete context;
        return true;
    }

    /**
     * Unlocking a repository
     */
    int svn_unlock(char *folder) {
        svn::Context* context = new svn::Context(std::string(".svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* l = new callbacklistener(&set_lastmessage);
        context->setListener(l);
        svn::Path destFolder(folder);
        try {
            // TODO: svn::unlock
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "unlock of %s failed: %s", folder, err.message());
        } catch(...) {
            sprintf(svn_lastmessage, "unlock of %s failed: unknown", folder);
        }
        delete svnClient;
        delete context;
    }
}
