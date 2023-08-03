#include <Poco/JSON/Parser.h>

auto main() -> int {
    const auto raw_json = R"(
        {
            "number": 123,
            "array": ["one", "two", "three"]
        }
    )";

    const auto parsed = Poco::JSON::Parser{}.parse(raw_json).extract<Poco::JSON::Object::Ptr>();

    std::cout << "number = " << parsed->getValue<std::int64_t>("number") << std::endl;

    std::cout << "array = [";
    const auto array = parsed->getArray("array");
    for (auto it = array->begin(); it != array->end(); ++it) {
        std::cout << it->convert<std::string>();
        if (it + 1 != array->end()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
