# Dependencies via CMake find_package example

* Установка зависимостей:

  * Linux:

    ```shell
    sudo apt-get install libboost-all-dev
    ```

  * Mac OS:

    ```shell
    brew install boost
    ```

* Сборка:

  ```shell
  mkdir build && cd build
  cmake ..
  make main
  ```

* Запуск

  * В одном окне терминала запускаем сервер:
    ```shell
    ./main 8777
    ```

  * В другом подключаемся к нему и пишем сообщения:
    ```shell
    nc localhost 8777
    hello, world!
    ```
