# Reallol

Это задача типа [crashme](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/docs/crashme.md).

Исходный код находится в файле `main.cpp`. Исполняемый файл получен командой
```shell
clang++-14 main.cpp -o reallol -std=c++11 -fsanitize=address
```

Советуем не писать ввод каждый раз руками, а сохранить его в файл и
пользоваться перенаправлением ввода
```shell
./reallol <input.txt
```

Послать ввод из файла на сервер можно [командой](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/docs/crashme.md#подготовленный-ввод):
```shell
{echo reallol; cat input.txt} | nc -N crashme.cpp-hse.net 9090
```

