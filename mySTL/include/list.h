#ifndef __LIST_H__
#define __LIST_H__

#include"alloctor.h"
//#include"construct.h"
#include<memory>
#include<utility>

namespace wx{

template<class T>
struct _LIST_NODE{      //list节点
    using L_Pointer = _LIST_NODE<T>*;
    L_Pointer prev;
    L_Pointer next;
    T data;
};

template<class T>
struct _list_iterator{      //list迭代器
    using iterator = _list_iterator<T>;
    using value_type = T;
    using link_type = _LIST_NODE<T>*;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    link_type node;

    _list_iterator() noexcept{}

    _list_iterator(const link_type x):node(x){}

    _list_iterator(const iterator& x):node(x.node) {}

    bool operator==(const iterator& that) const noexcept{
        return node == that.node;
    }

    bool operator!=(const iterator& that) const noexcept{
        return node != that.node;
    }

    T &operator*() noexcept{
        return node->data;
    }

    T const&operator*() const noexcept{
        return node->data;
    }

    pointer operator->() const noexcept{
        return &(operator*());
    }

    iterator &operator++() noexcept{     //++i
        node = node->next;
        return *this;
    }

    iterator &operator++(int) noexcept{      //i++
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    iterator &operator--() noexcept{     //--i
        node = node->prev;
        return *this;
    }

    iterator &operator--(int) noexcept{
        iterator tmp = *this;
        --*this;
        return tmp;
    }

    iterator &operator+(size_t i) noexcept{
        while(i--){
            node = node->next;
        }
        return *this;
    }

};

//template<class T, class Alloc = simple_Allocator<_LIST_NODE<T>>>
template<class T, class Alloc = std::allocator<_LIST_NODE<T>>>
class LIST{
private:
    using link_type = _LIST_NODE<T>*;
    using Node = _LIST_NODE<T>;
    using iterator = _list_iterator<T>;
    using reference = T&;
    using const_reference = const T&;
    using const_iterator = const iterator;
private:
    link_type  m_node;
    size_t m_size;
    Alloc m_alloc;
public:     //构造，析构函数
    LIST(){
        //m_node = new Node;
        m_node = m_alloc.allocate(1); 
        m_size = 0;
        construct(&m_node->data, 0);
        //m_node = get_node();
        m_node->prev = m_node;
        m_node->next = m_node;
    }

    explicit LIST(size_t n, const T&value=0): LIST(){
        // m_node = m_alloc.allocate(1);
        // m_size = 0;
        // construct(&m_node->data, 0);
        // m_node->prev = m_node->next = m_node;
        insert(begin(), n, value);
    }

    template<std::input_iterator InputIter>     //接受迭代器初始化
    LIST(InputIter first, InputIter last): LIST() {
        insert(begin(), first, last);
    }

    //接受初始化列表，委托LIST(Inputer first, Inputer last)构造
    LIST(std::initializer_list<T> ilist): LIST(ilist.begin(), ilist.end()){
    }

    void copy_create(const LIST& that){     //拷贝构造，拷贝复制直接调用
        auto first = that.begin();
        //auto position = begin();
        while(first != that.end()){
            //link_type node = get_node();
            link_type node = create_node(first.node->data);
            node->next = m_node;
            node->prev = m_node->prev;
            m_node->prev->next = node;
            m_node->prev = node;
            //construct(&node->data, that.get()->data);
            //++position;
            ++first;
        }
        // auto first = that.m_node->next;
        // auto last = that.m_node->prev;
        // m_node->prev = last;                                   //拷贝构造不能直接使用that的头和尾，不然会重复释放
        // m_node->next = first;
        // last->next = m_node;
        // first->prev = m_node;
        m_size = that.size();
    }

    void move_create(LIST&& that){     //移动构造，移动复制直接调用
        // auto first = that.begin();
        // //auto position = begin();
        // while(first != that.end()){
        //     //link_type node = get_node();
        //     link_type node = create_node(first.node->data);
        //     //destroy_node(first.node);                          //不用循环遍历that，一个节点一个节点插入，因为是循环链表，直接获取头尾指针插入
        //     //first.node = nullptr;
        //     node->next = m_node;
        //     node->prev = m_node->prev;
        //     m_node->prev->next = node;
        //     m_node->prev = node;
        //     //construct(&node->data, that.get()->data);
        //     ++first;
        // }
        // that.m_node->prev = that.m_node->next = that.m_node;
        // // m_size = that.size();
        // // that.size() = 0;
        // m_size = std::exchange(that.size(), 0);
        auto last = that.m_node->prev;
        auto first = that.m_node->next;
        m_node->next = first;
        m_node->prev = last;
        first->prev = m_node;
        last->next = m_node;
        that.m_node->prev = that.m_node->next = that.m_node;
        m_size = std::exchange(that.size(), 0);
    }

