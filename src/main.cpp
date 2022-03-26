/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:14:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 21:32:59 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "WorkManager.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdio>

// https://codereview.stackexchange.com/questions/201640/a-thread-safe-task-queue-implementation-using-my-own-lock-guard-in-c


class MyTask : public Task
{
public:
    MyTask(Mutex& mutex) : _mutex(mutex) {}
    ~MyTask() {}

    void run()
    {
        sleep(1);
        LockGuard lock(_mutex);
        printf("Task %d is running\n", this->getId());
    }

private:
    // Non-copyable
    MyTask(MyTask const &rhs) : _mutex(rhs._mutex) { (void)rhs; };
    MyTask &operator=(MyTask const &rhs) { (void)rhs; return *this; };

    // Private attributes
    Mutex& _mutex;
};

int main()
{
    std::cout << "Hello World!" << std::endl;
    WorkManager manager(5);
    Mutex mutex;
    for (size_t i = 0; i < 50; ++i)
        manager.push_task(new MyTask(i, mutex));
    manager.start();
    return 0;
}
