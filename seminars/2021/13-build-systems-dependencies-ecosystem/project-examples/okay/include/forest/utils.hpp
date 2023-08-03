#pragma once

// No namespace -> polluting user code with implementation details
// Better: namespace forest::details {

// Not a template function -> definition can be extracted to `.cpp`
//                            if we are not going with "header-only" way.
void DoImplementationDetail(...) {
    // Implementation...
}

// }  // namespace forest::details
