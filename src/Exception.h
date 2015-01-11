/*
 * Exception.h
 *
 *  Created on: 11 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>

class Exception: public std::exception
{
public:

    Exception(const std::string & msg);
    Exception(const Exception & exc);
    ~Exception() throw();

    const std::string & message() const
    {
        return _msg;
    }
private:
    std::string _msg;
};



#define IMPL_EXCEPTION(CLS, BASE, NAME)              \
class CLS : public BASE                              \
{                                                    \
public:                                              \
    CLS(const std::string& msg) :  BASE(msg)         \
    {}                                               \
    CLS(const CLS& exc) : BASE(exc)                  \
    {}                                               \
    ~CLS() throw()                                   \
    {}                                               \
    const char* name() const throw()                 \
    {                                                \
        return NAME;                                 \
    }                                                \
};

IMPL_EXCEPTION(RuntimeException, Exception, "Runtime exception")
IMPL_EXCEPTION(SystemException,  RuntimeException, "System exception")

IMPL_EXCEPTION(SocketException,  SystemException, "System exception")





#endif /* EXCEPTION_H_ */
