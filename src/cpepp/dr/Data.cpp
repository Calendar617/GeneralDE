#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "cpe/dr/dr_data.h"
#include "cpepp/dr/Data.hpp"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/Entry.hpp"

namespace Cpe { namespace Dr {

//class ConstDataElement
ConstDataElement::ConstDataElement(const void * data, LPDRMETAENTRY entry, size_t capacity)
    : m_data(data)
    , m_capacity(capacity > 0 ? capacity : dr_entry_size(entry))
    , m_entry(entry)
{
}

ConstDataElement::operator int8_t(void) const {
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

ConstDataElement::operator uint8_t(void) const {
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

ConstDataElement::operator int16_t(void) const {
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

ConstDataElement::operator uint16_t(void) const {
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

ConstDataElement::operator int32_t(void) const {
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

ConstDataElement::operator uint32_t(void) const {
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

ConstDataElement::operator int64_t(void) const {
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

ConstDataElement::operator uint64_t(void) const {
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

ConstDataElement::operator float(void) const {
    float r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_float(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read float from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator double(void) const {
    double r;
    Utils::ErrorCollector em;

    if (dr_entry_try_read_double(&r, m_data, m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "read double from " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return r;
}

ConstDataElement::operator const char *(void) const {
    const char * r = dr_entry_read_string(m_data, m_entry);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "read string from " << dr_entry_name(m_entry) << ": error!";
        throw ::std::runtime_error(os.str());
    }
    return r;
}

ConstData ConstDataElement::operator[] (size_t pos) const {
    size_t element_size = dr_entry_element_size(m_entry);
    if ((pos + 1) * element_size > m_capacity) {
        ::std::ostringstream os;
        os << "read array " << pos << " from " << dr_entry_name(m_entry) << ": pos overflow!";
        throw ::std::runtime_error(os.str());
    }

    LPDRMETA element_meta = dr_entry_ref_meta(m_entry);
    if (element_meta == NULL) {
        ::std::ostringstream os;
        os << "read array " << pos << " from " << dr_entry_name(m_entry) << ": no ref type!";
        throw ::std::runtime_error(os.str());
    }

    return ConstData(((const char *)data()) + element_size * pos, element_meta, element_size);
}

//class DataElement
DataElement::DataElement(void * data, LPDRMETAENTRY entry, size_t capacity) 
    : ConstDataElement(data, entry, capacity)
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

DataElement & DataElement::operator=(ConstDataElement const & o) {
    Utils::ErrorCollector em;

    if (dr_entry_set_from_ctype(const_cast<void *>(m_data), o.data(), o.entry().typeId(), m_entry, em) != 0) {
        ::std::ostringstream os;
        os << "set element to " << dr_entry_name(m_entry) << ": ";
        em.genErrorMsg(os);
        throw ::std::runtime_error(os.str());
    }

    return *this;
}

void DataElement::copy(const void * data, size_t capacity) {
    Utils::ErrorCollector em;
    if (capacity > m_capacity) {
        ::std::ostringstream os;
        os << meta().name().c_str() << "." << entry().name().c_str() << ": "
           << "copy data (size=" << capacity << ") overflow, capacity=" << m_capacity << "!";
        throw ::std::runtime_error(os.str());
    }

    memcpy((void*)m_data, data, capacity);
}

Data DataElement::operator[] (size_t pos) {
    size_t element_size = dr_entry_element_size(m_entry);
    if ((pos + 1) * element_size > m_capacity) {
        ::std::ostringstream os;
        os << "read array " << pos << " from " << dr_entry_name(m_entry) << ": pos overflow!";
        throw ::std::runtime_error(os.str());
    }

    LPDRMETA element_meta = dr_entry_ref_meta(m_entry);
    if (element_meta == NULL) {
        ::std::ostringstream os;
        os << "read array " << pos << " from " << dr_entry_name(m_entry) << ": no ref type!";
        throw ::std::runtime_error(os.str());
    }

    return Data(((char *)data()) + element_size * pos, element_meta, element_size);
}


//class ConstData
ConstData::ConstData(const void * data, LPDRMETA meta, size_t capacity)
    : m_data(data)
    , m_capacity(capacity > 0 ? capacity : dr_meta_size(meta))
    , m_meta(meta)
{
}

ConstDataElement ConstData::operator[](const char * name) const {
    if (m_meta == NULL) throw ::std::runtime_error("Data::operator[]: meta not exist!");

    LPDRMETAENTRY entry;
    int32_t off = dr_meta_path_to_off(m_meta, name, &entry);
    if (off < 0 || entry == NULL) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " have no entry " << name << "!";
        throw ::std::runtime_error(os.str());
    }

    if ((size_t)off >= m_capacity) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " entry " << name
           << " off " << off << " overflow, capacity is " << m_capacity << "!";
        throw ::std::runtime_error(os.str());
    }

    if (entry == dr_meta_entry_at(m_meta, dr_meta_entry_num(m_meta) - 1)) {
        return ConstDataElement(((char *)const_cast<void*>(m_data)) + off, entry, m_capacity - (size_t)off);
    }
    else {
        return ConstDataElement(((char *)const_cast<void*>(m_data)) + off, entry);
    }
}

//class Data
Data::Data(void * data, LPDRMETA meta, size_t capacity)
    : ConstData(data, meta, capacity)
{
}

Data::Data(void * data, size_t capacity)
    : ConstData(data, NULL, capacity)
{
}

void Data::copy(const void * data, size_t capacity) {
    if (m_meta == NULL) throw ::std::runtime_error("Data::copy: meta not exist!");

    Utils::ErrorCollector em;
    if (capacity > m_capacity) {
        ::std::ostringstream os;
        os << meta().name().c_str() << ": "
           << "copy data (size=" << capacity << ") overflow, capacity=" << m_capacity << "!";
        throw ::std::runtime_error(os.str());
    }

    memcpy((void*)m_data, data, capacity);
}

void Data::setMeta(LPDRMETA meta) {
    m_meta = meta;
}

void Data::setCapacity(size_t capacity) {
    m_capacity = capacity;
}

void Data::copySameEntriesFrom(ConstData const & o, error_monitor_t em) {
    if (m_meta == NULL) throw ::std::runtime_error("Data::copySameEntriesFrom: meta not exist!");

    meta().copy_same_entries(data(), capacity(), o.data(), o.meta(), o.capacity(), 0, em);
}

DataElement Data::operator[](const char * name) {
    if (m_meta == NULL) throw ::std::runtime_error("Data::operator[]: meta not exist!");

    LPDRMETAENTRY entry;
    int32_t off = dr_meta_path_to_off(m_meta, name, &entry);
    if (off < 0 || entry == NULL) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " have no entry " << name << "!";
        throw ::std::runtime_error(os.str());
    }

    if ((size_t)off >= m_capacity) {
        ::std::ostringstream os;
        os << "meta " << dr_meta_name(m_meta) << " entry " << name
           << " off " << off << " overflow, capacity is " << m_capacity << "!";
        throw ::std::runtime_error(os.str());
    }

    if (entry == dr_meta_entry_at(m_meta, dr_meta_entry_num(m_meta) - 1)) {
        return DataElement(((char *)const_cast<void*>(m_data)) + off, entry, m_capacity - (size_t)off);
    }
    else {
        return DataElement(((char *)const_cast<void*>(m_data)) + off, entry);
    }
}

}}
