/*
 * Runnable.h
 *
 *  Created on: 07 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_


class Runnable
{
public:
    Runnable();
    virtual ~Runnable();
    virtual void run() = 0;
};

#endif /* RUNNABLE_H_ */
