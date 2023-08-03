#include <iostream>
#include <iterator>
#include <memory>

// Non-template class -> no templated fields are possible
class Any {
    class AnyBase {
    public:
        virtual ~AnyBase() = default;
    };

    template <typename T>
    class AnyDerived : public AnyBase {
    public:
        template <typename... Args>
        AnyDerived(Args&&... args) : t_(std::forward<Args>(args)...) {
        }

        ~AnyDerived() {
            // type `T` is available here, `t_` will be deleted
        }

    private:
        T t_;
    };

public:
    Any() = default;

    /*
        Type `T` is available only in the scope of this ctor.
        But we can create templated `AnyDerived<T>` class and it will know how to destruct.
    */
    template <typename T>
    Any(T&& t) : storage_(new AnyDerived<std::decay_t<T>>(std::forward<T>(t))) {
    }

    ~Any() {
        // storage_ will be deleted -> virtual dtor of AnyBase will be called -> AnyDerived<T> will
        // be destructed
    }

private:
    std::unique_ptr<AnyBase> storage_;
};

int main() {
    int x = 42;
    Any a(x);
}
