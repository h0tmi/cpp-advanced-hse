#pragma once  // Can lead to problems that header guards prevent: https://belaycpp.com/2021/10/20/pragma-once-or-twice/

#include "utils.hpp"  // Better to move `utils.hpp` to `forest/details/`

namespace forest {  // Good

template <typename T>  // Cannot specify custom allocator :(
class AVLTree {
public: // Good: users should only care about public interface, so try to put it up.
    // Implementation...

private:
    // Implementation...

    void ImplementationDetail() {
        DoImplementationDetail();
    }
};

}  // namespace forest
