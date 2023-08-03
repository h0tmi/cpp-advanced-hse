# Dependencies via Conan example

## Требования

- Conan

Если есть `pipenv`, то можно установить вот так:

```shell
pipenv shell
pipenv install
```

Или через `pip`:
```shell
python3 -m venv conan_env
source conan_env/bin/activate
pip install conan
```

## Зависимости

- fmt
- date

Создаём свой профиль (конфигурацию для сборки):
```shell
conan profile new ./conan-profiles/my-profile --detect
```

Устанавливаем зависимости:
```shell
mkdir build && cd build
conan install .. --profile=../conan-profiles/my-profile
```

## Сборка и запуск

Будучи в директории `build`:
```shell
cmake ..
make main && bin/main $USER
```

## Conan

Искать пакеты можно при помощи CLI:
```shell
conan search --remote all imgui
```
