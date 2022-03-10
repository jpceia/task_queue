/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskQueue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:57 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/10 23:06:55 by jpceia           ###   ########.fr       */
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
    void stop();
    bool isStopped();

private:
    // Non-copyable
    TaskQueue(TaskQueue const &rhs);
    TaskQueue &operator=(TaskQueue const &rhs);

    std::queue<Task *> _queue;
    Mutex _mutex;
    ConditionVariable _cond;
    bool _stopped;
};

#endif
