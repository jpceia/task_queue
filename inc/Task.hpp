/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 06:28:49 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <vector>
# include "TaskSet.hpp"
# include "SafeQueue.hpp"
# include "BaseTask.hpp"

namespace wm
{

class Task : public BaseTask
{
public:
    Task();
    virtual ~Task();

    bool isReady() const;
    static bool isReady(Task *task);

    bool isLocked() const;
    static bool isLocked(Task *task);

    void lock();
    static void lock(Task *task);

    TaskSet getDependents() const;

    void addDependency(Task *task);
    void moveDeps(TaskSet& taskSet, SafeQueue<Task>& taskQueue);

private:
    // Helper methods
    bool _moveIfReady(TaskSet& taskSet, SafeQueue<Task>& taskQueue);

    // Private attributes
    TaskSet _dependencies;
    TaskSet _dependents;
    bool _locked;
};

} // namespace wm

#endif
