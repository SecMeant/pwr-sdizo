#pragma once

namespace sdizo{
using heap_key_t = int32_t;

template<typename ElemType>
heap_key_t key(ElemType element)
{return element.getKey();}

template<>
heap_key_t key(int32_t element);
}; //namespace sdizo

