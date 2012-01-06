#include <svncpp/client.hpp>
#include <iostream>
using namespace std;

namespace engine {

    /**
     * Checkout of a new repository.
     */
    bool svn_checkout(char *folder, char *url) {
        svn::Context* context = new svn::Context(std::string(".svnconfig"));;
        svn::Client* svnClient = new svn::Client(context);
        svn::Path destFolder(folder);
        try {
            cout << "try checkout" << endl << "folder: " << folder << endl << "url: " << url << endl;
            svnClient->checkout(url, destFolder, svn::Revision::HEAD, true);
            cout << "success" << endl;
        } catch(svn::ClientException& err) {
            cout << err.message() << endl;
        } catch(...) {
            cout << "svn checkout failed" << endl;
        }
        delete svnClient;
        delete context;
        return true;
    }

    /**
     * Updates the repository <repositoryname>
     */
    bool svn_update(char *folder) {
        svn::Context* context = new svn::Context(std::string(".svnconfig"));;
        svn::Client* svnClient = new svn::Client(context);
        svn::Path destFolder(folder);
        try {
            svnClient->update(destFolder, svn::Revision::HEAD, true, true);
        } catch(svn::ClientException& err) {
            cout << err.message() << endl;
        } catch(...) {
            cout << "svn update failed" << endl;
        }
        delete svnClient;
        delete context;
        return true;
    }

}
