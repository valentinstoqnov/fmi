//
// Created by valio_stoyanov on 5/14/18.
//

#ifndef JSON_PARSER_JSON_ARRAY_H
#define JSON_PARSER_JSON_ARRAY_H

#include <glob.h>
#include <iostream>
#include "json_value.h"
#include "../ds/array.h"

class JsonArray : public JsonValue {
  Array<JsonValue*> elements_;
 public:
  ~JsonArray() override;
  void PushBack(JsonValue*);
  void Serialize(std::ostream& out, bool pretty, unsigned depth) const override;
};

#endif //JSON_PARSER_JSON_ARRAY_H

