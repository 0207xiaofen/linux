#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <cstdlib>

namespace wx{

class alloc{
private:
    enum{_ALIGN = 8};  //区块上调边界，也是链表管理的最小内存
    enum{_MAX_BYTES = 128};     //区块管理的内存上限
    enum{_NUM_LIST = _MAX_BYTES / _ALIGN};      //自由链表个数
    enum{_NLIST = 20};  //一次malloc _NLIST个内存块
private:
    union LIST{     //free_list节点构造，包含指向下一个自由链表的指针和自身数据
        union LIST* free_list_link;      
        char  client_data[1];
    };
private:
    static LIST* volatile free_lists[_NUM_LIST];    //16个自由链表

    static size_t ROUND_UP(size_t bytes){       //字节对齐，将字节上调为8的倍数，方便管理
        return (bytes + _ALIGN -1) & ~(_ALIGN - 1);
    }

    static size_t FREELIST_INDEX(size_t bytes){     //确定由哪条自由链表管理
        return (bytes + _ALIGN - 1) / _ALIGN - 1;
    }
private:
    static char* start_free;    //内存池起始指针
    static char* end_free;      //内存池末尾指针
    static size_t heap_size;     //内存池大小
    static void* refill(size_t bytes);   //
    static char* chunk_alloc(size_t bytes, size_t& nlist);  //配置nlist个大小为bytes的内存块
public:
    static void* allocate(size_t bytes);
    static void deallocate(void* ptr, size_t bytes);
    static void* reallocate(void* ptr, size_t old_sz, size_t new_size);
};

}


#endif