#ifndef USF_LOGIC_OPCONTEXT_H
#define USF_LOGIC_OPCONTEXT_H
#include "cpepp/dr/System.hpp"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOpData {
public:
    LogicOpData(Cpe::Dr::Meta const & meta, size_t capacity);

    Cpe::Dr::Meta const & meta(void) const { return m_meta; }
    void * data(void) { return (this + 1); }
    const void * data(void) const { return (this + 1); }
    size_t capacity(void) const { return m_capacity; }

    template<typename T>
    T & as(void) { return *(T *)data(); }

    template<typename T>
    T const & as(void) const { return *(T const *)data(); }

private:
    Cpe::Dr::Meta const & m_meta;
    size_t m_capacity;
};

class LogicOpDataIterator {
public:
    virtual size_t count(void) const = 0;
    virtual LogicOpData & at(size_t pos) = 0;
    virtual LogicOpData const & at(size_t pos) const = 0;
    virtual ~LogicOpDataIterator();
};

class LogicOpContext {
public:
    virtual LogicOpContextID id(void) const = 0;
    virtual bool debug(void) const = 0;
    virtual Cpe::Dr::MetaLib const & metaLib(void) const = 0;

    /*data operations*/
    virtual LogicOpDataIterator & datas(void) = 0;
    virtual LogicOpDataIterator const & datas(void) const = 0;

    virtual LogicOpData & data(const char * name) = 0;
    virtual LogicOpData const & data(const char * name) const = 0;
    virtual LogicOpData * findData(const char * name) = 0;
    virtual LogicOpData const * findData(const char * name) const = 0;
    virtual LogicOpData & checkCreateData(const char * name, size_t capacity = 0) = 0;
    virtual LogicOpData & checkCreateData(Cpe::Dr::Meta const & meta, size_t capacity = 0) = 0;

    virtual void dump(cfg_t cfg) const = 0;

    virtual ~LogicOpContext();

    static ::std::auto_ptr<LogicOpContext>
    create(
        LogicOpContextID id,
        Cpe::Dr::MetaLib const & metaLib
        , bool debug);
};

}}

#endif
