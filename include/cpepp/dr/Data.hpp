#ifndef CPEPP_DR_DATA_H
#define CPEPP_DR_DATA_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class ConstDataElement {
public:
    ConstDataElement(const void * data, LPDRMETAENTRY entry, size_t capacity = 0);

    operator int8_t(void) const;
    operator uint8_t(void) const;
    operator int16_t(void) const;
    operator uint16_t(void) const;
    operator int32_t(void) const;
    operator uint32_t(void) const;
    operator int64_t(void) const;
    operator uint64_t(void) const;
    operator float(void) const;
    operator double(void) const;
    operator const char *(void) const;

    int8_t asInt8(void) const { return *this; }
    uint8_t asUInt8(void) const { return *this; }
    int16_t asInt16(void) const { return *this; }
    uint16_t asUInt16(void) const { return *this; }
    int32_t asInt32(void) const { return *this; }
    uint32_t asUInt32(void) const { return *this; }
    int64_t asInt64(void) const { return *this; }
    uint64_t asUInt64(void) const { return *this; }
    float asFloat(void) const { return *this; }
    double asDouble(void) const { return *this; }
    Cpe::Utils::CString const & asString(void) const { return Cpe::Utils::CString::_cast((const char *)(*this)); }

    ConstData operator[] (size_t pos) const;

    const void * data(void) const { return m_data; }
    size_t capacity(void) const { return m_capacity; }
    Entry const & entry(void) const { return *(Entry const *)m_entry; }
    Meta const & meta(void) const { return *(Meta const *)dr_entry_self_meta(m_entry); }

protected:
    const void * m_data;
    size_t m_capacity;
    LPDRMETAENTRY m_entry;
};

class DataElement : public ConstDataElement {
public:
    DataElement(void * data, LPDRMETAENTRY entry, size_t capacity = 0);

    DataElement & operator=(int8_t d);
    DataElement & operator=(uint8_t d);
    DataElement & operator=(int16_t d);
    DataElement & operator=(uint16_t d);
    DataElement & operator=(int32_t d);
    DataElement & operator=(uint32_t d);
    DataElement & operator=(int64_t d);
    DataElement & operator=(uint64_t d);
    DataElement & operator=(const char * d);
    DataElement & operator=(ConstDataElement const & o);

    using ConstDataElement::operator[];
    Data operator[](size_t pos);

    void copy(const void * data, size_t capacity);
    void copy(ConstData const & data);
    void copy(Data const & data);
    void copy(ConstDataElement const & data);
    void copy(DataElement const & data);

    template<typename T>
    void copy(T const & data) { copy(&data, sizeof(data)); }
};

class ConstData {
public:
    ConstData(const void * data, LPDRMETA meta = 0, size_t capacity = 0);

    Meta const & meta(void) const { return *((Meta*)m_meta); }
    const void * data(void) const { return m_data; }
    size_t capacity(void) const { return m_capacity; }

    ConstDataElement operator[](const char * name) const;

protected:
    const void * m_data;
    size_t m_capacity;
    LPDRMETA m_meta;
};

class Data : public ConstData {
public:
    Data(void * data, LPDRMETA meta, size_t capacity = 0);
    Data(void * data, size_t capacity = 0);

    using ConstData::data;
    void * data(void)  { return const_cast<void*>(m_data); }

    using ConstData::operator[];
    DataElement operator[](const char * name);

    void setMeta(LPDRMETA meta);
    void setCapacity(size_t capacity);
    void copySameEntriesFrom(ConstData const & o, error_monitor_t em = 0);

    void copy(const void * data, size_t capacity);
    void copy(ConstData const & data);
    void copy(Data const & data);
    void copy(ConstDataElement const & data);
    void copy(DataElement const & data);
};

inline void DataElement::copy(ConstData const & data) { copy(data.data(), data.capacity()); }
inline void DataElement::copy(Data const & data) { copy(data.data(), data.capacity()); }
inline void DataElement::copy(ConstDataElement const & data) { copy(data.data(), data.capacity()); }
inline void DataElement::copy(DataElement const & data) { copy(data.data(), data.capacity()); }

inline void Data::copy(ConstData const & data) { copy(data.data(), data.capacity()); }
inline void Data::copy(Data const & data) { copy(data.data(), data.capacity()); }
inline void Data::copy(ConstDataElement const & data) { copy(data.data(), data.capacity()); }
inline void Data::copy(DataElement const & data) { copy(data.data(), data.capacity()); }

}}

#endif
