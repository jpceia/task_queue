/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 03:25:09 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <set>
# include <pthread.h>
# include "Mutex.hpp"


class Task
{
public:
    typedef std::set<Task *> Set;
    Task();
    virtual ~Task();
    virtual void run() = 0;

    void addDependency(Task *task);
    bool isReady() const;
    Set getDependents() const;
    unsigned int getId() const;

private:
    // Private attributes
    Set _dependencies;
    Set _dependents;
    Mutex _depMtx;
    const unsigned int _id;
    
    // static variable to count the number of tasks
    static unsigned int _taskCount;
};

#endif
