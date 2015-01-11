/*
 * main_buffer.cpp
 *
 *  Created on: 09 янв. 2015 г.
 *      Author: tarasov
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Buffer.h"

/*
 Implement a buffer class that can be used to manage a binary
 data buffer. It should support copying into/from it, appending,
 resizing, and getting the underlying buffer as char*. The goal
 is to come up with a functional and clean interface. For example,
 here is how this buffer class can be used.

 Extend this class (or create a new implementation) to support the
 "multiple underlying buffers" strategy instead of/in addition to
 "one contiguous underlying buffer" (the idea is to minimize
 reallocation and copying).
 */



int main(int argc, char *argv[])
{

    //TODO const char* buf = ...
    char buf[1024] = "";

    //TODO size_t n = ...
    size_t n = sizeof(buf);

    //TODO buffer b; // Allocate an empty buffer.
    BinaryBuffer b; // Allocate an empty buffer.


    //TODO b.copy (buf, n); // Copy data into the buffer, growing the underlying
    //TODO // buffer if necessary. After this call b.size () == n.
    b.copy(buf, n); // Copy data into the buffer, growing the underlying
                    // buffer if necessary. After this call b.size () == n.


    assert(b.size() == (int )n);

    //TODO buffer b1 (n); // Allocate a buffer 'n' bytes long.
    //TODO memcpy (b1.data (), buf, n);
    BinaryBuffer b1(n); // Allocate a buffer 'n' bytes long.
    memcpy(b1.data(), buf, n);



    //TODO buffer b2 (0, 1024); // Allocate a buffer 0 bytes long with capacity
    //TODO // to grow without reallocation to 1024 bytes.
//    BinaryBuffer b2(0, 1024); // Allocate a buffer 0 bytes long with capacity
//    // to grow without reallocation to 1024 bytes.
//
//    //TODO b2.size (n);
//    //TODO memcpy (b2.data (), buf, n);
//    b2.size(n);
//    memcpy(b2.data(), buf, n);

    BinaryBuffer b3(1024, 5 * 1024); // Allocate a buffer 1024 bytes long with
    // capacity to grow without reallocation to
    // 5*1024 bytes.
    memset(b3.data(), 0, 1024);

    for (int i = 0; i < 5; ++i)
    {
        b3.append(buf, n); // Append the data to the buffer.
    }

    if (b3.capacity() < 10 * 1024)
    {
        b3.capacity(10 * 1024); // Make sure the buffer has this capacity.
    }

    BinaryBuffer b4(buf, n); // Allocate a buffer 'n' bytes long and copy the data

    BinaryBuffer b5(buf, n, 5 * 1024); // Allocate a buffer 'n' bytes long and
    // with capacity to grow without
    // reallocation to 5*1024 bytes. Copy
    // the date over.

    BinaryBuffer b6(buf, n, n, true);  // Assume ownership of the buffer with
    // size 'n' (second argument) and capacity
    // 'n' (third argument).

}
