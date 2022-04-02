/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SafeQueue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:57 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 05:27:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAFEQUEUE_HPP
# define SAFEQUEUE_HPP

# include "Atomic.hpp"
# include "Mutex.hpp"
# include "ConditionVariable.hpp"
# include <queue>
# include <vector>

namespace wm
{
    
template <typename T>
class SafeQueue
{
public:
    SafeQueue() {};
    virtual ~SafeQueue() {};

    void push(T *item);
    void push(const std::vector<T*>& items);
    T *pop();
    T *pop(const Atomic<bool>& wait);
    bool empty() const;
    void clear();

private:
    // Non-copyable
    SafeQueue(SafeQueue const &rhs) { (void)rhs; };
    SafeQueue &operator=(SafeQueue const &rhs) { (void)rhs; return *this; };

    // Private attributes
    std::queue<T*> _queue;
    ConditionVariable _cv;
    mutable Mutex _pushMtx;
    mutable Mutex _popMtx;
};


template <typename T>
void SafeQueue<T>::push(T *item)
{
    if (item != NULL)
    {
        LockGuard lock(_pushMtx);
        _queue.push(item);
        _cv.signal();
    }
}

template <typename T>
void SafeQueue<T>::push(const std::vector<T*>& items)
{
    if (items.empty())
        return ;

    LockGuard lock(_pushMtx);

    for (typename std::vector<T*>::const_iterator it = items.begin();
        it != items.end(); ++it)
    {
        if (*it != NULL)
            _queue.push(*it);
    }
    _cv.broadcast();
}

template <typename T>
T *SafeQueue<T>::pop()
{
    bool wait = false;
    return pop(wait);
}

template <typename T>
T *SafeQueue<T>::pop(const Atomic<bool>& wait)
{
    LockGuard lock(_popMtx);
    while (this->empty() && wait)
        _cv.wait(_popMtx);
    if (this->empty())
        return NULL;
    LockGuard lockEmpty(_pushMtx);
    T *item = _queue.front();
    _queue.pop();
    return item;
}

template <typename T>
bool SafeQueue<T>::empty() const
{
    LockGuard lock(_pushMtx);
    return _queue.empty();
}

template <typename T>
void SafeQueue<T>::clear()
{
    while (true)
    {
        T *item = this->pop();
        if (item == NULL)
            break ;
        delete item;
    }
}

} // namespace wm

#endif
