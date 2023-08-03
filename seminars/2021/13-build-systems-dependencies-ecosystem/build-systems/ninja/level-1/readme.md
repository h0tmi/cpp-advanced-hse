# Ninja example: level-1

* Изучите файл [`build.ninja`](./build.ninja).
* Запустите `hello_world`:

  ```shell
  ninja hello_world
  ```

* Соберите `main`:

  ```shell
  ninja run_main
  ```

* Попробуйте изменить код `SimpleAny` и снова собрать `main`. 
  Изменилось ли что-нибудь?
* Покликайте по неявному графу зависимостей:

  ```shell
  ninja -t browse --port=8777 run_main
  ```

* Посмотрите на явный граф зависимостей:

  ```shell
  sudo apt install graphviz
  ninja -t graph run_main | dot -T png -o graph.png
  open graph.png
  ```
