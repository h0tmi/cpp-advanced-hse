# Bazel example

* Собираем:
  
  ```shell
  bazel build //clocks:main
  ```

  `//` -- корень репозитория сборки, `clocks` -- пакет (директория с файлом
  `BUILD`), `main` -- таргет.
  
* Запускаем:
  
  ```shell
  bazel-bin/clocks/main
  ```
  
* Тестируем:

  ```shell
  bazel test //tests:test_chromo
  ```

* Смотрим граф:

  ```shell
  sudo apt install graphviz xdot
  xdot <(bazel query --notool_deps --noimplicit_deps "deps (//clocks:main)" --output graph)
  ```
