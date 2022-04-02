/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:57 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 04:37:43 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKQUEUE_HPP
# define TASKQUEUE_HPP

# include "Atomic.hpp"
# include "Mutex.hpp"
# include "ConditionVariable.hpp"
# include <queue>
# include <vector>

namespace wm
{

class Task;

class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    void push(Task *task);
    void push(const std::vector<Task *>& tasks);
    Task *pop();
    Task *pop(const Atomic<bool>& wait);
    bool empty() const;
    void clear();

private:
    // Non-copyable
    TaskQueue(TaskQueue const &rhs);
    TaskQueue &operator=(TaskQueue const &rhs);

    // Private attributes
    std::queue<Task *> _queue;
    ConditionVariable _cv;
    mutable Mutex _pushMtx;
    mutable Mutex _popMtx;
};

} // namespace wm

#endif
