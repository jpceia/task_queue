/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Atomic.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 04:07:33 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/29 04:51:24 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATOMIC_HPP
# define ATOMIC_HPP

# include "Mutex.hpp"
# include "LockGuard.hpp"

template <typename T>
class Atomic
{
public:
    Atomic(const T& data) : _data(data) {};
    Atomic(const Atomic& rhs) : _data(rhs.get()) {};
    ~Atomic() {};

    Atomic& operator=(const Atomic& rhs)
    {
        if (this != &rhs)
            this->set(rhs.get());
        return *this;
    };

    Atomic& operator=(const T& data)
    {
        _data = data;
        return *this;
    };
    
    T get() const
    {
        LockGuard lock(_mutex);
        return _data;
    };

private:
    T _data;
    mutable Mutex _mutex;
};

#endif
