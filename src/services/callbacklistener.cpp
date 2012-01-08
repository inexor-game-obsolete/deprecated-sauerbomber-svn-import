/*
 * callbacklistener.cpp
 */

#include "callbacklistener.h"
#include <svncpp/context.hpp>
#include <svncpp/context_listener.hpp>
#include <svn_client.h>

using namespace std;

namespace engine {

    callbacklistener::callbacklistener(void (*callback)(const char*)) {
        this->callback = callback;
    }

    void callbacklistener::contextNotify(
        const char* path,
        svn_wc_notify_action_t action,
        svn_node_kind_t kind,
        const char* mime_type,
        svn_wc_notify_state_t content_state,
        svn_wc_notify_state_t prop_state,
        svn_revnum_t revnum
    ) {
        std::stringstream message;
        message << actionToStr(action) << ": '" << path << "', Revision ";
        if (revnum == -1) {
            message << "HEAD";
        } else {
            message << revnum;
        }
        callback(message.str().c_str());
    }

    std::string callbacklistener::actionToStr(svn_wc_notify_action_t action) {
        std::string result;
        switch (action) {
            case svn_wc_notify_add:
                result = std::string("added");
                break;
            case svn_wc_notify_copy:
                result = std::string("copied");
                break;
            case svn_wc_notify_delete:
                result = std::string("deleted");
                break;
            case svn_wc_notify_restore:
                result = std::string("restored");
                break;
            case svn_wc_notify_revert:
                result = std::string("reverted");
                break;
            case svn_wc_notify_failed_revert:
                result = std::string("failed to revert");
                break;
            case svn_wc_notify_resolved:
                result = std::string("resolved conflict");
                break;
            case svn_wc_notify_skip:
                result = std::string("skipped");
                break;
            case svn_wc_notify_update_delete:
                result = std::string("deleting");
                break;
            case svn_wc_notify_update_add:
                result = std::string("adding");
                break;
            case svn_wc_notify_update_update:
                result = std::string("updating");
                break;
            case svn_wc_notify_update_completed:
                result = std::string("up to date");
                break;
            case svn_wc_notify_update_external:
                result = std::string("updating");
                break;
            case svn_wc_notify_status_completed:
                result = std::string("completed");
                break;
            case svn_wc_notify_status_external:
                result = std::string("external");
                break;
            case svn_wc_notify_commit_modified:
                result = std::string("modified");
                break;
            case svn_wc_notify_commit_added:
                result = std::string("added");
                break;
            case svn_wc_notify_commit_deleted:
                result = std::string("deleted");
                break;
            case svn_wc_notify_commit_replaced:
                result = std::string("replaced");
                break;
            case svn_wc_notify_commit_postfix_txdelta:
                result = std::string("post-fix text-delta");
                break;
            case svn_wc_notify_blame_revision:
                result = std::string("blame");
                break;
            case svn_wc_notify_locked:
                result = std::string("locked");
                break;
            case svn_wc_notify_unlocked:
                result = std::string("unlocked");
                break;
            case svn_wc_notify_failed_lock:
                result = std::string("failed locking");
                break;
            case svn_wc_notify_failed_unlock:
                result = std::string("failed unlocking");
                break;
            case svn_wc_notify_exists:
                result = std::string("exists");
                break;
            case svn_wc_notify_changelist_set:
                result = std::string("set changelist name");
                break;
            case svn_wc_notify_changelist_clear:
                result = std::string("cleared changelist name");
                break;
            case svn_wc_notify_changelist_moved:
                result = std::string("moved changelist");
                break;
            case svn_wc_notify_foreign_merge_begin:
            case svn_wc_notify_merge_begin:
                result = std::string("merging");
                break;
            case svn_wc_notify_update_replace:
                result = std::string("replaced");
                break;
            case svn_wc_notify_property_added:
                result = std::string("property added");
                break;
            case svn_wc_notify_property_modified:
                result = std::string("property updated");
                break;
            case svn_wc_notify_property_deleted:
            case svn_wc_notify_property_deleted_nonexistent:
                result = std::string("property deleted");
                break;
            case svn_wc_notify_revprop_set:
                result = std::string("revision property set");
                break;
            case svn_wc_notify_revprop_deleted:
                result = std::string("revision property deleted");
                break;
            case svn_wc_notify_merge_completed:
                result = std::string("merge completed");
                break;
            case svn_wc_notify_tree_conflict:
                result = std::string("tree conflict");
                break;
            case svn_wc_notify_failed_external:
                result = std::string("referenced in an externals definition");
                break;
            default:
                result = std::string("");
                break;
        }
        return result;
    }

}

