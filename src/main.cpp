/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:14:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 15:27:42 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "WorkManager.hpp"
#include <iostream>
#include <unistd.h>


class MyTask : public Task
{
public:
    MyTask(unsigned int id, Mutex& mutex) : _id(id), _mutex(mutex) {}
    ~MyTask() {}

    void run()
    {
        sleep(1);
        LockGuard lock(_mutex);
        std::cout << "MyTask " << _id << " is running" << std::endl;
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
    WorkManager manager(5);
    Mutex mutex;
    for (size_t i = 0; i < 50; ++i)
        manager.push_task(new MyTask(i, mutex));
    manager.push_task(NULL);
    manager.init();
    return 0;
}
