// No polymorphism
struct B {};
struct D : B {};

int main() {
    // Will not compile, no polymorphism -> no RTTI -> no downcast
    /*
    {
        B* ptr = new D;
        dynamic_cast<D*>(ptr);
    }
    */
    // Will compile. upcast -> no need in RTTI
    {
        D* ptr = new D;
        dynamic_cast<B*>(ptr);
        delete ptr;
    }
}
