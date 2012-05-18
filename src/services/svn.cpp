#include <svncpp/client.hpp>
#include <svncpp/targets.hpp>
#include "callbacklistener.h"
#include <string>
#include <iostream>

using namespace std;

namespace engine {

    char* svn_lastmessage = new char[255];
    char* svn_repositoryname = new char[255];
    int svn_repositoryrevision = -1;
    int svn_workers = 0;

    void set_lastmessage(const char* message) {
        sprintf(svn_lastmessage, "  %s", message);
        cout << svn_lastmessage << endl;
    }

    /**
     * Checkout of a new repository.
     */
    bool svn_checkout(char *folder, char *url, int revision) {
        svn::Context* context = new svn::Context(std::string("repositories/home/svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* listener = new callbacklistener(&set_lastmessage);
        context->setListener(listener);
        svn::Path destFolder(folder);
        try {
            sprintf(svn_lastmessage, "starting checkout \"%s\" to %s", url, folder);
            if (revision == -1) {
                svnClient->checkout(url, destFolder, svn::Revision::HEAD, true);
            } else {
                // svn::Revision* svnRevision = new svn::Revision((long int)revision);
                svnClient->checkout(url, destFolder, (long int)revision, true);
            }
            sprintf(svn_lastmessage, "successfully checked out \"%s\" to %s", url, folder);
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "checkout \"%s\" to %s failed: %s", url, folder, err.message());
        } catch(svn::Exception& err2) {
            sprintf(svn_lastmessage, "checkout \"%s\" to %s failed: %s", url, folder, err2.message());
        } catch(exception& err3) {
            sprintf(svn_lastmessage, "checkout \"%s\" to %s failed: %s", url, folder, err3.what());
        } catch(...) {
            sprintf(svn_lastmessage, "checkout \"%s\" to %s failed: unknown", url, folder);
        }
        delete svnClient;
        delete context;
        delete listener;
        return true;
    }

    /**
     * Updates the repository <repositoryname>
     */
    int svn_update(void *folderPtr) {
        char *folder = (char*)folderPtr;
        // cout << "svn_update: \"" << folder << "\"" << endl;
        svn::Context* context = new svn::Context(std::string("repositories/home/svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* listener = new callbacklistener(&set_lastmessage);
        context->setListener(listener);
        svn::Path destFolder(folder);
        try {
            sprintf(svn_lastmessage, "updating %s", folder);
            svnClient->update(destFolder, svn::Revision::HEAD, true, true);
            sprintf(svn_lastmessage, "successfully updated %s", folder);
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "update of %s failed: %s", folder, err.message());
        } catch(svn::Exception& err2) {
            sprintf(svn_lastmessage, "update of %s failed: %s", folder, err2.message());
        } catch(exception& err3) {
            sprintf(svn_lastmessage, "update of %s failed: %s", folder, err3.what());
        } catch(...) {
            sprintf(svn_lastmessage, "update of %s failed: unknown", folder);
        }
        delete svnClient;
        delete context;
        delete listener;
        return true;
    }

    /**
     * Unlocking a repository
     */
    int svn_unlock(char *folder) {
        svn::Context* context = new svn::Context(std::string("repositories/home/svnconfig"));
        svn::Client* svnClient = new svn::Client(context);
        callbacklistener* listener = new callbacklistener(&set_lastmessage);
        context->setListener(listener);
        // svn::Path destFolder(folder);
		try {
            svn::Targets* targets = new svn::Targets(folder);
            svnClient->unlock(*targets, true);
        } catch(svn::ClientException& err) {
            sprintf(svn_lastmessage, "unlock of %s failed: %s", folder, err.message());
        } catch(...) {
            sprintf(svn_lastmessage, "unlock of %s failed: unknown", folder);
        }
        delete svnClient;
        delete context;
        delete listener;
        return true;
    }
}
