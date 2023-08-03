# Первый семинар
## intro
- рассказать про себя :)
- ответить на вопросы по лекции
- показать как сдавать задачи [если надо]

## lifetime
- lifetime of temporaries
- lifetime extension

## forwarding & co
- type deduction for forwarding references (explain what type is deduced in different cases)
- why is the template parameter non-optional in std::forward?

## move-семантика
- moving from a const object
- examples of copy elision (rvo)
- [fun times with exceptions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2855.html)
- [push_back overloads](https://stackoverflow.com/questions/28130531/why-are-there-two-overloads-for-vectorpush-back), [push_back vs emplace_back](https://quuxplusone.github.io/blog/2021/03/03/push-back-emplace-back/)

## smart-pointers
- interface, usages
- a tidbit about auto_ptr
- unique_ptr with deleter for legacy C constructs [skip?]
- [smart pointers as parameters](https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/)
- cyclic dependencies with shared_ptr
- make_shared vs. shared_ptr(new)

## problems
- deque
  - кольцевой буффер (храним указатели на начало и конец)
  - хотим, чтобы ссылки на объекты не менялись
  - можно хранить указатели, а не сами объекты
  - хотим разумную локальность данных
  - храним указатели на блоки по 512 байт
  - реаллокация в тот момент, когда указатели смотрят на один и тот же блок
      - выделяем в два раза больше блоков, как в векторе
      - перекладываем, следим, чтобы не было пропусков
      - добавляем новый блок в конец
