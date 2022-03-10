/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:41 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:11:56 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"
#include "LockGuard.hpp"
#include <pthread.h>
#include <iostream>
#include <unistd.h> // sleep

Task::Task(unsigned int id, Mutex& mutex) :
    _id(id),
    _mutex(mutex)
{
}

Task::Task(Task const &rhs) :
    _id(rhs._id),
    _mutex(rhs._mutex)
{
}

Task::~Task()
{
}

Task& Task::operator=(Task const &rhs)
{
    (void)rhs;
    return *this;
}

void Task::run()
{
    sleep(1);
    {
        LockGuard lock(_mutex);
        std::cout << "Task " << _id << " is running" << std::endl;
    }
}
