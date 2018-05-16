//
// Created by valio_stoyanov on 5/14/18.
//

#ifndef JSON_PARSER_JSONBOOLEAN_H
#define JSON_PARSER_JSONBOOLEAN_H


#include "json_value.h"

class JsonBoolean: public JsonValue {
    bool value_;
public:
    explicit JsonBoolean(bool);
};


#endif //JSON_PARSER_JSONBOOLEAN_H
