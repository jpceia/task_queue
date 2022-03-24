/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:23:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 15:38:58 by jpceia           ###   ########.fr       */
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
    _workers(numWorkers),
    _running(false)
{
}

WorkManager::WorkManager(WorkManager const &rhs) :
    _workers(rhs._workers),
    _running(rhs._running)
{
    *this = rhs;
}

WorkManager::~WorkManager()
{
    this->wait();
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
    for (std::vector<pthread_t>::const_iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_join(*it, NULL);
    _running = false;
}

void WorkManager::init()
{
    if (_running)
        return ;
    for (std::vector<pthread_t>::iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_create(&*it, NULL, WorkerThread, (void*)&_taskQueue);
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
