#ifndef CPEPP_UTILS_CLASSCATEGORY_H
#define CPEPP_UTILS_CLASSCATEGORY_H

namespace Cpe { namespace Utils {

class Noncopyable {
    Noncopyable(Noncopyable const & o);
    Noncopyable & operator=(Noncopyable & o);

protected:
    Noncopyable() {}
};

class Nondestoryable {
    ~Nondestoryable();
};

class Noncreatable {
    Noncreatable();
};

class SimulateObject : public Noncreatable, public Nondestoryable, public Noncopyable {
};

}}

#endif
