/*
    Copyright 2004-2005 TouK s.c.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _XMLBEANSXX_DICT_H_
#define _XMLBEANSXX_DICT_H_

#include "BoostAssert.h"
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "macros.h"
#include "StoreString.h"
#include "QName.h"

	
namespace xmlbeansxx {

DECLARE_PTR(std::string,StringPtr,constStringPtr)
class XmlObject;
DECLARE_PTR(XmlObject,XmlObjectPtr,constXmlObjectPtr)

template<class T>
struct DictEl {
    //std::string name;
    QName name;
    T value;

    DictEl() {}
    DictEl(const QName& name,T value):
    name(name),value(value) {}
};

//slownik mapujacy u->v (v jest typu T), pamietajacy kolejnosc elementow
//dziala tak, ze kasujac jakis element nie zwalniamy value, tylko wymazujemy;
//name za to zwalniamy.
//natomiast free() zwalnia name i value.
struct AttrDict {
    typedef std::string T;
    std::vector<DictEl<T> > contents;

public:
    int size() const;
    bool hasEmptyContent() const;
    T find(const QName& u) const;
    int count(const QName& u) const;
    /// Duplicates u, but not v 
    void add
        (const QName& u,T v);
    void set
        (const QName& u,int pos,T v);
    int del(const QName& u);
    void free();
};


class Contents;
//typedef boost::intrusive_ptr<Contents> ContentsPtr;
//typedef Contents * ContentsPtr;
typedef boost::shared_ptr<Contents> ContentsPtr;

struct ElemDict {
    typedef xmlbeansxx::ContentsPtr T;
    std::vector<DictEl<T> > contents;

public:

    int size() const;
    bool hasEmptyContent() const;
    T find(const QName& u,int nr=0) const;
    int count(const QName& u) const;
    /** Duplicates u, but not v */
    void add
        (const QName& u,T v);
    void set
        (const QName& u,int pos,T v);
    int del(const QName& u);
    void removeAt(const QName& name,int index);
    void free();
};
}


#endif //XMLBEANSPP_DICT_H_
