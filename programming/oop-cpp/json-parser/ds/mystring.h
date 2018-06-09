//
// Created by valio_stoyanov on 5/14/18.
//

#ifndef JSON_PARSER_STRING_H
#define JSON_PARSER_STRING_H

#include <ostream>

class String {
  char* buffer_;
  size_t size_;
  size_t capacity_;
  void EnsureCapacity(size_t);
 public:
  String(const char*);
  String(const String&);
  ~String();

  String& operator=(const String&);
  friend std::ostream& operator<<(std::ostream&, const String&);
  bool operator==(const String& other) const;
  bool operator!=(const String& other) const;
  bool operator<(const String& other) const;
  bool operator>(const String& other) const;
  bool operator<=(const String& other) const;
  bool operator>=(const String& other) const;
  char& operator[](const size_t) ;
  char& operator[](const size_t) const;

  void PushBack(char);
  String& Append(const String&);
  String Substr(size_t pos, size_t n) const;
  int IndexOf(char) const;
  void Clear();
  char& At(size_t);
  char& Back();
  char& Front();
  const char& Front() const;
  const char& Back() const;
  char& At(size_t) const;
  size_t Length() const;
  bool Empty() const;
};

#endif //JSON_PARSER_STRING_H
