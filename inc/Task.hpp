/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:20:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/11 01:46:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP
# define TASK_HPP

# include <pthread.h>
# include "Mutex.hpp"

class Task
{
public:
    virtual ~Task() {};
    virtual void run() = 0;
};

#endif
