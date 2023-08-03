// compile with -Wthread-safety flag

#include <mutex>

class __attribute__((capability("mutex"))) Mutex {
private:
    std::mutex std_mutex;

public:
    void Lock() __attribute__((exclusive_lock_function)) {
        std_mutex.lock();
    }

    void Unlock() __attribute__((unlock_function)) {
        std_mutex.unlock();
    }
};

class MyObject {
public:
    Mutex Mu;
    int a __attribute__((guarded_by(Mu)));
    int *b __attribute__((pt_guarded_by(Mu)));
};
void foo(MyObject &Obj) {
    //    Obj.Mu.Lock();
    Obj.a = 0;  // Warning: requires lock Obj.Mu
                //    Obj.Mu.Unlock();

    Obj.b = &Obj.a;  // OK
    *Obj.b = 1;      // Warning: requires lock Obj.Mu
}

int main() {
    MyObject obj;
    foo(obj);
}
