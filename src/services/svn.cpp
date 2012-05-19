#include <svncpp/client.hpp>
#include <svncpp/targets.hpp>
#include <string>
#include <iostream>

#include "svn.h"

using namespace std;

namespace engine {

    svn::Context context(std::string("repositories/home/svnconfig"));
    svn::Client svnClient(&context);

    /**
     * Checkout of a new repository.
     */
    bool svn_checkout(int worker_id, const char *folder, const char *url, int revision) {
        svn::Context context(std::string("repositories/home/svnconfig"));
        cout << "svnco1 " << worker_id << " " << folder << endl;
        svn::Client svnClient(&context);
        callbacklistener listener(worker_id);
        context.setListener(&listener);
        svn::Path destFolder(folder);
        cout << "svnco2 " << worker_id << " " << folder << endl;
        try {
            // sprintf(svn_worker[worker_id].message, "starting checkout \"%s\" to %s", url, folder);
            if (revision == -1) {
                cout << "svncoA " << worker_id << " " << folder << endl;
                svnClient.checkout(url, destFolder, svn::Revision::HEAD, true);
                cout << "svncoB " << worker_id << " " << folder << endl;
            } else {
                cout << "svncoC " << worker_id << " " << folder << endl;
                svnClient.checkout(url, destFolder, (long int)revision, true);
                cout << "svncoD " << worker_id << " " << folder << endl;
            }
            // sprintf(svn_worker[worker_id].message, "successfully checked out \"%s\" to %s", url, folder);
        } catch(svn::ClientException& err) {
            // sprintf(svn_worker[worker_id].message, "checkout \"%s\" to %s failed: %s", url, folder, err.message());
        } catch(svn::Exception& err2) {
            // sprintf(svn_worker[worker_id].message, "checkout \"%s\" to %s failed: %s", url, folder, err2.message());
        } catch(exception& err3) {
            // sprintf(svn_worker[worker_id].message, "checkout \"%s\" to %s failed: %s", url, folder, err3.what());
        } catch(...) {
            // sprintf(svn_worker[worker_id].message, "checkout \"%s\" to %s failed: unknown", url, folder);
        }
        cout << "svnco3 " << worker_id << " " << folder << endl;
        SDL_Delay(500);
        return true;
    }

    /**
     * Updates the repository <repositoryname>
     */
    bool svn_update(int worker_id, void *folderPtr) {
        const char *folder = (const char*)folderPtr;
        svn::Context context(std::string("repositories/home/svnconfig"));
        svn::Client svnClient(&context);
        callbacklistener listener(worker_id);
        context.setListener(&listener);
        svn::Path destFolder(folder);
        try {
            // sprintf(svn_worker[worker_id].message, "updating %s", folder);
            svnClient.update(destFolder, svn::Revision::HEAD, true, true);
            // sprintf(svn_worker[worker_id].message, "successfully updated %s", folder);
        } catch(svn::ClientException& err) {
            // sprintf(svn_worker[worker_id].message, "update of %s failed: %s", folder, err.message());
        } catch(svn::Exception& err2) {
            // sprintf(svn_worker[worker_id].message, "update of %s failed: %s", folder, err2.message());
        } catch(exception& err3) {
            // sprintf(svn_worker[worker_id].message, "update of %s failed: %s", folder, err3.what());
        } catch(...) {
            // sprintf(svn_worker[worker_id].message, "update of %s failed: unknown", folder);
        }
        return true;
    }

    /**
     * Unlocking a repository
     */
    int svn_unlock(const char *folder) {
        svn::Context context(std::string("repositories/home/svnconfig"));
        svn::Client svnClient(&context);
        // callbacklistener listener(worker_id);
        // context.setListener(&listener);
        svn::Path destFolder(folder);
		    try {
            svn::Targets* targets = new svn::Targets(folder);
            svnClient.unlock(*targets, true);
        } catch(svn::ClientException& err) {
            // sprintf(svn_worker[worker_id].message, "unlock of %s failed: %s", folder, err.message());
        } catch(...) {
            // sprintf(svn_worker[worker_id].message, "unlock of %s failed: unknown", folder);
        }
        return true;
    }
}
