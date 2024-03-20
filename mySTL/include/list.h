#ifndef __LIST_H__
#define __LIST_H__


namespace wx{

template<class T>
struct _LIST_NODE{
    using L_Pointer = _LIST_NODE<T>*;
    L_Pointer prev;
    L_Pointer next;
    T m_data;
};

template<class T>
class LIST{
private:
    _LIST_NODE<T>*  m_node;
public:
    LIST(){
        m_node->data = 0;
        m_node->prev = m_node;
        m_node->next = m_node;
    }

    ~LIST(){
        
    }


};


}

#endif