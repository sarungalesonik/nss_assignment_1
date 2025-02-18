#ifndef ACCESS_CONTROLLER_H
#define ACCESS_CONTROLLER_H

#include <sys/types.h>
#include <sys/extattr.h>
#include <pwd.h>
#include <grp.h>
#include <map>
#include <string>
#include <sstream>
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class AccessController {
private:
    uid_t file_owner;
    std::map<uid_t, int> user_access_rights;

    friend class boost::serialization::access;
    template<class ArchiveType>
    void serialize(ArchiveType& archive, const unsigned int version);

public:
    AccessController() : file_owner(0) {}

    bool read_from_file(const std::string& file_path);
    bool write_to_file(const std::string& file_path);

    static uid_t user_to_id(const std::string& username);
    static int decode_permissions(const std::string& rights_str);
    static std::string id_to_user(uid_t uid);
    static std::string encode_permissions(int rights);

    void revoke_access(uid_t uid);
    void define_owner(uid_t uid);
    void grant_access(uid_t uid, int rights);
    uid_t current_owner() const;
    bool has_permission(uid_t uid, int needed_rights) const;

    void display() const;
};

#endif 