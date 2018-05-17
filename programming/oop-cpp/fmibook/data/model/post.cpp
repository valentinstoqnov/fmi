//
// Created by valio_stoyanov on 5/16/18.
//

#include <cstring>
#include "post.h"

Post::Post()
: content_(nullptr), id_(0)
{}

Post::Post(const String& content, const unsigned id)
: id_(id), content_(content)
{}

Post& Post::operator=(const Post& other) {
    if (this != &other) {
        id_ = other.id_;
        content_ = other.content_;
    }

    return *this;
}

void Post::set_content(const String& content) {
    content_ = content;
}

const String& Post::get_content() const {
    return content_;
}

void Post::set_id(const unsigned id) {
    id_ = id;
}

unsigned Post::get_id() const {
    return id_;
}

void Post::serialize(std::ostream& out) const {
    if (out.good()) {
        out.write(reinterpret_cast<const char *>(&id_), sizeof(unsigned));
        content_.serialize(out);
    }
}

void Post::deserialize(std::istream& in) {
    if (in.good()) {
        in.read(reinterpret_cast<char*>(&id_), sizeof(unsigned));
        content_.deserialize(in);
    }
}