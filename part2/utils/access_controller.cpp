#include "access_controller.h"
#include <iostream>
#include <cstring>

template<class ArchiveType>
void AccessController::serialize(ArchiveType& archive, const unsigned int version) {
    archive & file_owner;
    archive & user_access_rights;
}

void AccessController::define_owner(uid_t uid) {
    file_owner = uid;
}

uid_t AccessController::current_owner() const {
    return file_owner;
}

void AccessController::grant_access(uid_t uid, int rights) {
    user_access_rights[uid] = rights;
}

void AccessController::revoke_access(uid_t uid) {
    user_access_rights.erase(uid);
}

bool AccessController::read_from_file(const std::string& file_path) {
    char attr_buffer[4096];
    ssize_t data_size = extattr_get_file(file_path.c_str(), EXTATTR_NAMESPACE_USER, 
                                       "customacl", attr_buffer, sizeof(attr_buffer));
    if(data_size <= 0) return false;

    std::stringstream data_stream(std::string(attr_buffer, data_size));
    boost::archive::binary_iarchive data_archive(data_stream);
    data_archive >> *this;
    return true;
}

bool AccessController::write_to_file(const std::string& file_path) {
    std::stringstream data_stream;
    boost::archive::binary_oarchive data_archive(data_stream);
    data_archive << *this;
    std::string serialized_data = data_stream.str();

    ssize_t bytes_written = extattr_set_file(file_path.c_str(), EXTATTR_NAMESPACE_USER,
                                           "customacl", serialized_data.c_str(), 
                                           serialized_data.size());
    if(bytes_written < 0) {
        std::cerr << "Operation failed: " << strerror(errno) << " (" << errno << ")\n";
        return false;
    }
    return true;
}

std::string AccessController::id_to_user(uid_t uid) {
    struct passwd *user_entry = getpwuid(uid);
    return user_entry ? user_entry->pw_name : "";
}

bool AccessController::has_permission(uid_t uid, int needed_rights) const {
    const auto user_entry = user_access_rights.find(uid);
    if(user_entry != user_access_rights.end() && 
      (user_entry->second & needed_rights) == needed_rights) {
        return true;
    }
    return (uid == file_owner) && needed_rights;
}

uid_t AccessController::user_to_id(const std::string& username) {
    struct passwd *user_entry = getpwnam(username.c_str());
    return user_entry ? user_entry->pw_uid : static_cast<uid_t>(-1);
}
int AccessController::decode_permissions(const std::string& rights_str) {
    int result = 0;
    for(char c : rights_str) {
        switch(c) {
            case 'r': result |= 0b100; break;
            case 'w': result |= 0b010; break;
            case 'x': result |= 0b001; break;
        }
    }
    return result;
}

std::string AccessController::encode_permissions(int rights) {
    return {
        (rights & 4) ? 'r' : '-',
        (rights & 2) ? 'w' : '-',
        (rights & 1) ? 'x' : '-'
    };
}

void AccessController::display() const {
    std::cout << "File Owner: " << id_to_user(file_owner) << '\n';
    std::cout << "Access Entries:\n";
    for(const auto& entry : user_access_rights) {
        std::cout << "  " << id_to_user(entry.first)
                << ": " << encode_permissions(entry.second) << '\n';
    }
}