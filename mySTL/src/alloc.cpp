#include"alloc.h"

namespace wx{

char* alloc::start_free = 0;
char* alloc::end_free = 0;
size_t alloc::heap_size = 0;
alloc::LIST* volatile alloc::free_lists[alloc::_NUM_LIST] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};  

void* alloc::allocate(size_t bytes){
    if(bytes > _MAX_BYTES){ //如果所需内存大于链表管理的最大内存，则直接调用malloc
        return malloc(bytes);
    }
    LIST* volatile *my_free_list;
    LIST* result;
    my_free_list = free_lists + FREELIST_INDEX(bytes);  //获取目标链表地址
    result = *my_free_list;     //拿到链表
    if(result == 0){
        void* r = refill(ROUND_UP(bytes));
        return r;
    }
    *my_free_list = result->free_list_link;     //内存被用，指向下一块空闲内存
    return result;

}

void alloc::deallocate(void* ptr, size_t bytes){
    if(bytes > _MAX_BYTES){ //判断内存是否是从链表中给出去的，如果不是，直接调用free
        free(ptr);
        return;
    }
    LIST* volatile *my_free_list;
    LIST* q = (LIST*)ptr;
    my_free_list = free_lists + FREELIST_INDEX(bytes);
    q->free_list_link = *my_free_list;      //回收内存
    *my_free_list = q;
}

void* alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz){       //重新分配内存
    deallocate(ptr, old_sz);
    ptr = allocate(new_sz);
    return ptr;
}

void* alloc::refill(size_t bytes){
    size_t nlist = _NLIST;
    char* chunk = chunk_alloc(bytes, nlist);    //从内存池拿nlist个大小为bytes的连续内存块，nlist是引用传递，内存不足时可以修改大小
    if(nlist == 1){
        return chunk;   //如果只拿到一块，直接分配出去，不用串到链表中
    }

    LIST* volatile *my_free_list;
    LIST* result = 0;
    LIST* curent_list = 0;
    LIST* next_list = 0;
    my_free_list = free_lists + FREELIST_INDEX(bytes);  //获取链表位置
    result = (LIST*)chunk; 
    *my_free_list = next_list = (LIST*)(chunk + bytes);
    for(int i=1; ; ++i){    //i从1开始，因为第0块已经被给出去了，从1开始将内存块串起来
        curent_list = next_list;
        next_list = (LIST*)(next_list + bytes);
        if(nlist - i == 1){
            curent_list->free_list_link = 0;
            break;
        }
        curent_list->free_list_link = next_list;
    } 

    return result;
}

char* alloc::chunk_alloc(size_t bytes, size_t& nList){
    size_t need_bytes = bytes * nList;              //需要的内存
    size_t have_bytes = start_free - end_free;      //当前内存池还有多少内存
    char* result;
    if(have_bytes > need_bytes){    //如果内存池足够给出nlist个，直接给出，调整heap_size的start指针
        result = start_free;
        start_free += need_bytes;
        return result;
    }else if(have_bytes >= bytes){  //如果内存池可以给出小于nlist个，调整nlist， 给出，然后调整指针
        nList = have_bytes / bytes;
        result = start_free;
        start_free += nList * bytes;
        return result;
    }else{                      //如果内存池一个都给不出
        size_t bytes_get = 2 * need_bytes + ROUND_UP(heap_size << 4);   //向系统heap索取2倍大小的need_bytes，一般给出去，一般内存池自留
        if(have_bytes > 0){             //把剩余的内存利用起来，串到对应的自由链表里面
            LIST* volatile *my_free_list;
            my_free_list = free_lists + FREELIST_INDEX(have_bytes);
            ((LIST*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (LIST*)start_free;
        }

        start_free = (char*)malloc(bytes_get);
        if(start_free == 0){     //malloc失败
            LIST* volatile *my_free_list;
            LIST* ptr;
            int i;
            for(i=bytes; i<_MAX_BYTES; i+=_ALIGN){      //在管理区块比bytes大的自由链表搜索有没有内存块可用
                my_free_list = free_lists + FREELIST_INDEX(i);
                ptr = *my_free_list;
                if(ptr != 0){           //有则给出去，调整链表可用的区块大小
                    *my_free_list = ptr->free_list_link;
                    start_free = (char*)ptr;
                    end_free = start_free + i;
                    return chunk_alloc(bytes, nList);
                }
            }
            end_free = 0;     
        }

        heap_size += bytes_get;
        end_free = start_free + bytes_get;
        return chunk_alloc(bytes, nList);
    }
}


}