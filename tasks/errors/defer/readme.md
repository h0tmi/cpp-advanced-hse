# Defer

В языке Go есть встроенный оператор defer, который исполняется по выходе из функции. Например,

```golang
package main

import "fmt"

func main() {
  defer fmt.Println(", computer")
  defer fmt.Println("world")

  fmt.Println("hello")
}
```

То `world` напишется после вывода hello, а потом напишется `, computer`. Таким образом эмулируются деструкторы в языке Golang. И это полезно против бесконечных `if err != nil { return nil, err }`.

Сделайте класс Defer в C++. Он должен при выходе из scope вызывать зарегистрированный `void operator()() &&`, он может принимать любые функторы, а также должен поддерживать методы Invoke и Cancel. В сложных случаях обращайтесь к тестам.

Так как при Cancel должен вызываться деструктор, вам нужно уметь не вызывать его второй раз при удалении объекта. Для этого используйте аллоцированный на стеке буффер

```cpp
alignas(Callback) char callback_buffer_[sizeof(Callback)];
```

И чтобы положить Callback на эту структуру, используйте [placement new](https://en.cppreference.com/w/cpp/language/new):

```cpp
explicit Storage(Callback callback) {
    ::new (GetCallbackBuffer()) Callback(std::move(callback));
}

void* GetCallbackBuffer() { return static_cast<void*>(callback_buffer_); }

Callback& GetCallback() {
    return *reinterpret_cast<Callback*>(GetCallbackBuffer());
}
```

Тем самым вы сможете за место `callback_buffer_` побитово положить данные и контролировать, когда вы хотите вызывать деструктор или сам Callback.

Заметьте, что хоть Defer и шаблонный класс, сами шаблоны писать не надо, прочитайте как устроено [CTAD (class template argument deduction)](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) в C++ и сдайте задачу.
