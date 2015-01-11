/*
 * Thread.h
 *
 *  Created on: 07 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Runnable.h"
#include "SharedPtr.h"
#include "Noncopyable.h"
#include "Exception.h"

#include <memory>
#include <pthread.h>
#include <errno.h>


class Thread : private Noncopyable
{
public:
    Thread();
    virtual ~Thread();
    void start(Runnable & target);
    void join();
private:
    static void* runnableEntry(void* pThread);

    struct TData
    {
        TData();
        ~TData();
        void setDone();
        void wait();

        pthread_t     thread;
        bool          started;
        bool          joined;
        SharedPtr<Runnable> pRunnableTarget;
    private:
        bool   m_done;
        pthread_mutex_t m_mutex;
        pthread_cond_t  m_cond;
    };


    std::auto_ptr<TData> m_pTData;
};

#endif /* THREAD_H_ */
