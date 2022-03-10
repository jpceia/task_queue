/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConditionVariable.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 22:59:47 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:04:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONDITIONVARIABLE_HPP
# define CONDITIONVARIABLE_HPP

# include <pthread.h>
# include "Mutex.hpp"

class ConditionVariable
{
public:
    ConditionVariable() { pthread_cond_init(&_cond, NULL); }
    ~ConditionVariable() { pthread_cond_destroy(&_cond); }
    void wait(Mutex &mutex) { pthread_cond_wait(&_cond, &mutex._mutex); }
    void signal() { pthread_cond_signal(&_cond); }
    void broadcast() { pthread_cond_broadcast(&_cond); }

private:
    // Non-copyable
    ConditionVariable(ConditionVariable const &rhs) : _cond(rhs._cond) {};
    ConditionVariable &operator=(ConditionVariable const &rhs) { (void)rhs; return *this; }

    pthread_cond_t _cond;
};

#endif
