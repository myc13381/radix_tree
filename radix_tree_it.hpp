// #ifndef RADIX_TREE_IT
// #define RADIX_TREE_IT

// #include <iterator>
// #include <functional>

// // forward declaration
// template <typename K, typename T, class Compare = std::less<K> > class radix_tree;
// template <typename K, typename T, class Compare = std::less<K> > class radix_tree_node;

// template <typename K, typename T, class Compare = std::less<K> >
// class radix_tree_it : public  std::iterator<std::forward_iterator_tag, std::pair<K, T>> {
//     friend class radix_tree<K, T, Compare>;

// public:
//     radix_tree_it() : m_pointee(0) { }
//     radix_tree_it(const radix_tree_it& r) : m_pointee(r.m_pointee) { } // 复制构造函数
//     radix_tree_it& operator=(const radix_tree_it& r) 
//     {
//         if(this != &r)
//         {
//             m_pointee = r.m_pointee; 
//         }
//         return *this; 
//     }
//     ~radix_tree_it() { }

//     std::pair<const K, T>& operator*  () const;
//     std::pair<const K, T>* operator-> () const;
//     const radix_tree_it<K, T, Compare>& operator++ ();
//     radix_tree_it<K, T, Compare> operator++ (int);
//     // const radix_tree_it<K, T, Compare>& operator-- ();
//     bool operator!= (const radix_tree_it<K, T, Compare> &lhs) const;
//     bool operator== (const radix_tree_it<K, T, Compare> &lhs) const;

// private:
//     radix_tree_node<K, T, Compare> *m_pointee; // 指向节点的指针，核心成员变量
//     radix_tree_it(radix_tree_node<K, T, Compare> *p) : m_pointee(p) { }

//     radix_tree_node<K, T, Compare>* increment(radix_tree_node<K, T, Compare>* node) const; // 移动到上一个
//     radix_tree_node<K, T, Compare>* descend(radix_tree_node<K, T, Compare>* node) const; // 一直向下找到第一个叶子节点
// };

// // 这个函数试图找到给定节点node在基数树中的下一个节点。
// // 它首先检查该节点是否有父节点，如果没有（即该节点是树的根节点），则返回NULL。
// // 如果有父节点，它就在父节点的子节点集合中查找node，并尝试找到node之后的下一个子节点。
// //  -->  如果找到了下一个子节点，就通过调用descend函数下降到那个子节点；
// //  -->  如果没有找到（即node是其父节点的最后一个子节点），则递归地对父节点调用increment函数。

// template <typename K, typename T, typename Compare>
// radix_tree_node<K, T, Compare>* radix_tree_it<K, T, Compare>::increment(radix_tree_node<K, T, Compare>* node) const
// {
// 	radix_tree_node<K, T, Compare>* parent = node->m_parent;

//     if (parent == NULL)
//         return NULL;

//     typename radix_tree_node<K, T, Compare>::it_child it = parent->m_children.find(node->m_key);
//     assert(it != parent->m_children.end()); // 如果找不到，直接终止，因为这个节点不存在
//     ++it; // 我们的目的是找到下一个节点，所以 ++it

//     if (it == parent->m_children.end())
//         return increment(parent);
//     else
//         return descend(it->second);
// }

// // 从给定的节点node开始，下降到其第一个子节点（如果node不是叶子节点）。
// // 如果node是叶子节点，则直接返回node。
// // 否则，它获取node的第一个子节点，并递归地对那个子节点调用descend函数。
// template <typename K, typename T, typename Compare>
// radix_tree_node<K, T, Compare>* radix_tree_it<K, T, Compare>::descend(radix_tree_node<K, T, Compare>* node) const
// {
//     if (node->m_is_leaf)
//         return node;

//     typename radix_tree_node<K, T, Compare>::it_child it = node->m_children.begin();

//     assert(it != node->m_children.end());

//     return descend(it->second);
// }

// template <typename K, typename T, typename Compare>
// std::pair<const K, T>& radix_tree_it<K, T, Compare>::operator* () const
// {
//     return *m_pointee->m_value;
// }

// template <typename K, typename T, typename Compare>
// std::pair<const K, T>* radix_tree_it<K, T, Compare>::operator-> () const
// {
//     return m_pointee->m_value;
// }

// template <typename K, typename T, typename Compare>
// bool radix_tree_it<K, T, Compare>::operator!= (const radix_tree_it<K, T, Compare> &lhs) const
// {
//     return m_pointee != lhs.m_pointee; // 指针不相同
// }

// template <typename K, typename T, typename Compare>
// bool radix_tree_it<K, T, Compare>::operator== (const radix_tree_it<K, T, Compare> &lhs) const
// {
//     return m_pointee == lhs.m_pointee;
// }

// template <typename K, typename T, typename Compare>
// const radix_tree_it<K, T, Compare>& radix_tree_it<K, T, Compare>::operator++ ()
// {
//     if (m_pointee != NULL) // it is undefined behaviour to dereference iterator that is out of bounds...
//         m_pointee = increment(m_pointee);
//     return *this;
// }

// template <typename K, typename T, typename Compare>
// radix_tree_it<K, T, Compare> radix_tree_it<K, T, Compare>::operator++ (int)
// {
//     radix_tree_it<K, T, Compare> copy(*this);
//     ++(*this);
//     return copy;
// }

// /*
// template <typename K, typename T>
// const radix_tree_it<K, T, Compare>& radix_tree_it<K, T, Compare>::operator-- ()
// {
//     assert(m_pointee != NULL);

//     return *this;
// }
// */

// #endif // RADIX_TREE_IT
