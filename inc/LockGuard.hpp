/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LockGuard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 22:26:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/29 04:22:57 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCKGUARD_HPP
# define LOCKGUARD_HPP

#include <pthread.h>
#include "Mutex.hpp"

class LockGuard
{
public:
    LockGuard(Mutex& mutex) : _mutex(mutex) { _mutex.lock(); }
    ~LockGuard() { _mutex.unlock(); }

private:
    // Non-copyable
    LockGuard(const LockGuard &rhs) : _mutex(rhs._mutex) {};
    LockGuard &operator=(const LockGuard &rhs) { (void)rhs; return *this; }

    // Private attributes
    Mutex& _mutex;
};

#endif
