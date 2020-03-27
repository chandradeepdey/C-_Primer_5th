#include "Message_Folder.h"

#include <set>
#include <string>

Folder &Folder::operator=(const Folder &other) {
    if (this != &other) {
        remove_from_Messages();
        messages = other.messages;
        add_to_Messages(other);
    }
    return *this;
}

Message &Message::operator=(const Message &other) {
    if (this != &other) {
        remove_from_Folders();
        contents = other.contents;
        folders = other.folders;
        add_to_Folders(other);
    }
    return *this;
}

void swap(Folder &lhs, Folder &rhs) {
    lhs.remove_from_Messages();
    rhs.remove_from_Messages();
    // std::swap(std::set) is in <set>
    std::swap(lhs.messages, rhs.messages);
    lhs.add_to_Messages(lhs);
    rhs.add_to_Messages(rhs);
}

void swap(Message &lhs, Message &rhs) {
    lhs.remove_from_Folders();
    rhs.remove_from_Folders();
    // std::swap(std::set) is in <set>
    std::swap(lhs.folders, rhs.folders);
    // std::swap(std::string) is in <string>
    std::swap(lhs.contents, rhs.contents);
    lhs.add_to_Folders(lhs);
    rhs.add_to_Folders(rhs);
}
