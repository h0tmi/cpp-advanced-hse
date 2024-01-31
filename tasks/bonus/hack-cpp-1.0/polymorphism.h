#pragma once
#include <type_traits>

template <typename T>
concept Polymorphic = std::is_polymorphic_v<T>;

/* 
\param N the number of virtual functions that must be trapped
\param trapped the object to be trapped
\param trap_func the replacement function 
*/
template <size_t N, Polymorphic P, typename F>
void TrapObject(P& object, const F& trap_func) {
    // TODO: TRAP TRAP TRAP
}
