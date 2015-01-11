/*
 * Thread.cpp
 *
 *  Created on: 07 янв. 2015 г.
 *      Author: tarasov
 */

#include "Thread.h"
#include <sys/time.h>
#include <signal.h>
#include <iostream>


//---------------------------------------------------------
Thread::Thread() :
        m_pTData(new TData)
{
}

//---------------------------------------------------------
Thread::~Thread()
{
    if (m_pTData->started && !m_pTData->joined)
    {
        pthread_detach(m_pTData->thread);
    }
}

//---------------------------------------------------------
void Thread::start(Runnable & pTarget)
{

    if (!m_pTData->pRunnableTarget.empty())
    {
        throw SystemException("thread already running");
    }

    pthread_attr_t attributes;
    pthread_attr_init(&attributes);

    m_pTData->pRunnableTarget.reset(&pTarget);  //    _pData->pRunnableTarget = pTarget;

    if (pthread_create(&m_pTData->thread, &attributes, runnableEntry, this))
    {
        m_pTData->pRunnableTarget.reset();  //_pData->pRunnableTarget = 0;
        pthread_attr_destroy(&attributes);
        throw "SystemException(\"cannot start thread\");";
    }

    m_pTData->started = true;
    pthread_attr_destroy(&attributes);
}

//---------------------------------------------------------
void Thread::join()
{
    if (!m_pTData->started)
    {
        return;
    }
    m_pTData->wait();  //_pData->done.wait();
    void* result;
    if (pthread_join(m_pTData->thread, &result))
    {
        throw SystemException("cannot join thread");
    }
    m_pTData->joined = true;
}

//---------------------------------------------------------
void* Thread::runnableEntry(void* pThread)
{
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGQUIT);
    sigaddset(&sset, SIGTERM);
    sigaddset(&sset, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &sset, 0);

    Thread* pThreadImpl = reinterpret_cast<Thread*>(pThread);
    std::auto_ptr<TData> pData = pThreadImpl->m_pTData;
    try
    {
        pData->pRunnableTarget->run();
    }
//    catch (Exception& exc)
//    {
//        //$$$%%%ErrorHandler::handle(exc);
//    }
//    catch (std::exception& exc)
//    {
//        //$$$%%%ErrorHandler::handle(exc);
//    }
    catch (...)
    {
        //$$$%%%ErrorHandler::handle();
    }

    pData->pRunnableTarget.reset();  //pData->pRunnableTarget = 0;
    pData->setDone();  //pData->done.set();
    return 0;
}

//---------------------------------------------------------
Thread::TData::TData() :
        thread(0), started(false), joined(false), pRunnableTarget(0), m_done(false)
{
    if (pthread_mutex_init(&m_mutex, 0))
    {
        throw SystemException("cannot create event (mutex)");
    }
    if (pthread_cond_init(&m_cond, 0))
    {
        throw SystemException("cannot create event (condition)");
    }
}

//---------------------------------------------------------
Thread::TData::~TData()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

//---------------------------------------------------------
void Thread::TData::setDone()
{
    if (pthread_mutex_lock(&m_mutex))
    {
        throw SystemException("cannot signal event (lock)");
    }
    m_done = true;
    if (pthread_cond_broadcast(&m_cond))
    {
        pthread_mutex_unlock(&m_mutex);
        throw SystemException("cannot signal event");
    }
    pthread_mutex_unlock(&m_mutex);
}

//---------------------------------------------------------
void Thread::TData::wait()
{
    if (pthread_mutex_lock(&m_mutex))
    {
        throw SystemException("wait for event failed (lock)");
    }

    while (!m_done)
    {
        if (pthread_cond_wait(&m_cond, &m_mutex))
        {
            pthread_mutex_unlock(&m_mutex);
            throw SystemException("wait for event failed");
        }
    }
    pthread_mutex_unlock(&m_mutex);
}
