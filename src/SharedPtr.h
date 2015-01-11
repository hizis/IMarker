/*
 * SharedPtr.h
 *
 *  Created on: 08 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef INCLUDE_ROOT_SHAREDPTR_H_
#define INCLUDE_ROOT_SHAREDPTR_H_

#include <cstddef>
#include <algorithm>
#include <cassert>

/**
 * @brief implementation of reference counter for the following minimal smart pointer.
 *
 * SharedPtrCount is a container for the allocated pn reference counter.
 */
class SharedPtrCount
{
public:
    SharedPtrCount() :
            pn(NULL)
    {
    }
    SharedPtrCount(const SharedPtrCount& count) :
            pn(count.pn)
    {
    }
    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(SharedPtrCount& lhs) throw() // never throws
    {
        std::swap(pn, lhs.pn);
    }
    /// @brief getter of the underlying reference counter
    long useCount(void) const throw() // never throws
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
    }
    /// @brief acquire/share the ownership of the pointer, initializing the reference counter
    template<class U>
    void acquire(U* p) // may throw std::bad_alloc
    {
        if (NULL != p)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1); // may throw std::bad_alloc
                }
                catch (std::bad_alloc&)
                {
                    delete p;
                    throw; // rethrow the std::bad_alloc
                }
            }
            else
            {
                ++(*pn);
            }
        }
    }
    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    template<class U>
    void release(U* p) throw() // never throws
    {
        if (NULL != pn)
        {
            --(*pn);
            if (0 == *pn)
            {
                delete p;
                delete pn;
            }
            pn = NULL;
        }
    }

public:
    long* pn; //!< Reference counter
};

/**
 * @brief minimal implementation of smart pointer, a subset of the C++11 std::SharedPtr or boost::SharedPtr.
 *
 * SharedPtr is a smart pointer retaining ownership of an object through a provided pointer,
 * and sharing this ownership with a reference counter.
 * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
 */
template<class T>
class SharedPtr
{
public:
    /// The type of the managed object, aliased as member type
    typedef T element_type;

    /// @brief Default constructor
    SharedPtr(void) throw() : // never throws
            px(NULL), pn()
    {
    }
    /// @brief Constructor with the provided pointer to manage
    explicit SharedPtr(T* p) : // may throw std::bad_alloc
            //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
            pn()
    {
        acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Constructor to share ownership. Warning : to be used for pointer_cast only ! (does not manage two separate <T> and <U> pointers)
    template<class U>
    SharedPtr(const SharedPtr<U>& ptr, T* p) :
            //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
            pn(ptr.pn)
    {
        acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Copy constructor to convert from another pointer type
    template<class U>
    SharedPtr(const SharedPtr<U>& ptr) throw() :
            //px(ptr.px),
            pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count())); // must be coherent : no allocation allowed in this path
        acquire(static_cast<typename SharedPtr<T>::element_type*>(ptr.px));   // will never throw std::bad_alloc
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    SharedPtr(const SharedPtr& ptr) throw() : // never throws (see comment below)
            //px(ptr.px),
            pn(ptr.pn)
    {
        assert((NULL == ptr.px) || (0 != ptr.pn.useCount())); // must be cohérent : no allocation allowed in this path
        acquire(ptr.px);   // will never throw std::bad_alloc
    }
    /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
    SharedPtr& operator=(SharedPtr ptr) throw() // never throws
    {
        swap(ptr);
        return *this;
    }
    /// @brief the destructor releases its ownership
    inline ~SharedPtr(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset releases its ownership
    inline void reset(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset release its ownership and re-acquire another one
    void reset(T* p) // may throw std::bad_alloc
    {
        assert((NULL == p) || (px != p)); // auto-reset not allowed
        release();
        acquire(p); // may throw std::bad_alloc
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(SharedPtr& lhs) throw() // never throws
    {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    // reference counter operations :
    inline operator bool() const throw() // never throws
    {
        return (0 < pn.useCount());
    }

    inline bool empty()
    {
        return (0 == pn.useCount());
    }

    inline bool unique(void) const throw() // never throws
    {
        return (1 == pn.useCount());
    }
    long use_count(void) const throw() // never throws
    {
        return pn.useCount();
    }

    // underlying pointer operations :
    inline T& operator*() const throw() // never throws
    {
        SHARED_ASSERT(NULL != px);
        return *px;
    }
    inline T* operator->() const throw() // never throws
    {
        assert(NULL != px);
        return px;
    }
    inline T* get(void) const throw() // never throws
    {
        // no assert, can return NULL
        return px;
    }

private:
    /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
    inline void acquire(T* p) // may throw std::bad_alloc
    {
        pn.acquire(p); // may throw std::bad_alloc
        px = p; // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
    }

    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    inline void release(void) throw() // never throws
    {
        pn.release(px);
        px = NULL;
    }

private:
    // This allow pointer_cast functions to share the reference counter between different SharedPtr types
    template<class U>
    friend class SharedPtr;

private:
    T* px; //!< Native pointer
    SharedPtrCount pn; //!< Reference counter
};

// comparaison operators
template<class T, class U> inline bool operator==(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() == r.get());
}
template<class T, class U> inline bool operator!=(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() != r.get());
}
template<class T, class U> inline bool operator<=(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() <= r.get());
}
template<class T, class U> inline bool operator<(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() < r.get());
}
template<class T, class U> inline bool operator>=(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() >= r.get());
}
template<class T, class U> inline bool operator>(const SharedPtr<T>& l, const SharedPtr<U>& r) throw() // never throws
{
    return (l.get() > r.get());
}

// static cast of SharedPtr
template<class T, class U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U>& ptr) // never throws
{
    return SharedPtr<T>(ptr, static_cast<typename SharedPtr<T>::element_type*>(ptr.get()));
}

// dynamic cast of SharedPtr
template<class T, class U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U>& ptr) // never throws
{
    T* p = dynamic_cast<typename SharedPtr<T>::element_type*>(ptr.get());
    if (NULL != p)
    {
        return SharedPtr<T>(ptr, p);
    }
    else
    {
        return SharedPtr<T>();
    }
}

#endif /* INCLUDE_ROOT_SHAREDPTR_H_ */
