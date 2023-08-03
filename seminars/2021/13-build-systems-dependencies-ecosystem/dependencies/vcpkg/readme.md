# Dependencies via vcpkg example

* Необходимые проекту зависимости описаны в файле [`vcpkg.json`](./vcpkg.json).

* Собираем:

  ```shell
  mkdir build && cd build
  export VCPKG_ROOT=<FILL ME>
  cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
  make main
  ```

* Запускаем:

  ```shell
  ./main
  ```

* Откуда код? 
  [Отсюда](https://github.com/conan-io/examples/tree/master/libraries/dear-imgui/basic).
