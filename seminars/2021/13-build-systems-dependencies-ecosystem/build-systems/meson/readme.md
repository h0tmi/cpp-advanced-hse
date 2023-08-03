# Meson example

* Проект создан самим Meson:
  ```shell
  meson init --name particle --build
  ```

* Инициализируем сборку:
  ```shell
  meson setup build
  ```

* Непосредственная сборка таргетов:
  ```shell
  meson compile -C build
  ```
