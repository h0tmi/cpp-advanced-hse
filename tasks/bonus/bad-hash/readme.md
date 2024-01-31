# Bad Hash

Это задача типа [crashme](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/docs/crashme.md).

Исходный код находится в файле `main.cpp`.
Исполняемый файл запускается на Ubuntu 22.04 и получен командой
```shell
clang++-14 main.cpp -o bad_hash -O2 -std=c++20
```
**Обратите внимание**, файл скомпилирован с `libstdc++`.

Советуем не писать ввод каждый раз руками, а сохранить его в файл и
пользоваться перенаправлением ввода
```shell
./bad_hash <input.txt
```

Послать ввод из файла на сервер можно [командой](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/docs/crashme.md#подготовленный-ввод):
```shell
(echo bad-hash; cat input.txt) | nc crashme.cpp-hse.net 9090
```

### Генератор
**Обратите внимание**, что вы также должны отправить генератор, который вы использовали для получения решения. Все файлы с вашим решением (за исключением самого файла с ответом) положите в директорию solution, после чего сделайте submit как обычно.
