---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Обработка ошибок в C++

Данила Кутенин. Лекция 5

---

#### Коды ошибок
+ Вводим перечисление возможных численных кодов ошибок
```cpp
enum class Error {
    EOK = 0,
    ENOMEM = 1,
    EINVAL = 2,
    // etc
}
```
+ Дополнительный канал для передачи значения кода ошибки

---

[Linux kernel](https://elixir.bootlin.com/linux/latest/source/fs/f2fs/file.c#L54)

```cpp
static vm_fault_t f2fs_vm_page_mkwrite(struct vm_fault *vmf)
{
    struct page *page = vmf->page;
    struct inode *inode = file_inode(vmf->vma->vm_file);
    struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
    struct dnode_of_data dn;
    bool need_alloc = true;
    int err = 0;

    if (unlikely(f2fs_cp_error(sbi))) {
        err = -EIO;
        goto err;
    }

    if (!f2fs_is_checkpoint_ready(sbi)) {
        err = -ENOSPC;
        goto err;
    }

    err = f2fs_convert_inline_inode(inode);
    if (err)
        goto err;
```

---

```cpp
out_sem:
    up_read(&F2FS_I(inode)->i_mmap_sem);

    sb_end_pagefault(inode->i_sb);
err:
    return block_page_mkwrite_return(err);
}
```

---
#### Коды ошибок
```cpp
// 1. Output parameter
File* openFile(const char* path, Error* error);

// 2. Return error code
Error openFile(const char* path, File** result);

// 2.1 Tuple (Value, Error), e.g. <charconv>
struct OpenFileResult {
    File* File;
    Error Error;
};
OpenFileResult openFile(const char* path);

// 3. Thread-local (global) state (errno)
File* openFile(const char* path);
```

---

#### Проблемы кодов ошибок
+ Легко пропустить обработку
+ Ветвление на каждую проверку
+ Теряется контекст
+ Предопределенные коды ошибок

---

#### Классификация видов животных
+ Принадлежащих Императору
+ Набальзамированных
+ Прирученных
+ Молочных поросят
+ Сирен
+ Сказочных
+ Бродячих собак
+ Включённых в эту классификацию
+ Бегающих как сумасшедшие
+ Бесчисленных
+ Нарисованных тончайшей кистью из верблюжьей шерсти
+ Прочих
+ Разбивших цветочную вазу
+ Похожих издали на мух

---

#### Коды ошибок в C++

+ Есть в стандартной библиотеке (`<system_error>`, `<charconv>`)
+ Не подходят в качестве общего механизма обработки ошибок

---

#### Коды ошибок в C++
```cpp
template <typename T>
class BadVector {
public:
    BadVector(size_t capacity)
        : begin_{(T*)malloc(sizeof(T) * capacity)}
        , capacity_{capacity}
        , size_{0}
    {/* begin_ = nullptr */}

private:
    T* begin_;
    size_t capacity_;
    size_t size_;
};
```

---

#### Коды ошибок в C++
```cpp
template <typename T>
class BadVector {
public:
    BadVector(size_t capacity)
        : begin_{(T*)malloc(sizeof(T) * capacity)}
        , capacity_{capacity}
        , size_{0}
    {}

    bool IsValid() const {
        return begin_ == end_cap_ || begin_ != nullptr;
    }

private:
    T* begin_;
    size_t capacity_;
    size_t size_;
};
```

---

#### Исключения
```cpp
template <typename T>
class BadVector {
public:
    BadVector(size_t capacity)
        : begin_{(T*)malloc(sizeof(T) * capacity)}
        , capacity_{capacity}
        , size_{0}
    {
        if (begin_ == nullptr) {
            throw std::bad_alloc{};
        }
    }

private:
    T* begin_;
    size_t capacity_;
    size_t size_;
};
```

---

#### Исключения
```cpp
template <typename T>
class BadVector {
public:
    BadVector(size_t capacity)
        : begin_{(T*)operator new(sizeof(T) * capacity)}
        , capacity_{capacity}
        , size_{0}
    {
    }

private:
    T* begin_;
    size_t capacity_;
    size_t size_;
};
```

---

#### Синтаксис

```cpp
void FailngFunc() {
  throw std::runtime_error("Fail");
}
void SomeFunc() {
  std::vector<int> x(10);
  FailingFunc();
}
int main() {
  try {
    SomeFunc();
  } catch (const std::exception& ex) {
  }
}
```

---

#### Иерархия

```cpp
namespace std {
class exception {
    virtual const char* what() = 0;
};

class runtime_error : public exception { /* ... */ };
class logic_error : public exception { /* ... */ };
class invalid_argument : public logic_error {};
} // namespace std
```

---

#### Иерархия

```cpp
void F() {
  try {
    throw std::runtime_error("FOO");
  } catch (const std::logic_error& ) {

  } catch (const std::runtime_error& ) {
    // Land here <---
  } catch (const std::exception& ) {

  }
}
```

---

#### Исключения
+ Предназначены для ошибок, которые можно обработать
+ Линейный код
+ Нельзя случайно пропустить
+ Обрабатываются в подходящем месте
+ Объект исключения несет необходимый контекст
+ Нет лишних проверок при отсутствии ошибок
+ Можно строить сложную иерархию

---

#### Процесс выброса исключения
+ Выделяем объект исключения
+ Выбираем подходящий `catch`
+ Раскручиваем стек (зовем деструкторы объектов)
+ Передаем управление выбранному `catch`-блоку

---

#### Тонкости
+ Исключения из деструкторов по-умолчанию запрещены (неявно `noexcept`)
+ При попытке выбросить исключение из деструктора при наличии необработанного исключения получаем `std::terminate`
+ Может быть более одного активного исключения (`std::uncaught_exceptions()`)

---

#### Гарантии безопасности исключений
+ Базовая гарантия безопасности: при возникновении исключения во время выполнения операции состояние программы согласовано `std::vector::operator=`
+ Строгая гарантия безопасности: при выполнении операции исключения не оказывают влияния на состояние программы `std::vector::push_back`
+ Гарантия отсутствия исключений: при выполнении операции исключения не возникают (`noexcept`)
+ Безгарантийное: при исключении может быть всё что угодно, например, потеря памяти, но не Undefined Behavior

---

#### Вопрос?

Как вы думаете, в реальных программах что происходит?

---

#### Ответ

+ Ставить правильно catch очень сложно, поэтому ставят часто около main
+ Eсли вдруг ошибка, убить программу и знать где её убить
+ Был эксперимент от Microsoft, они поставили аллокатор, который время от времени кидает исключения просто так. В итоге Microsoft Office сломался через 30 секунд использования без возможности восстановления (поломался индекс)
+ Исключения, которые указывают на то, что вы истощили ресурсы машины, вряд ли смогут быть брошены

---

#### Ответ

Большинство больших программ практически не даёт 100% гарантии даже базовой безопасности. Большинство безгарантийные.

---

#### Проблемы исключений
+ Реализации механизма исключений сложные и медленные
+ Сложно оптимизировать
+ Неявные пути исполнения
+ Легко пропустить корректную обработку ошибок

---

#### Типы-суммы
+ Функциональный подход к обработке ошибок
+ `Result<T, E>` из Rust или `Either<L, R>` из Haskell
+ `llvm::Expected<T>`, `absl::StatusOr<T>`
+ `std::expected<T, E>` / `Result<T, E>` / `Either<T, E>` (P0323)

---

#### Google experience

+ В Google нет исключений, есть `absl::Status`, `absl::StatusOr<T>`
+ `RETURN_IF_ERROR(expr)`, чтобы избежать много ifов и goto, макрос
+ `ASSIGN_OR_RETURN(var, expr)`, тоже макрос
+ Вроде как живём уже 20 лет с этим, работает
+ `absl::Status` помечен как `[[nodiscard]]`, то есть нельзя проигнорировать возвращаемое значение

---

#### Google experience

```cpp
absl::StatusOr<std::string> ReadUserNameFromFile() {
    ASSIGN_OR_RETURN(auto f, File::open("hello.txt"));
    std::string s;
    RETURN_IF_ERROR(f.ReadToString(&s));
    return s;
}
```


---

#### C experience

+ Постоянные `goto err` (не так плохо как кажется)
+ Вроде тоже живут, но ошибки из-за отсутствия нормальной типизации (все ошибки в `enum`, нет `[[nodicard]]`)

---

#### Go experience

+ Статические чекеры проверяют, что вы возвращаете ошибки

```golang
func ReadUserNameFromFile() (string, error) {
    file, err := Open("hello.txt")
    if err != nil {
        return nil, err
    }
    var s string
    err := file.Read(&s)
    if err != nil {
        return nil, err
    }
    return s, nil
}
```

---

#### Go experience

![img](https://pbs.twimg.com/media/DCIF7-2W0AEAv9c.jpg)

---

#### Rust experience

```rust
use std::fs::File;
use std::io;
use std::io::Read;

fn read_username_from_file() -> Result<String, io::Error> {
    let mut f = File::open("hello.txt")?; // question mark! like ASSIGN_OR_RETURN
    let mut s = String::new();
    f.read_to_string(&mut s)?; // question mark! like RETURN_IF_ERROR
    Ok(s)
}
```

---

#### Развитие механизмов обработки ошибок
+ Zero-overhead deterministic exceptions (P0709)
```cpp
int SafeDivide(int i, int j) throws {
    if (j == 0) {
      throw arithmetic_errc::divide_by_zero;
    }
    if (i == std::numeric_limits<int>::min() && j == -1) {
      throw arithmetic_errc::integer_divide_overflows;
    }
    if (i % j != 0) {
      throw arithmetic_errc::not_integer_division;
    }
    return i / j;
}
```
