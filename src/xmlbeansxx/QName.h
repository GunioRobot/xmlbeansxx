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
#ifndef XMLBEANSXX_QNAME_INCLUDED
#define XMLBEANSXX_QNAME_INCLUDED
#include "BoostAssert.h"
#include "StoreString.h"
#include <string>
namespace xmlbeansxx {
/**
* Objects of this class represent qualified name, which contains namespace URI and localPart. Corresponds to javax.xml.namespace.QName class.
*/
class QName: public std::pair<StoreString, StoreString> {
public:
    QName *operator -> () { return this; }
    const QName *operator -> () const { return this; }
    QName(const StoreString &namespacePrefix, const StoreString& localPart) : std::pair<StoreString, StoreString>(namespacePrefix, localPart) {}
    QName() {
        first = "";
        second = "";
    }

    class Hash {
    public:
        unsigned int operator()(const QName &name) const {
            return name.first.hashCode() * 31 ^ name.second.hashCode();
        }
    };
 
    operator std::string() const {
    	return toString();
    }
    std::string toString() const {
    	if(std::string(first).size()>0)	return second + std::string("@") + first;
	else				return second;
    }
    static QName store(const std::string &namespacePrefix, const std::string& localPart) {
    	return QName(StoreString::store(namespacePrefix), StoreString::store(localPart));
    }
    static QName store(const char * namespacePrefix, const char * localPart) {
    	return QName(StoreString::store(namespacePrefix), StoreString::store(localPart));
    }
};

inline bool operator<(const QName& a,const QName &b)
{
	if(a.first==b.first)
		return a.second<b.second;
	return a.first<b.first;
}

}
#endif //XMLBEANSXX_QNAME_INCLUDED
