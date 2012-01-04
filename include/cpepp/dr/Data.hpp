#ifndef CPEPP_DR_DATA_H
#define CPEPP_DR_DATA_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class ConstDataElement {
public:
    ConstDataElement(const void * data, Entry const & entry);

    operator int8_t(void);
    operator uint8_t(void);
    operator int16_t(void);
    operator uint16_t(void);
    operator int32_t(void);
    operator uint32_t(void);
    operator int64_t(void);
    operator uint64_t(void);
    operator const char *(void);

private:
    const void * m_data;
    Entry const & m_entry;
};

class DataElement : public ConstDataElement {
public:
    DataElement(void * data, Entry const & entry);

    DataElement & operator=(int8_t d);
    DataElement & operator=(uint8_t d);
    DataElement & operator=(int16_t d);
    DataElement & operator=(uint16_t d);
    DataElement & operator=(int32_t d);
    DataElement & operator=(uint32_t d);
    DataElement & operator=(int64_t d);
    DataElement & operator=(uint64_t d);
    DataElement & operator=(const char * d);
};

class ConstData {
public:
    Meta const & meta(void) const { return m_meta; };

    ConstDataElement operator[](const char * name) const;

private:
    const void * m_data;
    Meta const & m_meta;
};

class Data : public ConstData {
public:
    using ConstData::operator[];

    DataElement operator[](const char * name);
};

}}

#endif