    LIST(const LIST& that): LIST(){
        // auto first = that.begin();
        // auto position = begin();
        // while(first != that.end()){
        //     //link_type node = get_node();
        //     link_type node = create_node(first.node->data);
        //     node->next = position.node;
        //     node->prev = position.node->prev;
        //     position.node->prev->next = node;
        //     position.node->prev = node;
        //     //construct(&node->data, that.get()->data);
        //     ++position;
        //     ++first;
        // }
        // m_size = that.size();
        copy_create(that);
    }

    LIST &operator=(const LIST& that){
        //insert(that.begin(), that.end());  拷贝复制是const， 而insert是非const
        if(begin() != end()){
            clear();
        }
        copy_create(that);
        return *this;
    }

    LIST(LIST&& that): LIST(){
        move_create(std::move(that));
    }

    LIST &operator=(LIST&& that){
        if(begin() != end()){
            clear();
        }
        move_create(std::move(that));
        return *this;
    }

    ~LIST() noexcept{
        clear();
        delete m_node;
        // link_type cur = m_node->next;
        // while(cur != m_node){
        //     destroy(&cur->data);
        //     link_type next = cur->next;
        //     //m_alloc.deallocate(cur);
        //     put_node(cur);
        //     cur = next;
        // }
        // destroy_node(m_node);
        // //m_node->prev = m_node->next = m_node;
        // m_size = 0;
        //clear();
        //put_node(m_node);
    }

    LIST &operator=(std::initializer_list<T> ilist){
        if(begin() != end()){
            clear();
        }
        insert(begin(), ilist.begin(), ilist.end());
        return *this;
    }
private:    //对节点的构建和回收
    link_type get_node(){
        return m_alloc.allocate(1);
    }

    void put_node(link_type p){
        m_alloc.deallocate(p, 1);
    }

    link_type create_node(const T& x){
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }

    void destroy_node(link_type p){
        destroy(&p->data);
        put_node(p);
    }
public:
    iterator insert(const iterator position, const T& val){
        //link_type node = new Node;
        //node->data = val;
        link_type node = m_alloc.allocate(1);
        construct(&node->data, val);
        node->next = position.node;
        node->prev = position.node->prev;
        position.node->prev->next = node;
        position.node->prev = node;
        ++m_size;
        return node;
    }

    iterator insert(const iterator position, T&& val){
        //link_type node = new Node;
        //node->data = std::move(val);
        link_type node = m_alloc.allocate(1);
        construct(&node->data, std::move(val));
        node->next = position.node;
        node->prev = position.node->prev;
        position.node->prev->next = node;
        position.node->prev = node;
        ++m_size;
        return node;
    }

    iterator insert(iterator position, size_t count, const T& val){
        auto cur = position.node;
        while(count){
            position = insert(position, val);
            ++position;
            --count;
        }
        return cur;
    }

    iterator insert(iterator position, std::initializer_list<T> ilist){
        auto cur = position;
        insert(position, ilist.begin(), ilist.end());
        return cur;
    }

    template<std::input_iterator InputIt>
    iterator insert(iterator position, InputIt first, InputIt last){
        auto cur = position;
        while(first != last){
            position = insert(position, *first);
            ++position;
            ++first;
        }
        return cur;
    }

    iterator erase(const_iterator position){
        link_type node = position.node->next;
        position.node->prev->next = position.node->next;
        position.node->next->prev = position.node->prev;
        destroy_node(position.node);
        --m_size;
        return node;
    }

    iterator erase(iterator first, iterator last){
        // size_t diff = last - first;
        // first.node->prev->next = last.node;
        // last.node->prev = first.node->prev;
        // m_size -= diff;
        while(first != last){
            first = erase(first);
        }
        return last;
    }
public:     //push，pop操作
    void push_back(const T& val){
        // link_type node = new Node;
        // //link_type node = m_alloc.allocate();
        // node->data = val;
        // //construct(&node->data, val);
        // //link_type node = create_node(val);
        // node->next = m_node;
        // node->prev = m_node->prev;
        // m_node->prev->next = node;
        // m_node->prev = node;
        // ++m_size;
        insert(end(), val);
    }

