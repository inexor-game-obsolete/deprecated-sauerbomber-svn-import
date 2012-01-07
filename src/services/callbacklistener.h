#ifndef CALLBACKLISTENER_H_
#define CALLBACKLISTENER_H_

#include <svncpp/context.hpp>
#include <svncpp/context_listener.hpp>
#include <svn_client.h>
#include <iostream>
#include <sstream>
#include <string>

/*
context.hpp
context_listener.hpp
svn_client.h
Verschluckte includes in src:
iostream
sstream
string
*/

using namespace std;

namespace engine {

    class callbacklistener: public svn::ContextListener {

        public:
            callbacklistener(void (*callback)(const char*));
            bool contextCancel() { return false; };
            bool contextGetLogin(
                const std::string& realm,
                std::string& username,
                std::string& password,
                bool& maySave
            ) { return true; };
            bool contextGetLogMessage(std::string& msg) { callback(msg.c_str()); return true; };
            void contextNotify(
                const char* path,
                svn_wc_notify_action_t action,
                svn_node_kind_t kind,
                const char* mime_type,
                svn_wc_notify_state_t content_state,
                svn_wc_notify_state_t prop_state,
                svn_revnum_t revnum
            );
            bool contextSslClientCertPrompt(std::string& certFile) { return true; };
            bool contextSslClientCertPwPrompt(
                std::string& password,
                const std::string& realm,
                bool& maySave
            ) { return true; };
            SslServerTrustAnswer contextSslServerTrustPrompt(
                const SslServerTrustData& data,
                apr_uint32_t& acceptedFailures
            ) { return ACCEPT_TEMPORARILY; };
        private:
            std::string actionToStr(svn_wc_notify_action_t action);
            void (*callback)(const char*);
    };

}

#endif /* CALLBACKLISTENER_H_ */
