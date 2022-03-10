/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:14:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:12:15 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "TaskQueue.hpp"
#include <pthread.h>
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

int main()
{
    std::cout << "Hello World!" << std::endl;
    TaskQueue taskQueue;
    std::vector<pthread_t> threads(5, 0);
    Mutex mutex;
    for (size_t i = 0; i < threads.size(); ++i)
        pthread_create(&threads[i], NULL, WorkerThread, &taskQueue);
    for (size_t i = 0; i < 50; ++i)
        taskQueue.push(new Task(i, mutex));
    taskQueue.stop();
    for (size_t i = 0; i < threads.size(); ++i)
        pthread_join(threads[i], NULL);
    return 0;
}