    void push_back(T &&val){    //用于不可拷贝的对象，例如thread，std::move(thread)进来
        // link_type node = new Node;
        // //link_type node = m_alloc.allocate();
        // node->data = std::move(val);
        // //construct(&node->data, std::move(val));
        // //link_type node = create_node(std::move(val));
        // node->next = m_node;
        // node->prev = m_node->prev;
        // m_node->prev->next = node;
        // m_node->prev = node;
        // ++m_size;
        insert(end(), val);
    }

    void push_front(const T& val){
        // link_type node = new Node;
        // node->data = val;
        // // link_type node = m_alloc.allocate();
        // // construct(&node->data, val);
        // //link_type node = create_node(val);
        // node->prev = m_node;
        // node->next = m_node->next;
        // m_node->next->prev = node;
        // m_node->next = node;
        // ++m_size;
        insert(begin(), val);
    }

    void push_front(T&& val){       //同上，std::move()
        // link_type node = new Node;
        // node->data = std::move(val);
        // //link_type node = m_alloc.allocate();
        // //construct(&node->data, std::move(val));
        // //link_type node = create_node(std::move(val));
        // node->prev = m_node;
        // node->next = m_node->next;
        // m_node->next->prev = node;
        // m_node->next = node;
        // ++m_size;
        insert(begin(), val);
    }

    void pop_back() noexcept{
        link_type old = m_node->prev;
        m_node->prev->prev->next = m_node;
        m_node->prev = m_node->prev->prev;
        if(old != m_node){
            --m_size;
            //delete old;
            destroy(&old->data);
            m_alloc.deallocate(old, 1);
            //destroy_node(old);
        }
    }

    void pop_front() noexcept{
        link_type old = m_node->next;
        m_node->next->next->prev = m_node;
        m_node->next = m_node->next->next;
        if(old != m_node){
            --m_size;
            //delete old;
            destroy(&old->data);
            m_alloc.deallocate(old, 1);
            //destroy_node(old);
        }
    }
public:     //迭代器begin end
    const iterator begin() const noexcept{
        return m_node->next;
    }

    iterator begin() noexcept{
        return m_node->next;
    }

    iterator end() noexcept{
        return m_node;
    }

    const iterator end() const noexcept{
        return m_node;
    }

public:
    size_t size() const noexcept{
        return m_size;
    }

    size_t &size() noexcept{
        return m_size;
    }

    bool empty() const noexcept{
        //return m_node->next==m_node;
        return begin() == end();
    }

    reference front() noexcept{
        return m_node->next->data;
    }

    const_reference front() const noexcept{
        return m_node->next->data;
    }

    reference back() noexcept{
        return m_node->prev->data;
    }

    const_reference back() const noexcept{
        return m_node->prev->data;
    }

    link_type const &get() const noexcept{
        return m_node;
    }

    link_type &get() noexcept{
        return m_node;
    }
public:
    void clear() noexcept{
        link_type cur = m_node->next;
        while(cur != m_node){
            auto next = cur->next;
            destroy_node(cur);
            //delete cur;
            cur = next;
        }
        m_node->prev = m_node->next = m_node;
        m_size = 0;
    }

    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args){
        link_type node = get_node();
        node->prev = position.node->prev;
        node->next = position.node;
        position.node->prev->next = node;
        position.node->prev = node;
        construct(&node->data, std::forward<Args>(args)...);
        ++m_size;
        return node;
    }

    void resize(const size_t count, const T& val=0){
        if(m_size == count)[[unlikely]] return;
        else if(m_size > count){
            //size_t diff = m_size - count;
            iterator first = begin() + count;
            while(first != end()){
                first = erase(first);
            }
            m_size = count;
        }else{
            size_t diff = count - m_size;
            insert(end(), diff, val);
            m_size = count;
        }
    }
public:
    void assign(size_t count, const T& val){
        clear();
        insert(begin(), count, val);
    }

    template<std::input_iterator Input>
    void assign(Input first, Input last){
        clear();
        insert(begin(), first, last);
    }

    void assign(std::initializer_list<T> ilist){
        assign(ilist.begin(), ilist.end());
    }
};


}

#endif