/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorkManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 23:23:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 03:25:55 by jpceia           ###   ########.fr       */
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
    bool& wait = wm->_acceptingWork;

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
        wm->_finish_task(task);
    }
    return NULL;
}

WorkManager::WorkManager(int numWorkers) :
    _workers(numWorkers),
    _working(false),
    _acceptingWork(true)
{
}

WorkManager::WorkManager(WorkManager const &rhs) :
    _workers(rhs._workers),
    _working(rhs._working),
    _acceptingWork(rhs._acceptingWork)
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
    _acceptingWork = false;
    for (std::vector<pthread_t>::const_iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_join(*it, NULL);
    _working = false;
}

void WorkManager::start()
{
    if (_working)
    {
        std::cerr << "WorkManager::start() called while already started" << std::endl;
        return ;
    }
    // Check which tasks are ready to pass to taskQueue (no dependencies)
    _taskQueue.push(_lockedTasks.moveUnlockedTasks());
    
    // Start workers
    for (std::vector<pthread_t>::iterator it = _workers.begin();
        it != _workers.end(); ++it)
        pthread_create(&*it, NULL, WorkerThread, (void*)this);
    _working = true;
}

void WorkManager::pushTask(Task *task)
{
    if (!_acceptingWork)
    {
        std::cerr << "WorkManager::push_task() called while not accepting work" << std::endl;
        return ;
    }
    if (task == NULL)
    {
        std::cerr << "WorkManager::push_task() called with NULL task" << std::endl;
        return ;
    }
    _lockedTasks.insert(task);
}

void WorkManager::pushTask(const std::vector<Task *>& tasks)
{
    if (!_acceptingWork)
    {
        std::cerr << "WorkManager::push_task() called while not accepting work" << std::endl;
        return ;
    }
    for (std::vector<Task *>::const_iterator it = tasks.begin();
        it != tasks.end(); ++it)
    {
        if (*it == NULL)
        {
            std::cerr << "WorkManager::push_task() called with NULL task" << std::endl;
            return ;
        }
        _lockedTasks.insert(*it);
    }
}

void WorkManager::_finish_task(Task *task)
{
    if (task == NULL)
    {
        std::cerr << "WorkManager::_finish_task() called with NULL task" << std::endl;
        return ;
    }
    // Get dependent tasks
    Task::Set dependentTasks = task->getDependents();
    delete task;
    // Check which tasks are ready to pass to taskQueue (no dependencies)
    for (Task::Set::iterator it = dependentTasks.begin();
        it != dependentTasks.end(); ++it)
    {
        if ((*it)->isLocked())
            continue ;
        #ifdef DEBUG
        std::cout << "Task " << (*it)->getId() << " is ready to pass to taskQueue" << std::endl;
        #endif
        if (_lockedTasks.erase(*it) == 0)
            std::cerr << "Error: task not found in lockedTasks" << std::endl;
        #ifdef DEBUG
        std::cout << "Task " << (*it)->getId() << " is removed from lockedTasks" << std::endl;
        #endif
        _taskQueue.push(*it);
        #ifdef DEBUG
        std::cout << "Task " << (*it)->getId() << " passed to taskQueue" << std::endl;
        #endif
    }
}
