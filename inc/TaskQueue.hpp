/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:57 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 15:30:50 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKQUEUE_HPP
# define TASKQUEUE_HPP

# include "Task.hpp"
# include "Mutex.hpp"
# include "ConditionVariable.hpp"
# include <pthread.h>
# include <queue>
# include <utility>

class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    void push(Task *task);
    Task *pop();
    bool isStopped();

private:
    // Non-copyable
    TaskQueue(TaskQueue const &rhs);
    TaskQueue &operator=(TaskQueue const &rhs);

    // Private attributes
    std::queue<Task *> _queue;
    ConditionVariable _cond;
    Mutex _mutex;
    bool _stopped;
};

#endif
