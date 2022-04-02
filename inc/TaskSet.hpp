/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:42:45 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 04:41:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKSET_HPP
# define TASKSET_HPP

# include "Mutex.hpp"
# include <set>
# include <vector>

namespace wm
{

class Task;

class TaskSet
{
public:
    typedef std::set<Task *>::iterator                  iterator;
    typedef std::set<Task *>::const_iterator            const_iterator;
    typedef std::set<Task *>::reverse_iterator          reverse_iterator;
    typedef std::set<Task *>::const_reverse_iterator    const_reverse_iterator;

    TaskSet();
    TaskSet(const TaskSet& rhs);
    virtual ~TaskSet();
    TaskSet& operator=(const TaskSet& rhs);

    iterator begin() { return _set.begin(); };
    iterator end() { return _set.end(); };
    const_iterator begin() const { return _set.begin(); };
    const_iterator end() const { return _set.end(); };
    reverse_iterator rbegin() { return _set.rbegin(); };
    reverse_iterator rend() { return _set.rend(); };
    const_reverse_iterator rbegin() const { return _set.rbegin(); };
    const_reverse_iterator rend() const { return _set.rend(); };

    std::vector<Task*> moveUnlockedTasks();
    size_t erase(Task *task);
    bool insert(Task *task);
    bool empty() const;
    void clear();

private:
    mutable Mutex _mutex;
    std::set<Task *> _set;
};

} // namespace wm

#endif
