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


#ifndef _XMLBEANSXX_STORESTRING_H_
#define _XMLBEANSXX_STORESTRING_H_

//#include <set>
#include <string>
#include "logger.h"
#include <ext/hash_map>



namespace xmlbeansxx {

/** Immutable string with automatic memory deallocation */
class SimpleString {
private:
    char *buf;
    void copyFrom(const SimpleString &from);
public:
    SimpleString(const std::string &s);
    SimpleString(const SimpleString &from);
    ~SimpleString();
    SimpleString &operator=(const SimpleString &b);

    const char *c_str() const;
};

/** Immutable string without memory deallocation */
/*
class CStr {
private:
    const char *buf;
public:
    CStr(const char *buf);
    const char *get() const;
    bool operator<(const CStr &b) const;
};*/

class CStrHashFn {
    public:
    unsigned int operator()(const char *str) const {
        unsigned int v=0;
        while (*str!=0) {
            v+=(unsigned int)(*str)*31;
            str++;
        }
        return v;
    }
};

class CStrLessFn {
    public:
    bool operator()(const char *a,const char *b) const {
        return strcmp(a,b)<0;
    }
};

class CStrEqFn {
    public:
    bool operator()(const char *a,const char *b) const {
        if (a==b) return true; 
        else return strcmp(a,b)==0;
    }
};

/** String storage collection. Contains c_str's. */
class StringStorage {
private:
    //std::set<CStr> contents;
    //typedef std::set<const char *,CStrLessFn> StoreSet;
    typedef __gnu_cxx::hash_map<const char *,int,CStrHashFn,CStrEqFn> StoreMap;
    StoreMap contents;
public:
    struct SSInfo {
        const char *str;
        int hashCode;
    };
    std::vector<SSInfo> stored;
    
    StringStorage();
    void add(const std::string &str);
    void add(const char * cs);
    bool isStored(const char *str);
    unsigned long get(const char *str);
    ~StringStorage();
};

/** Immutable string with ability to remember globally some strings. User can use static method 'store' to remember some strings. */
class StoreString {
private:
    STATIC_LOGGER_PTR(log);

    unsigned long buf;
    inline bool isStored() const;

    void construct(const std::string &str);
    static StringStorage *getStorage();
    inline void copyFrom(const StoreString &from);

public:
    StoreString();
    StoreString(const StoreString &from);
    StoreString(const std::string &str);
    StoreString(const char * str);
    ~StoreString();
    inline const char *c_str() const;
    inline unsigned int hashCode() const;

    inline StoreString &operator=(const StoreString &b);
    inline bool operator ==(const StoreString &str) const;

    inline operator std::string() const;

    std::string toString() const;
    /** Remembers a string in collection, so it won't be allocated several times. */
    static StoreString store(const std::string &str);
    static StoreString store(const char *cs);
    
    class Hash {
    public:
        unsigned int operator()(const StoreString &ss) const {
            return ss.hashCode();
        }
    };

};

inline std::ostream &operator<<(std::ostream &out, const StoreString &str);
inline std::string operator+(const std::string &a,const StoreString &b);
inline std::string operator+(const StoreString &a,const std::string &b);
inline std::string operator+(const StoreString &a,const StoreString &b);

inline bool operator==(const std::string &a,const StoreString &b);
inline bool operator==(const StoreString &a,const std::string &b);


//Inline functions
inline StoreString &StoreString::operator=(const StoreString &b) {
    //TRACER(log,"operator=");
    copyFrom(b);
    return *this;
}

inline void StoreString::copyFrom(const StoreString &from) {
    if (from.isStored()) {
        buf=from.buf;
    } else {
        construct(from.c_str());
    }
}

inline bool StoreString::isStored() const {
    return ((unsigned long)buf)&1;
}

inline const char *StoreString::c_str() const {
    if (isStored()) {
        return getStorage()->stored[buf/2].str; 
    } else { return (char *)buf; }
}

inline unsigned int StoreString::hashCode() const {
    if (isStored()) { return getStorage()->stored[buf/2].hashCode; }
    else return CStrHashFn()((char *)buf);
}

inline bool StoreString::operator ==(const StoreString &str) const {
    if (isStored() && str.isStored()) return buf==str.buf;
    else return CStrEqFn()(c_str(),str.c_str());
}

inline StoreString::operator std::string() const {
    return std::string(const_cast<char *>(c_str()));
}

inline std::ostream &operator<<(std::ostream &out, const StoreString &str) {
    return out<<str.c_str();
}

inline std::string operator+(const std::string &a,const StoreString &b) {
    return a+b.toString();
}

inline std::string operator+(const StoreString &a,const std::string &b) {
    return a.toString()+b;
}

inline std::string operator+(const StoreString &a,const StoreString &b) {
    return a.toString()+b.toString();
}

inline bool operator==(const std::string &a,const StoreString &b) {
    return CStrEqFn()(a.c_str(),b.c_str());
}

inline bool operator==(const StoreString &a,const std::string &b) {
    return CStrEqFn()(a.c_str(),b.c_str());
}

inline bool operator<(const StoreString &a,const StoreString &b) {
    return CStrLessFn()(a.c_str(),b.c_str());
}


class StoreStringHashFn {
    public:
    unsigned int operator()(const StoreString &ss) const {
        return ss.hashCode();
    }
};

}

#endif//_XMLBEANSXX_STORESTRING_H_


