/*
 * Noncopyable.h
 *
 *  Created on: 08 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

class Noncopyable
{
protected:
    Noncopyable()
    {
    }
    ~Noncopyable()
    {
    }
private:
    Noncopyable(const Noncopyable&);
    Noncopyable& operator=(const Noncopyable&);
};

#endif /* NONCOPYABLE_H_ */
