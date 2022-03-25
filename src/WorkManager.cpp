/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:23:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 21:11:10 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "TaskQueue.hpp"
#include "WorkManager.hpp"
#include <iostream>

void* WorkManager::WorkerThread(void *ptr)
{
    WorkManager* wm = (WorkManager *)ptr;
    TaskQueue& taskQueue = wm->_taskQueue;
    bool& wait = wm->_accepting_work;

    while (true)
    {
        Task *task = taskQueue.pop(wait);
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
    _working(false),
    _accepting_work(true)
{
}

WorkManager::WorkManager(WorkManager const &rhs) :
    _workers(rhs._workers),
    _working(rhs._working),
    _accepting_work(rhs._accepting_work)
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
    if (!_working)
        return ;
    _accepting_work = false;
    for (std::vector<pthread_t>::const_iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_join(*it, NULL);
    _working = false;
}

void WorkManager::start()
{
    if (_working) // already working
        return ;
    for (std::vector<pthread_t>::iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_create(&*it, NULL, WorkerThread, (void*)this);
    _working = true;
}

void WorkManager::push_task(Task* task)
{
    _taskQueue.push(task);
}
