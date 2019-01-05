/*************************************************************************************************************
 Copyright (c) 2006-2019 David Dubbeldam, Sofia Calero, Thijs J.H. Vlugt.

    D.Dubbeldam@uva.nl            http://www.acmm.nl/molsim/users/dubbeldam/index.html
    scaldia@upo.es                http://www.upo.es/raspa/sofiacalero.php
    t.j.h.vlugt@tudelft.nl        http://homepage.tudelft.nl/v9k6y

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************/

#pragma once

#include <iostream>
#include <QDataStream>

#include <vector>
#include <memory>
#include <unordered_set>
#include <type_traits>

#include <exception>

class InvalidArchiveVersionException : public std::exception
{
public:
  InvalidArchiveVersionException(const char* file_, int line_, const char* func_, const char* info_ = "") :
      _file (file_),
      _line (line_),
      _func (func_),
      _info (info_)
  {

  }
  QString message() {return "Invalid archive version (upgrade to latest iRASPA version)";}
  const char* get_file() const { return _file; }
  int get_line() const { return _line; }
  const char* get_func() const { return _func; }
  const char* get_info() const { return _info; }
private:
  const char* _file;
  int _line;
  const char* _func;
  const char* _info;
};

class InconsistentArchiveException : public std::exception
{
public:
  InconsistentArchiveException(const char* file_, int line_, const char* func_, const char* info_ = "") :
      _file (file_),
      _line (line_),
      _func (func_),
      _info (info_)
  {

  }
  QString message() {return "Archive is inconsistent (internal bug)";}
  const char* get_file() const { return _file; }
  int get_line() const { return _line; }
  const char* get_func() const { return _func; }
  const char* get_info() const { return _info; }
private:
  const char* _file;
  int _line;
  const char* _func;
  const char* _info;
};


//a function that can serialize any enum into QDataStream
//it stores the enum in a qint64
template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator<<(QDataStream& stream, const Enum& e) {
    stream << static_cast<qint64>(e);
    return stream;
}

//a function that can deserialize any enum from QDataStream
//it reads the enum as if it was stored in qint64
template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator>>(QDataStream& stream, Enum& e) {
    qint64 v;
    stream >> v;
    e = static_cast<Enum>(v);
    return stream;
}

template<class T> QDataStream &operator<<(QDataStream& stream, const std::vector<T>& val)
{
  stream << static_cast<int32_t>(val.size());
  for(const T& singleVal : val)
    stream << singleVal;
  return stream;
}

template<class T> QDataStream &operator>>(QDataStream& stream, std::vector<T>& val)
{
  int32_t vecSize;
  val.clear();
  stream >> vecSize;
  val.reserve(vecSize);
  T tempVal;
  while(vecSize--)
  {
    stream >> tempVal;
    val.push_back(tempVal);
  }
  return stream;
}

template<class T> QDataStream &operator<<(QDataStream& stream, const std::unordered_set<T>& val)
{
  stream << static_cast<int32_t>(val.size());
  for(const T& singleVal : val)
    stream << singleVal;
  return stream;
}

template<class T> QDataStream &operator>>(QDataStream& stream, std::unordered_set<T>& val)
{
  int32_t vecSize;
  val.clear();
  stream >> vecSize;
  val.reserve(vecSize);
  T tempVal;
  while(vecSize--)
  {
    stream >> tempVal;
    val.insert(tempVal);
  }
  return stream;
}

template<class T> QDataStream &operator<<(QDataStream& stream, const std::vector<std::shared_ptr<T>>& val)
{
  stream << static_cast<int32_t>(val.size());
  for(const std::shared_ptr<T>& singleVal : val)
    stream << singleVal;
  return stream;
}

template<class T> QDataStream &operator>>(QDataStream& stream, std::vector<std::shared_ptr<T>>& val)
{
  int32_t vecSize;
  val.clear();
  stream >> vecSize;
  val.reserve(vecSize);

  while(vecSize--)
  {
    std::shared_ptr<T> tempVal = std::make_shared<T>();
    stream >> tempVal;
    val.push_back(tempVal);
  }
  return stream;
}

template<class Key, class T> QDataStream &operator<<(QDataStream& stream, const std::map<Key, T>& table)
{
  stream << static_cast<int32_t>(table.size());
  for(auto const& [key, val] : table)
  {
    stream << key;
    stream << val;
  }
  return stream;
}

template<class Key, class T> QDataStream &operator>>(QDataStream& stream, std::map<Key, T>& table)
{
  int32_t vecSize;
  table.clear();
  stream >> vecSize;
  Key key;
  T value;

  while(vecSize--)
  {
    stream >> key;
    stream >> value;
    table[key] = value;
  }
  return stream;
}
