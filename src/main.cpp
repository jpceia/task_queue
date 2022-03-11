/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:14:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 15:23:16 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "TaskQueue.hpp"
#include <pthread.h>
#include <iostream>
#include <unistd.h>

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

class MyTask : public Task
{
public:
    MyTask(unsigned int id, Mutex& mutex) : _id(id), _mutex(mutex) {}
    ~MyTask() {}

    void run()
    {
        sleep(1);
        _mutex.lock();
        std::cout << "MyTask " << _id << " is running" << std::endl;
        _mutex.unlock();
    }

private:
    // Non-copyable
    MyTask(MyTask const &rhs) : _id(rhs._id), _mutex(rhs._mutex) { (void)rhs; };
    MyTask &operator=(MyTask const &rhs) { (void)rhs; return *this; };

    // Private attributes
    const unsigned int _id;
    Mutex& _mutex;
};

int main()
{
    std::cout << "Hello World!" << std::endl;
    TaskQueue taskQueue;
    std::vector<pthread_t> threads(5, 0);
    Mutex mutex;
    for (size_t i = 0; i < threads.size(); ++i)
        pthread_create(&threads[i], NULL, WorkerThread, &taskQueue);
    for (size_t i = 0; i < 50; ++i)
        taskQueue.push(new MyTask(i, mutex));
    taskQueue.stop();
    for (size_t i = 0; i < threads.size(); ++i)
        pthread_join(threads[i], NULL);
    return 0;
}
