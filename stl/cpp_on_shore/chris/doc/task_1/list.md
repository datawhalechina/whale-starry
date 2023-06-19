# This is the first class.
## list
1. stl中的list是双向的, 在定义的时候需要包括prev节点与next
2. stl list 建立了节点基类_List_node_base 是一个struct而没有使用class
3. _List_node_header头节点是_List_node_base的子类
4. 普通的node是一个模板结构体(或者称为模板类), 方便使用者在使用的时候可以做扩展，使用不同数据类型的list
5. 同时还定义了list的iterator，便于使用者做迭代，
6. iterator实际上是个指针，需要解引用获取到其指向的值
//iterator设计解引用时，必须使用static_cast强制类型转换为_List_Node*
7. SGI list 不仅是一个双向链表 还是一个环形链表，因此只需要1个指针就可以完整遍历整个链表
8. 关于list的内存管理, 缺省使用了alloc作为空间配置器, 并且根据此另外定义了一个
list_node_allocator, 方便以每个节点大小而不是基本数据类型大小为配置单位
e.g. template<class T, class Alloc = alloc> // 缺省使用alloc为配置器
源码中使用了 typedef simple_alloc<list_node, Alloc> list_node_allocator
后续就可以使用list_node_allocator来分配节点内存 便于使用
9. 有了上面的基础之后 list_node_allocator(n)就可以轻松的为n个节点分配内存
10. 配置一个节点
linktype get_node() {return list_node_allocator::allocate();}
11. 释放一个节点
linktype put_node(link_type p) {list_node_allocator::deallocate();}
12. 配置并构造一个节点, 带有元素值
linktype create_node(const T& x) {
    linktype p = get_node();
    construct(&p->data, x); // 全局函数, 构造/析构基本工具
    return p;
}
13. 析构并释放一个节点
void destroy_node(link_type p){
    destroy(&p->data); // 全局函数, 构造和析构的基本工具
    put_node(p);
}
14. list中的push_back()利用了insert(end()), 是对于insert函数的重载
e.g. 
// 函数目的: 在迭代器position所指向位置插入一个节点, 内容为x
iterator insert(iterator position, const T& x) {
    link_type tmp = create_node(x); // 产生一个节点
    // 调整双向指针, 使tmp插入
    tmp->next = position.node;
    tmp->prev = position.node->prev;
    (link_type(position.node->prev))->next = tmp;
    position.node->prev = tmp;
    return tmp;
}
15. STL对于插入操作的基本规范: 插入完成后, 插入后的新节点位于哨兵迭代器(标识出插入点)所指节点的前方
16. 清除链表中的所有节点
template<class T, class Alloc>
void list<T, Alloc>::clear()
{
    link_type cur = (link_type) node->next; // begin()
    while(cur!=node){ // 遍历每一个节点
        link_type tmp = cur;
        cur = (link_type) cur->next;
        destroy_node(tmp); // 析构并释放当前节点
    }
    // 恢复node的原始状态
    node->next = node;
    node->prev = node;
}
17. 由于list是双向链表, 在考虑好边界条件的前提下, 从头添加(去除)元素和从尾添加(去除)元素
push_front, push_back       pop_front, pop_back
操作几乎是一样的，时间复杂度也没有什么大区别
18. list内部的transfer操作 将某段连续元素迁移到某个特定位置 之前
// 将[first, last)内所有的元素移动到position之前
void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
        (*(link_type((*last.node).prev))).next = position.node;
        (*(link_type((*first.node).prev))).next = last.node;
        (*(link_type((*position.node).prev))).next = first.node;  
        link_type tmp = link_type((*position.node).prev);
        (*position.node).prev = (*last.node).prev;
        (*last.node).prev = (*first.node).prev;
        (*first.node).prev = tmp;
    }
}

19. transfer是非公开的接口 公开接口是splice
20. transfer是完成merge(), reverse(), sort()在stl list源码的基石操作



