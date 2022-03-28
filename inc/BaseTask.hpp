/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTask.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 07:56:43 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/28 04:20:56 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASETASK_HPP
# define BASETASK_HPP

class BaseTask
{
public:
    BaseTask();
    virtual ~BaseTask();
    virtual void run() = 0;

    unsigned int getId() const;

private:

    // Private attributes
    const unsigned int _id;
    
    // static variable to count the number of tasks
    static unsigned int _taskCount;
};

#endif
