/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 06:35:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/04/04 09:01:26 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_HPP
# define NODE_HPP

# include "safe/Set.hpp"
# include <iostream>

namespace wm
{

template <typename T>
class Node
{
public:
    Node() {};
    virtual ~Node();

    bool isReady() const { return _dependencies.empty(); };
    static bool isReady(T *node) { return node->isReady(); };
    SafeSet<T> getDependents() const { return _dependents; };

    void addDependency(Node *node);

protected:
    // Private attributes
    SafeSet<Node> _dependencies;
    SafeSet<Node> _dependents;
};

template <typename T>
Node<T>::~Node()
{
    if (!_dependencies.empty())
        std::cerr << "Node is being deleted while still having dependencies" << std::endl;
    for (typename SafeSet<Node>::const_iterator it = _dependents.begin();
        it != _dependents.end(); ++it)
    {
        Node *node = *it;
        node->_dependencies.erase(this);
    }
}

template <typename T>
void Node<T>::addDependency(Node *node)
{
    if (node == this)
        std::cerr << "Node::addDependency: Node cannot depend on itself" << std::endl;
    else if (static_cast<T*>(this)->isLocked())
        std::cerr << "Node::addDependency: Cannot add dependency to locked node"
            << std::endl;
    else if (_dependencies.insert(node))
        node->_dependents.insert(this);
    else
        std::cerr << "Node::addDependency: dependency already exists" << std::endl;
}

} // namespace wm

#endif
