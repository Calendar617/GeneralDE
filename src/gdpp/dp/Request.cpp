#include <sstream>
#include <stdexcept>
#include "gdpp/dp/Request.hpp"

namespace Gd { namespace Dp {

Request & Request::parent(cpe_hash_string_t type) {
    Request * r = findParent(type);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "can`t find parent request of type " << cpe_hs_data(type) << "!";
        throw ::std::runtime_error(os.str().c_str());
    }
    return *r;
}

Request & Request::brother(cpe_hash_string_t type) {
    Request * r = findBrother(type);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "can`t find brother request of type " << cpe_hs_data(type) << "!";
        throw ::std::runtime_error(os.str().c_str());
    }
    return *r;
}

Request & Request::child(cpe_hash_string_t type) {
    Request * r = findChild(type);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "can`t find child request of type " << cpe_hs_data(type) << "!";
        throw ::std::runtime_error(os.str().c_str());
    }
    return *r;
}

void Request::checkSize(size_t size) {
    if (size > this->size()) {
        ::std::ostringstream os;
        os << "request have not enough data, require " << size
           << ", but only have " << this->size() << "!";
        throw ::std::runtime_error(os.str().c_str());
    }
}

}}
