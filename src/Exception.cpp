/*
 * Exception.cpp
 *
 *  Created on: 11 янв. 2015 г.
 *      Author: tarasov
 */

#include <Exception.h>


Exception::Exception(const std::string& msg): _msg(msg)
{
}




Exception::Exception(const Exception& exc):
    std::exception(exc),
    _msg(exc._msg)
{
}


Exception::~Exception() throw()
{
}


