[video](https://www.youtube.com/watch?v=_Ivd3qzgT7U)

1. **Catch** order matters (order_matters.cpp)
2. **Stack unwinding**
  * combining new-delete and exceptions is a bad idea. (new_and_unwinding.cpp)
  * destructors and more new-delete (destructors_and_catch.cpp)
  * throw exceptions and try to catch them in the same try-catch block (rethrow_multi_catch.cpp)
  * destructors order (stack unwinding) (long_exception.cpp)

3. **noexcept**
  * **noexcept** (move_noexcept.cpp)
  * **move_if_noexcept**
  * throw from **noexcept** (lier.cpp)
  * **noexcept(std::is_pod<T>)**

4. **Exception safety**
* Nothrow exception guarantee, e.g. destructor (bad_destructor.cpp)
* Strong exception guarantee --- state just before the function call (on exception), e.g. vector::push_back
* Basic exception guarantee --- valid state (throw_constructor.cpp)
5. **Performance** (performance.cpp)
```
perf 0  0.18s user 0.00s system 98% cpu 0.183 total
perf 1  15.58s user 0.00s system 99% cpu 15.587 total
```
Very simple try-catch block to [asm](https://godbolt.org/z/4f93nMcaP)

6. **Throw by value, catch by reference** (catch_reference_of_value.cpp) [some text](http://ptgmedia.pearsoncmg.com/images/0321113586/items/sutter_item73.pdf)
7. Exceptions: where they live (heap or some memory for a small amount of std::bad_alloc)
8. Don't use **setjmp** and **longjmp** with С++
9. [**exception** header](https://en.cppreference.com/w/cpp/header/exception)
* rethrow_exception
* current_exception
* exception_ptr
* uncaught_exceptions [cppreference example](https://en.cppreference.com/w/cpp/error/uncaught_exception), seems to be enough
* terminate handler [cppreference](https://en.cppreference.com/w/cpp/error/terminate_handler)
  * set_terminate instead of std::abort, [cppreference](https://en.cppreference.com/w/cpp/error/set_terminate), [ibm](https://www.ibm.com/docs/en/zos/2.3.0?topic=only-terminate-function-c)
10. might be interesting [link](https://stackoverflow.com/questions/307610/how-do-exceptions-work-behind-the-scenes-in-c#307716)
11. [The true cost of zero-cost exceptions](https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/)
