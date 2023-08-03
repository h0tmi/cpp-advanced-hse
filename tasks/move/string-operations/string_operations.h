#pragma once

/*
`bool StartsWith(STR string, STR text)` — проверяет, что строка `string` начинается с `text`.

`bool EndsWith(STR string, STR text)` — проверяет, что строка `string` оканчивается на `text`.

`STR StripPrefix(STR string, STR prefix)` — возвращает `string` с убранным `prefix`,
если `string` не начинается на `prefix`, возвращает `string`.

`STR StripSuffix(STR string, STR suffix)` — тоже самое, но с суффиксом.

`STR ClippedSubstr(STR s, size_t pos, size_t n = STR::npos)` — тоже самое, что и `s.substr(pos, n)`,
но если `n` больше `s.size()`, то возвращается `s`.

`STR StripAsciiWhitespace(STR)` — `strip` строки, удаляем все символы с обоих концов
вида [isspace](https://en.cppreference.com/w/cpp/string/byte/isspace).

`std::vector<STR> StrSplit(STR text, STR delim)` — делаем `split` строки по `delim`. Подумайте,
прежде чем копипастить из уже имеющейся задачи. Обойдитесь одной аллокацией памяти.

`STR ReadN(STR filename, int n)` — открывает файл и читает `n` байт из filename. Используйте Linux
Syscalls `open`, `read`, `close`. Если открыть или прочитать файл нельзя, возвращает пустую строчку.

`STR AddSlash(STR path)` — добавляет к `path` файловой системы символ `/`, если его не было.

`STR RemoveSlash(STR path)` — убирает `/` из `path`, если это не сам путь `/` и путь заканчивается
на `/`.

`STR Dirname(STR path)` — известно, что `path` — корректный путь до файла без слеша на конце,
верните папку, в которой этот файл лежит без слеша на конце, если это не корень.

`STR Basename(STR path)` — известно, что `path` — корректный путь до файла, верните его название.

`STR CollapseSlashes(STR path)` — известно, что `path` — корректный путь, но `/` могут повторяться,
надо убрать все повторения.

`STR StrJoin(const std::vector<STR>& strings, STR delimiter)` — склеить все строки в одну через
`delimiter`. Обойдитесь одной аллокацией памяти.

`STR StrCat(Args...)` — склеить все аргументы в один в их строковом представлении.
Должны поддерживаться числа (`int, long, long long` и их `unsigned` версии), также все строковые
типы (`std::string, std::string_view, const char*`). Аргументов в `StrCat` не больше пяти.
Придумайте как это сделать за одну аллокацию памяти.
*/
