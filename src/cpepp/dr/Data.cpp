#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "cpe/dr/dr_data.h"
#include "cpepp/dr/Data.hpp"

namespace Cpe { namespace Dr {

//class ConstDataElement
ConstDataElement::ConstDataElement(const void * data, LPDRMETAENTRY entry)
    : m_data(data)
    , m_entry(entry)
{
}

ConstDataElement::operator int8_t(void) {
    int8_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_int8(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read int8 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator uint8_t(void) {
    uint8_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_uint8(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read uint8 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator int16_t(void) {
    int16_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_int16(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read int16 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator uint16_t(void) {
    uint16_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_uint16(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read uint16 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator int32_t(void) {
    int32_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_int32(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read int32 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator uint32_t(void) {
    uint32_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_uint32(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read uint32 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator int64_t(void) {
    int64_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_int64(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read int64 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator uint64_t(void) {
    uint64_t r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_uint64(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read uint64 from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator const char *(void) {
    const char * r = dr_entry_read_string(m_data, m_entry);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "read string from " << dr_entry_name(m_entry) << ": error!";
        throw ::std::runtime_error(os.str());
    }
    return r;
}

//class DataElement
DataElement::DataElement(void * data, LPDRMETAENTRY entry) 
    : ConstDataElement(data, entry)
{
}

DataElement & DataElement::operator=(int8_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_int8(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set int8(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(uint8_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_uint8(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set uint8(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(int16_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_int16(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set int16(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(uint16_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_uint16(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set uint16(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(int32_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_int32(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set int32(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(uint32_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_uint32(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set uint32(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(int64_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_int64(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set int64(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(uint64_t d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_uint64(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set uint64(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

DataElement & DataElement::operator=(const char * d) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_string(const_cast<void *>(m_data), d, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set string(" << d << ") to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

//class ConstData
ConstData::ConstData(const void * data, LPDRMETA meta)
    : m_data(data)
    , m_meta(meta)
{
}

ConstDataElement ConstData::operator[](const char * name) const {
    LPDRMETAENTRY entry;
    int32_t off = dr_meta_path_to_off(m_meta, name, &entry);
    if (off < 0 || entry == NULL) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " have no entry " << name << "!";
        throw ::std::runtime_error(os.str());
    }

    return ConstDataElement(((const char *)m_data) + off, entry);
}

//class Data
Data::Data(void * data, LPDRMETA meta)
    : ConstData(data, meta)
{
}

DataElement Data::operator[](const char * name) {
    LPDRMETAENTRY entry;
    int32_t off = dr_meta_path_to_off(m_meta, name, &entry);
    if (off < 0 || entry == NULL) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " have no entry " << name << "!";
        throw ::std::runtime_error(os.str());
    }

    return DataElement(((char *)const_cast<void*>(m_data)) + off, entry);
}

}}
