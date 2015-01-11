/*
 * ThreadPool.h
 *
 *  Created on: 08 янв. 2015 г.
 *      Author: tarasov
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Runnable.h"
#include "Thread.h"
#include "Noncopyable.h"


#include <queue>
#include <pthread.h>




/**
 * @brief Wrapper around std::queue with some mutex protection
 */
class WorkQueue : private Noncopyable
{
public:
    WorkQueue();
    ~WorkQueue();
    Runnable *nextTask();
    void addTask(Runnable *nt);
private:
    std::queue<Runnable*> m_tasks;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};


/**
 *
 */
class PoolWorkerThread : public Runnable
{
public:
    PoolWorkerThread(WorkQueue& workQueue) ;
    void start();
    void join();
private:
    void run();
    WorkQueue & m_workQueue;
    Thread m_thread;
};


/**
 * @brief ThreadPool implementation
 */
class ThreadPool : private Noncopyable
{
public:
    ThreadPool(std::size_t n);
    ~ThreadPool();
    void addTask(Runnable *nt);
private:
    std::vector<PoolWorkerThread*> m_threads;
    WorkQueue m_workQueue;
};

#endif /* THREADPOOL_H_ */
