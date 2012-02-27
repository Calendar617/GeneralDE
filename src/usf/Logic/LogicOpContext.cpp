#include <vector>
#include "cpe/cfg/cfg_manage.h"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/app/Log.hpp"
#include "usf/Logic/LogicOpContext.hpp"

namespace Usf { namespace Logic {

class LogicOpContextImpl
    : public LogicOpContext
    , public LogicOpDataIterator
{
    typedef ::std::vector<LogicOpData*> LogicOpDataContainer;

public:
    LogicOpContextImpl(
        LogicOpContextID id,
        Cpe::Dr::MetaLib const & metaLib,
        bool debug)
        : m_id(id)
        , m_metaLib(metaLib)
        , m_debug(debug)
    {
    }

    ~LogicOpContextImpl() {
        for(LogicOpDataContainer::iterator it = m_datas.begin();
            it != m_datas.end();
            ++it)
        {
            deleteLogicOpData(*it);
        }
        m_datas.clear();
    }

    virtual LogicOpContextID id(void) const {
        return m_id;
    }

    virtual bool debug(void) const {
        return m_debug;
    }

    virtual Cpe::Dr::MetaLib const & metaLib(void) const {
        return m_metaLib;
    }

    virtual LogicOpDataIterator & datas(void) {
        return *this;
    }

    virtual LogicOpDataIterator const & datas(void) const {
        return *this;
    }

    virtual LogicOpData & data(const char * name) {
        LogicOpData * r = findData(name);
        if (r == 0) {
            APP_THROW_EXCEPTION(
                ::std::runtime_error,
                "data %s not exist in context!",
                name);
        }
        return *r;
    }

    virtual LogicOpData const & data(const char * name) const {
        LogicOpData const * r = findData(name);
        if (r == 0) {
            APP_THROW_EXCEPTION(
                ::std::runtime_error,
                "data %s not exist in context!",
                name);
        }
        return *r;
    }

    virtual LogicOpData * findData(const char * name) {
        LogicOpDataContainer::iterator pos = findData_i(name);
        return pos == m_datas.end() ? 0 : *pos;
    }

    virtual LogicOpData const * findData(const char * name) const {
        LogicOpDataContainer::const_iterator pos = findData_i(name);
        return pos == m_datas.end() ? 0 : *pos;
    }

    virtual LogicOpData & checkCreateData(const char * name, size_t capacity) {
        return LogicOpContextImpl::checkCreateData(m_metaLib.meta(name), capacity);
    }

    virtual LogicOpData & checkCreateData(Cpe::Dr::Meta const & meta, size_t capacity) {
        if (capacity == 0) capacity = meta.size();

        LogicOpDataContainer::iterator it = findData_i(meta.name());
        if (it != m_datas.end()) {
            if ((*it)->capacity() < capacity) {
                LogicOpData * logicOpData =
                    createLogicOpData(meta, capacity);
                
                memcpy(logicOpData->data(),
                       (*it)->data(),
                       (*it)->capacity());

                deleteLogicOpData(*it);
                *it = logicOpData;
            }
            return **it;
        }
        else {
            m_datas.reserve(m_datas.size() + 1);

            LogicOpData * logicOpData =
                createLogicOpData(meta, capacity);
            bzero(logicOpData->data(), logicOpData->capacity());

            meta.set_defaults(
                logicOpData->data(),
                logicOpData->capacity());
            m_datas.push_back(logicOpData);
            return *logicOpData;
        }
    }

    virtual size_t count(void) const {
        return m_datas.size();
    }

    virtual LogicOpData & at(size_t pos) {
        return *m_datas[pos];
    }

    virtual LogicOpData const & at(size_t pos) const {
        return *m_datas[pos];
    }

    virtual void dump(cfg_t cfg) const {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", (int)m_id);

        cfg_t roleNode = cfg_struct_add_struct(cfg, buf, cfg_replace);

        for(LogicOpDataContainer::const_iterator it = m_datas.begin();
            it != m_datas.end();
            ++it)
        {
            cfg_t dataNode =
                cfg_struct_add_struct(roleNode, (*it)->meta().name(), cfg_replace);

            (*it)->meta().write_to_cfg(dataNode, (const void *)(*it)->data());
        }
    }

private:
    LogicOpData * createLogicOpData(Cpe::Dr::Meta const & meta, size_t capacity) {
        char * buf = new char[sizeof(LogicOpData) + capacity + 8];
        return new (buf) LogicOpData(meta, capacity);
    }

    void deleteLogicOpData(LogicOpData * data) {
        delete [] (char *)(data);
    }

    LogicOpDataContainer::iterator
    findData_i(const char * name) {
        LogicOpDataContainer::iterator it = m_datas.begin();
        while(it != m_datas.end() && (*it)->meta().name() != name) {
            ++it;
        }
        return it;
    }

    LogicOpDataContainer::const_iterator
    findData_i(const char * name) const {
        LogicOpDataContainer::const_iterator it = m_datas.begin();
        while(it != m_datas.end() && (*it)->meta().name() != name) {
            ++it;
        }
        return it;
    }

    LogicOpContextID const m_id;
    Cpe::Dr::MetaLib const & m_metaLib;
    bool const m_debug;

    LogicOpDataContainer m_datas;
};

LogicOpContext::~LogicOpContext() {
}

::std::auto_ptr<LogicOpContext>
LogicOpContext::create(
    LogicOpContextID id,
    Cpe::Dr::MetaLib const & metaLib,
    bool debug)
{
    return create(id, metaLib, debug);
}

LogicOpData::LogicOpData(Cpe::Dr::Meta const & meta, size_t capacity)
    : m_meta(meta)
    , m_capacity(capacity)
{
}

LogicOpDataIterator::~LogicOpDataIterator() {
}

}}
