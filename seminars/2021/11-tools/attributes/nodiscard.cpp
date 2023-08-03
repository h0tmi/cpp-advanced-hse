[[nodiscard]] int get_int() {
    return 8;
}

class [[nodiscard]] ImportantClass{};

ImportantClass get_important_class() {
    return ImportantClass();
}

int main() {
    get_int();
    get_important_class();
}