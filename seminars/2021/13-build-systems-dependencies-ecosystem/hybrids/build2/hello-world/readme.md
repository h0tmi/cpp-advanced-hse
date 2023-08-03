# build2 example

## Сборка

* Папка `hello` создана `bdep`:

  ```shell
  bdep new -l c++,cpp -t exe hello
  ```

* Из директории `hello` создадим конфигурацию для сборки с clang:

  ```shell
  bdep init -C ../hello-clang @clang cc config.cxx=clang++
  ```
  
* И конфигурацию для gcc:

  ```shell
  bdep init -C ../hello-gcc @gcc cc config.cxx=g++
  ```

* Попробуем собрать:

  ```shell
  b
  ```
  
  Собралась только дефолтная (первая) конфигурация.
  Соберём ещё gcc:

  ```shell
  b ../hello-gcc/hello/
  ```

* Запустим тесты:

  ```shell
  b test
  ```
  
* Ну и сам код:

  ```shell
  hello/hello World
  ```

## Зависимости

* Добавим репозиторий с зависимостью libhello в 
  [манифест репозиториев](./hello/repositories.manifest).
  (Он там уже есть, на самом деле, нужно раскомментировать строчки с cppget).

* В [манифест зависимостей](./hello/manifest) добавим libhello.

* В [файле сборки](./hello/hello/buildfile) подключим libhello.

* Поменяем код на 

  ```cpp
  #include <libhello/hello.hxx>
  // ...

  int main (int argc, char* argv[])
  {
    // ...
    hello::say_hello (cout, argv[1]);
  }
  ```

* Summary:

  ```logs
  repositories.manifest   # add https://pkg.cppget.org/1/stable
  manifest                # add 'depends: libhello ^1.0.0'
  buildfile               # import libhello
  hello.cxx               # use libhello
  ```

* Проверим состояние проекта:

  ```shell
  bdep status
  ```

* Соберём с зависимостью:

  ```shell
  b
  ```
