/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mutex.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 22:57:44 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 01:03:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTEX_HPP
# define MUTEX_HPP

#include <pthread.h>

class Mutex
{
public:
    Mutex() { pthread_mutex_init(&_mutex, NULL); }
    ~Mutex() { pthread_mutex_destroy(&_mutex); }
    void lock() { pthread_mutex_lock(&_mutex); }
    void unlock() { pthread_mutex_unlock(&_mutex); }
private:
    friend class LockGuard;
    friend class ConditionVariable;

    // Non-copyable
    Mutex(Mutex const &rhs) : _mutex(rhs._mutex) {};
    Mutex &operator=(Mutex const &rhs) { (void)rhs; return *this; }

    // Private attributes
    pthread_mutex_t _mutex;
};

#endif
