#ifndef CPEPP_UTILS_FUNCTIONALEXT_H
#define CPEPP_UTILS_FUNCTIONALEXT_H
#include <memory>
#include "System.hpp"

namespace Cpe { namespace Utils {

template<typename T>
struct CopyConstructCopyer {
    ::std::auto_ptr<T> operator()(T const * p) const {
        return ::std::auto_ptr<T>(new T(*p));
    }
};

template<typename T>
struct Deletor {
    void operator()(T * p) const throw() {
        delete p;
    }
};

}}

#endif
