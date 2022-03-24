/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:23:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 15:18:32 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "TaskQueue.hpp"
#include "WorkManager.hpp"
#include <iostream>

void* WorkerThread(void *ptr)
{
    TaskQueue* taskQueue = (TaskQueue*)ptr;

    while (true)
    {
        Task *task = taskQueue->pop();
        if (!task)
            break ;
        try 
        {
            task->run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        delete task;
    }
    return NULL;
}

WorkManager::WorkManager(int numWorkers) :
    _workers(new pthread_t[numWorkers]),
    _numWorkers(numWorkers),
    _running(false)
{
}

WorkManager::WorkManager(WorkManager const &rhs) :
    _numWorkers(rhs._numWorkers),
    _running(rhs._running)
{
    *this = rhs;
}

WorkManager::~WorkManager()
{
    this->wait();
    delete[] _workers;
}

WorkManager& WorkManager::operator=(WorkManager const &rhs)
{
    (void)rhs;
    return *this;
}

void WorkManager::wait()
{
    if (!_running)
        return ;
    for (int i = 0; i < _numWorkers; i++)
        pthread_join(_workers[i], NULL);
    _running = false;
}

void WorkManager::init()
{
    if (_running)
        return ;
    for (int i = 0; i < _numWorkers; i++)
        pthread_create(&_workers[i], NULL, WorkerThread, (void*)&_taskQueue);
    _running = true;
}

void WorkManager::push_task(Task* task)
{
    _taskQueue.push(task);
}

void WorkManager::reset()
{
    _taskQueue.reset();
}
