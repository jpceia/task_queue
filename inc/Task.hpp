/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 18:40:25 by jpceia           ###   ########.fr       */
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

    void add_dependency(Task *task);
    bool has_dependencies() const;
    Set getDependents() const;

private:
    void _add_dependent(Task *task);
    void _remove_dependency(Task* task);

    // Private attributes
    Set _dependencies;
    Set _dependents;
    Mutex _mutex; // for dependencies
};

#endif
