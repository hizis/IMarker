/*
 * Buffer.cpp
 *
 *  Created on: 09 янв. 2015 г.
 *      Author: tarasov
 */

#include <Buffer.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>

//    _M_reallocate(size_type __n)
//    {
//      _Bit_type* __q = this->_M_allocate(__n);
//      this->_M_impl._M_finish = _M_copy_aligned(begin(), end(),
//                        iterator(__q, 0));
//      this->_M_deallocate();
//      this->_M_impl._M_start = iterator(__q, 0);
//      this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
//    }
//
//
//    _M_deallocate()
//    {
//  if (_M_impl._M_start._M_p)
//    _M_impl.deallocate(_M_impl._M_start._M_p,
//               _M_impl._M_end_of_storage - _M_impl._M_start._M_p);
//    }

///////////////////////////////////////////////////////////////////////////////
/// BinaryBuffer
//-----------------------------------------------------------------------------
//--- Constructors & Destructors


BinaryBuffer::BinaryBuffer() :
        m_capacity(0), m_used(0), m_ptr(0), m_ownMem(true)
{
}



BinaryBuffer::BinaryBuffer(std::size_t length) :
m_capacity(0), m_used(0), m_ownMem(false)
{

    std::size_t capacity(1024);

    // sanity check ( ... however could be another solutions for that)
    if (length > capacity)
    {
        capacity = length;
    }

    m_ptr = new char[capacity];
    if (m_ptr)
    {
        m_capacity = capacity;
        m_used = length;
    }
    assert(m_ptr != NULL);
}


BinaryBuffer::BinaryBuffer(std::size_t length, std::size_t capacity) :
m_capacity(0), m_used(0), m_ownMem(false)
{
    // sanity check ( ... however could be another solutions for that)
    if (length > capacity)
    {
        capacity = length;
    }

    m_ptr = new char[capacity];
    if (m_ptr)
    {
        m_capacity = capacity;
        m_used = length;
    }
    assert(m_ptr != NULL);
}


BinaryBuffer::BinaryBuffer(const char * pMem, std::size_t length, std::size_t capacity, bool owner) :
        m_capacity(0), m_used(0), m_ownMem(false)
{
    // sanity check ( ... however could be another solutions for that)
    if (length > capacity)
    {
        capacity = length;
    }

    m_ptr = new char[capacity];

    if (m_ptr)
    {
        m_capacity = capacity;
        if (pMem)
        {
            memcpy(m_ptr, pMem, length);
            m_used = length;
        }
    }
    assert(m_ptr != NULL);
}




//BinaryBuffer::BinaryBuffer(std::size_t capacity) :
//        _capacity(capacity), _used(capacity), _ptr(0), _ownMem(true)
//{
//    if (capacity > 0)
//    {
//        _ptr = new T[capacity];
//    }
//}

BinaryBuffer::BinaryBuffer(const BinaryBuffer& other) :
        m_capacity(other.m_used), m_used(other.m_used), m_ptr(0), m_ownMem(true)
{
    if (m_used)
    {
        m_ptr = new char[m_used];
        std::memcpy(m_ptr, other.m_ptr, m_used * sizeof(char));
    }
}

BinaryBuffer::BinaryBuffer(const char * pMem, std::size_t length):
    m_capacity(length),
    m_used(length),
    m_ptr(0),
    m_ownMem(true)
{
    if (m_capacity > 0)
    {
        m_ptr = new char[m_capacity];
        std::memcpy(m_ptr, pMem, m_used * sizeof(char));
    }
}


BinaryBuffer::~BinaryBuffer()
{
    if (m_ownMem)
    {
        delete[] m_ptr;
    }
}


//BinaryBuffer::~BinaryBuffer(void)
//{
//    if (m_buffer)
//    {
//        delete m_buffer;
//    }
//}
//
////-----------------------------------------------------------------------------
//
void BinaryBuffer::copy(char* str, std::size_t len)
{
    //reallocate
    if (len >= m_capacity)
    {
        if (m_ptr)
        {
            delete m_ptr;
        }
        m_ptr = new char[len];
        if (m_ptr)
        {
            m_capacity = len;
        }
    }

    if (m_ptr)
    {
        m_used = len;
        memcpy(m_ptr, str, len);
    }
//    assert(m_buffer != NULL);
}

BinaryBuffer & BinaryBuffer::operator =(const BinaryBuffer& other)
{
    if (this != &other)
    {
        BinaryBuffer tmp(other);
        swap(tmp);
    }
    return *this;
}

void BinaryBuffer::append(char * buf, std::size_t sz)
{
    if (0 == sz)
    {
        return;
    }
    size(m_used + sz);
    std::memcpy(m_ptr + m_used - sz, buf, sz * sizeof(char));

//    // increase capacity if necessary
//    capacity(m_size + len);
//    // append new data to the buffer
//    if (m_buffer)
//    {
//        memcpy(m_buffer + m_size, str, len);
//        m_size += len;
//    }
//    assert(m_buffer != NULL);
}

void BinaryBuffer::append(const BinaryBuffer & buf)
    /// Resizes this BufferImpl and appends the argument BufferImpl.
{
    append(buf.m_ptr, buf.m_used);
}

//void BinaryBuffer::capacity(int capacity)
//{
//    if (capacity > m_capacity)
//    {
//        // should resize internal buffer
//        char *tmpbuf = new char[capacity];
//        if (tmpbuf && m_buffer)
//        {
//            memcpy(tmpbuf, m_buffer, m_size);
//            delete m_buffer;
//            m_buffer = tmpbuf;
//            m_capacity = capacity;
//        }
//    }
//    assert(m_buffer != NULL);
//}

void BinaryBuffer::capacity(std::size_t newCapacity)
{
    if (!m_ownMem) throw "Cannot resize BufferImpl which does not own its storage.";

    if (newCapacity != m_capacity)
    {
        char * ptr = 0;
        if (newCapacity > 0)
        {
            ptr = new char[newCapacity];
            std::size_t newSz = m_used < newCapacity ? m_used : newCapacity;
            std::memcpy(ptr, m_ptr, newSz * sizeof(char));
        }
        delete [] m_ptr;
        m_ptr = ptr;
        m_capacity = newCapacity;

        if (newCapacity < m_used)
        {
            m_used = newCapacity;
        }

    }
}

void BinaryBuffer::size(std::size_t newsize)
{
    if (newsize > m_capacity)
    {
        capacity(newsize);
    }
    m_used = newsize;
}


void BinaryBuffer::swap(BinaryBuffer& other)
/// Swaps the BufferImpl with another one.
{
    using std::swap;

    swap(m_ptr, other.m_ptr);
    swap(m_capacity, other.m_capacity);
    swap(m_used, other.m_used);
}
