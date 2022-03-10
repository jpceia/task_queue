/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:14:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <pthread.h>
# include "Mutex.hpp"

class Task
{
public:
    Task(unsigned int id, Mutex& mutex);
    ~Task();

    void run();

private:
    // Non-copyable
    Task(Task const &rhs);
    Task &operator=(Task const &rhs);

    unsigned int _id;
    Mutex& _mutex;
};

#endif
