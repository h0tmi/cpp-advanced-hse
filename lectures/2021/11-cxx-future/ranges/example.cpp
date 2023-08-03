import <ranges>;
import <iostream>;
import <vector>;

/*
* Install llvm-13 https://libcxx.llvm.org/BuildingLibcxx.html#the-default-build

clang++ -std=c++20 -O3 -fmodules -nostdinc++ -nostdlib++ \
  -isystem /usr/local/include/c++/v1/ -L/usr/local/lib -Wl,-rpath,/usr/local/lib \
  -lc++ example.cpp -o example
*/

int main() {
  std::vector<int> il = {3, 1, 4, 1, 5, 9};
  for (int i : il) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
  std::ranges::reverse_view rv{il};
  for (int i : rv) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
  for (int i : il | std::views::reverse) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
  for (size_t i = 0; i != rv.size(); ++i)
    std::cout << rv[i] << ' ';
  std::cout << '\n';
}
