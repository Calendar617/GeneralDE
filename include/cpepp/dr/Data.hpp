#ifndef CPEPP_DR_DATA_H
#define CPEPP_DR_DATA_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class ConstDataElement {
public:
    ConstDataElement(const void * data, LPDRMETAENTRY entry);

    operator int8_t(void);
    operator uint8_t(void);
    operator int16_t(void);
    operator uint16_t(void);
    operator int32_t(void);
    operator uint32_t(void);
    operator int64_t(void);
    operator uint64_t(void);
    operator const char *(void);

protected:
    const void * m_data;
    LPDRMETAENTRY m_entry;
};

class DataElement : public ConstDataElement {
public:
    DataElement(void * data, LPDRMETAENTRY entry);

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
    ConstData(const void * data, LPDRMETA meta);

    Meta const & meta(void) const { return *((Meta*)m_meta); }

    ConstDataElement operator[](const char * name) const;

protected:
    const void * m_data;
    LPDRMETA m_meta;
};

class Data : public ConstData {
public:
    Data(void * data, LPDRMETA meta);

    using ConstData::operator[];
    DataElement operator[](const char * name);
};

}}

#endif
