// Copyright 2019 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef ABSL_STATUS_INTERNAL_STATUS_INTERNAL_H_
#define ABSL_STATUS_INTERNAL_STATUS_INTERNAL_H_

#if defined(__cplusplus) && defined(__has_cpp_attribute)
// NOTE: requiring __cplusplus above should not be necessary, but
// works around https://bugs.llvm.org/show_bug.cgi?id=23435.
#define ABSL_HAVE_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define ABSL_HAVE_CPP_ATTRIBUTE(x) 0
#endif

#ifdef __has_attribute
#define ABSL_HAVE_ATTRIBUTE(x) __has_attribute(x)
#else
#define ABSL_HAVE_ATTRIBUTE(x) 0
#endif

#if ABSL_HAVE_ATTRIBUTE(nodiscard)
#define ABSL_MUST_USE_RESULT [[nodiscard]]
#elif defined(__clang__) && ABSL_HAVE_ATTRIBUTE(warn_unused_result)
#define ABSL_MUST_USE_RESULT __attribute__((warn_unused_result))
#else
#define ABSL_MUST_USE_RESULT
#endif

#if ABSL_HAVE_ATTRIBUTE(noreturn) || (defined(__GNUC__) && !defined(__clang__))
#define ABSL_ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define ABSL_ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define ABSL_ATTRIBUTE_NORETURN
#endif

#if ABSL_HAVE_CPP_ATTRIBUTE(clang::lifetimebound)
#define ABSL_ATTRIBUTE_LIFETIME_BOUND [[clang::lifetimebound]]
#elif ABSL_HAVE_ATTRIBUTE(lifetimebound)
#define ABSL_ATTRIBUTE_LIFETIME_BOUND __attribute__((lifetimebound))
#else
#define ABSL_ATTRIBUTE_LIFETIME_BOUND
#endif

#if ABSL_HAVE_ATTRIBUTE(nonnull) || (defined(__GNUC__) && !defined(__clang__))
#define ABSL_ATTRIBUTE_NONNULL(arg_index) __attribute__((nonnull(arg_index)))
#else
#define ABSL_ATTRIBUTE_NONNULL(...)
#endif

#include <atomic>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#ifndef SWIG
// Disabled for SWIG as it doesn't parse attributes correctly.
namespace absl {
// Returned Status objects may not be ignored. Codesearch doesn't handle ifdefs
// as part of a class definitions (b/6995610), so we use a forward declaration.
class ABSL_MUST_USE_RESULT Status;
}  // namespace absl
#endif  // !SWIG

namespace absl {

enum class StatusCode : int;

namespace status_internal {

// Container for status payloads.
struct Payload {
    std::string type_url;
    std::string payload;
};

using Payloads = std::vector<Payload>;

// Reference-counted representation of Status data.
struct StatusRep {
    StatusRep(absl::StatusCode code_arg, std::string_view message_arg,
              std::unique_ptr<status_internal::Payloads> payloads_arg)
        : ref(int32_t{1}), code(code_arg), message(message_arg), payloads(std::move(payloads_arg)) {
    }

    std::atomic<int32_t> ref;
    absl::StatusCode code;
    std::string message;
    std::unique_ptr<status_internal::Payloads> payloads;
};

absl::StatusCode MapToLocalCode(int value);
}  // namespace status_internal

}  // namespace absl

#endif  // ABSL_STATUS_INTERNAL_STATUS_INTERNAL_H_