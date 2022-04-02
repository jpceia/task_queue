/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTask.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 07:57:17 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/02 04:38:41 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseTask.hpp"

namespace wm
{

unsigned int BaseTask::_taskCount = 0;

BaseTask::BaseTask() :
    _id(_taskCount++)
{
}

BaseTask::~BaseTask()
{
}

unsigned int BaseTask::getId() const
{
    return _id;
}

} // namespace wm
