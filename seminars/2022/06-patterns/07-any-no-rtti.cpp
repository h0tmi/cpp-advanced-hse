// compile with -fno-rtti

#include <tuple>
#include <memory>
#include <iostream>

template <typename T>
void NoOp() {
}

typedef void (*TVoidFunction)();

class Any {
public:
    class InnerBase {
    public:
        virtual TVoidFunction GetTag() = 0;
        virtual ~InnerBase() = default;
    };

    template <class T>
    class Inner : public InnerBase {
    public:
        Inner(T&& val) : val_(std::move(val)) {
        }

        TVoidFunction GetTag() override {
            return &NoOp<T>;
        }

        T val_;
    };

    template <class T>
    Any(T&& t) : ptr_(new Inner<T>(std::forward<T>(t))) {
    }

    template <class T>
    const T& Get() {
        if (ptr_->GetTag() == &NoOp<T>) {
            // safe after check
            return static_cast<Inner<T>&>(*ptr_).val_;
        } else {
            throw std::bad_cast();
        }
    }

private:
    std::unique_ptr<InnerBase> ptr_;
};

int main() {
    Any a = std::string{"kek"};

    std::cout << a.Get<std::string>() << "\n";

    a.Get<double>();
}
