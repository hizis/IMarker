/*
 * Buffer.h
 *
 *  Created on: 09 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <cstddef>



/// Binary Buffer
// @short Class:  the binary buffer class
class BinaryBuffer
{
public:
    typedef char ValueType;

    BinaryBuffer();

    BinaryBuffer(std::size_t length);

    // Allocate a buffer of 'len' bytes long with capacity
    // to grow without reallocation to 'capacity' bytes.
    BinaryBuffer(std::size_t length, std::size_t capacity);
//    BinaryBuffer(std::size_t capacity);
//
//    // Allocate a buffer 'len' bytes long and with capacity to grow without
//    // reallocation to 'capacity' bytes. Copy the data over.
//    BinaryBuffer(char* buf, int len, int capacity = 1024, bool owner = true);
    BinaryBuffer(const char * pMem, std::size_t length, std::size_t capacity, bool owner = true);
    BinaryBuffer(const char * pMem, std::size_t length);

    // Copy constructor.
    BinaryBuffer(const BinaryBuffer& other);

    // Destructor deletes allocated internal buffer if exists
    virtual ~BinaryBuffer(void);
//
//    // Copy data into the buffer, growing the underlying
//    // buffer if necessary. After this call b.size () == n.
    void copy(char* str, std::size_t len);
    BinaryBuffer & operator = (const BinaryBuffer & other);


    // Append data to the buffer, growing the underlying
    // buffer if necessary. After this call b.size () == n.
    void append(char* buf, std::size_t sz);
    void append(const BinaryBuffer & buf);
//    void append(BinaryBuffer& s)
//    {
//        append(s.m_buffer, s.m_size);
//    }

    // Get pointer to the data buffer

    /*const*/ char* data() /*const*/
        /// Returns a pointer to the beginning of the BufferImpl.
    {
        return m_ptr;
    }

//
//    const T* begin() const
//        /// Returns a pointer to the beginning of the BufferImpl.
//    {
//        return _ptr;
//    }
//
//    T* end()
//        /// Returns a pointer to end of the BufferImpl.
//    {
//        return _ptr + _used;
//    }
//
//    const T* end() const
//        /// Returns a pointer to the end of the BufferImpl.
//    {
//        return _ptr + _used;
//    }
//

//    // Get size of the buffer
//    int size(void)
//    {
//        return m_size;
//    }
//    /// Set new size of the buffer, growing capacity if necessary
//    void size(std::size_t newSize);
//

      // Get capacity of the buffer
//    int capacity(void)
//    {
//        return m_capacity;
//    }

    void capacity(std::size_t newCapacity);

    std::size_t capacity() const;


    void swap(BinaryBuffer& other);

    void size(std::size_t newSize);
    std::size_t size() const
    {
        return m_used;
    }

private:
//    char *m_buffer;
//    int m_capacity;
//    int m_size;
//    bool m_owner;

    std::size_t m_capacity;
    std::size_t m_used;
    char *      m_ptr;
    bool        m_ownMem;


};


inline std::size_t BinaryBuffer::capacity() const
    /// Returns the allocated memory size in elements.
{
    return m_capacity;
}


#endif /* BUFFER_H_ */
