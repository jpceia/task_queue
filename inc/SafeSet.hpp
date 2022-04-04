/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SafeSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/04 07:18:15 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAFESET_HPP
# define SAFESET_HPP

# include "Mutex.hpp"
# include "LockGuard.hpp"
# include <set>
# include <vector>

namespace wm
{

template <typename T>
class SafeSet
{
public:
    typedef typename std::set<T*>::iterator                 iterator;
    typedef typename std::set<T*>::const_iterator           const_iterator;
    typedef typename std::set<T*>::reverse_iterator         reverse_iterator;
    typedef typename std::set<T*>::const_reverse_iterator   const_reverse_iterator;

    SafeSet() {};
    template <typename InputIterator>
    SafeSet(InputIterator first, InputIterator last) : _set(first, last) {};
    SafeSet(const SafeSet& rhs) : _set(rhs._set) {};
    virtual ~SafeSet() {};
    SafeSet& operator=(const SafeSet& rhs) { _set = rhs._set; return *this; };

    iterator begin() { return _set.begin(); };
    iterator end() { return _set.end(); };
    const_iterator begin() const { return _set.begin(); };
    const_iterator end() const { return _set.end(); };
    reverse_iterator rbegin() { return _set.rbegin(); };
    reverse_iterator rend() { return _set.rend(); };
    const_reverse_iterator rbegin() const { return _set.rbegin(); };
    const_reverse_iterator rend() const { return _set.rend(); };

    template<typename U>
    operator SafeSet<U>() const;

    template <typename U>
    SafeSet filter(bool (*predicate)(U*));

    template <typename U>
    SafeSet filter_and_remove(bool (*predicate)(U*));

    template <typename U>
    void apply(void (*func)(U*));

    iterator find(T *item) const;

    void erase(iterator position);
    size_t erase(T *item);
    
    bool insert(T *item);

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last);

    bool empty() const;
    size_t size() const;
    void clear();

private:
    mutable Mutex _mutex;
    std::set<T*> _set;
};

template <typename T>
template <typename U>
SafeSet<T>::operator SafeSet<U>() const
{
    return SafeSet<U>(this->begin(), this->end());
}

template <typename T>
typename SafeSet<T>::iterator SafeSet<T>::find(T *item) const
{
    LockGuard lock(_mutex);
    return _set.find(item);
}

template <typename T>
void SafeSet<T>::erase(iterator position)
{
    LockGuard lock(_mutex);
    _set.erase(position);
}

template <typename T>
size_t SafeSet<T>::erase(T *item)
{
    LockGuard lock(_mutex);
    return _set.erase(item);
}

template <typename T>
bool SafeSet<T>::insert(T *item)
{
    LockGuard lock(_mutex);
    return _set.insert(item).second;
}

template <typename T>
template <typename InputIterator>
void SafeSet<T>::insert(InputIterator first, InputIterator last)
{
    LockGuard lock(_mutex);
    _set.insert(first, last);
}

template <typename T>
bool SafeSet<T>::empty() const
{
    LockGuard lock(_mutex);
    return _set.empty();
}

template <typename T>
size_t SafeSet<T>::size() const
{
    LockGuard lock(_mutex);
    return _set.size();
}

template <typename T>
void SafeSet<T>::clear()
{
    LockGuard lock(_mutex);
    while (!_set.empty())
        delete *_set.begin();
    _set.clear();
}

template <typename T>
template <typename U>
SafeSet<T> SafeSet<T>::filter(bool (*predicate)(U*))
{
    SafeSet<T> filtered;

    LockGuard lock(_mutex);
    for (iterator it = _set.begin();
        it != _set.end(); ++it)
        if (predicate(dynamic_cast<U*>(*it)))
            filtered._set.insert(*it);
    return filtered;
}

template <typename T>
template <typename U>
SafeSet<T> SafeSet<T>::filter_and_remove(bool (*predicate)(U*))
{
    SafeSet<T> filtered;

    LockGuard lock(_mutex);
    for (iterator it = _set.begin(); it != _set.end(); )
    {
        if (predicate(dynamic_cast<U*>(*it)))
        {
            filtered._set.insert(*it);
            _set.erase(it++);
        }
        else
            ++it;
    }
    return filtered;
}

template <typename T>
template <typename U>
void SafeSet<T>::apply(void (*func)(U*))
{
    LockGuard lock(_mutex);
    for (typename std::set<T*>::iterator it = _set.begin(); it != _set.end(); ++it)
        func(dynamic_cast<U*>(*it));
}

} // namespace wm

#endif
