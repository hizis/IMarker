/*
 * ThreadPool.cpp
 *
 *  Created on: 08 янв. 2015 г.
 *      Author: tarasov
 */

#include <ThreadPool.h>



//---------------------------------------------------------
WorkQueue::WorkQueue()
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
WorkQueue::~WorkQueue()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}


//---------------------------------------------------------
Runnable * WorkQueue::nextTask()
{
    Runnable *nt = 0;
    pthread_mutex_lock(&m_mutex);

    while (m_tasks.empty())
    {
        pthread_cond_wait(&m_cond, &m_mutex);
    }

    nt = m_tasks.front();
    m_tasks.pop();

    pthread_mutex_unlock(&m_mutex);
    return nt;
}


//---------------------------------------------------------
void WorkQueue::addTask(Runnable *nt)
{
    pthread_mutex_lock(&m_mutex);
    m_tasks.push(nt);
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

//---------------------------------------------------------
PoolWorkerThread::PoolWorkerThread(WorkQueue& workQueue) :
        m_workQueue(workQueue)
{
}

//---------------------------------------------------------
void PoolWorkerThread::start()
{
    m_thread.start(*this);
}

//---------------------------------------------------------
void PoolWorkerThread::join()
{
    m_thread.join();
}

//---------------------------------------------------------
void PoolWorkerThread::run()
{
    while (Runnable* task = m_workQueue.nextTask())
    {
        task->run();
    }
}



//---------------------------------------------------------
ThreadPool::ThreadPool(std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        m_threads.push_back(new PoolWorkerThread(m_workQueue));
        m_threads.back()->start();
    }
}


//---------------------------------------------------------
ThreadPool::~ThreadPool()
{
    for (size_t i = 0, e = m_threads.size(); i < e; ++i)
    {
        m_workQueue.addTask(NULL);
    }

    for (size_t i = 0, e = m_threads.size(); i < e; ++i)
    {
        m_threads[i]->join();
        delete m_threads[i];
    }
    m_threads.clear();
}

//---------------------------------------------------------
void ThreadPool::addTask(Runnable *nt)
{
    m_workQueue.addTask(nt);
}

