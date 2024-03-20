#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

namespace wx{
template<class T, class iterator>
iterator find(iterator first, iterator last, const T& val){
    while(first != last){
		if(*first == val)
			return first;
		++first;
	}
	return last;
}

// template<class iterator>
// void sort(iterator first, iterator last){
// 	while(first != last){
// 		if()
// 	}
// }

}
#endif
