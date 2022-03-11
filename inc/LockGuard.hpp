/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LockGuard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 22:26:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 01:03:47 by jpceia           ###   ########.fr       */
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
    LockGuard(LockGuard const &rhs) : _mutex(rhs._mutex) {};
    LockGuard &operator=(LockGuard const &rhs) { (void)rhs; return *this; }

    // Private attributes
    Mutex& _mutex;
};

#endif
