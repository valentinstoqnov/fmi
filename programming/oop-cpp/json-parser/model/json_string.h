#ifndef JSON_PARSER_JSON_STRING_H
#define JSON_PARSER_JSON_STRING_H

#include "../ds/mystring.h"
#include "json_value.h"

class JsonString : public JsonValue {
  String content_;
 public:
  explicit JsonString(const char*);
  explicit JsonString(const String&);
  static JsonString* Deserialize(const char*&);
  friend std::ostream& operator<<(std::ostream&, const JsonString&);
  void Serialize(std::ostream& ostream,
                 bool pretty, unsigned depth) const override;
};

#endif //JSON_PARSER_JSON_STRING_H
