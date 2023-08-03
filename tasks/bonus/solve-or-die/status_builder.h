// Copyright 2020 The Abseil Authors.
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
//

#ifndef STATUS_BUILDER
#define STATUS_BUILDER

#include "status.h"
#include <sstream>
// #include <source_location> // Not for everyone :(

namespace absl {

class StatusBuilder;

namespace status_internal {
template <typename Fn, typename Arg, typename Expected>
inline constexpr bool kResultMatches =
    std::is_same_v<std::decay_t<std::invoke_result_t<Fn, Arg>>, Expected>;

template <typename Adaptor, typename Builder>
using PurePolicy = std::enable_if_t<kResultMatches<Adaptor, Builder, StatusBuilder>,
                                    std::invoke_result_t<Adaptor, Builder>>;

template <typename Adaptor, typename Builder>
using SideEffect = std::enable_if_t<kResultMatches<Adaptor, Builder, absl::Status>,
                                    std::invoke_result_t<Adaptor, Builder>>;

template <typename Adaptor, typename Builder>
using Conversion = std::enable_if_t<!kResultMatches<Adaptor, Builder, StatusBuilder> &&
                                        !kResultMatches<Adaptor, Builder, absl::Status>,
                                    std::invoke_result_t<Adaptor, Builder>>;
}  //  namespace status_internal

class source_location {
public:
    // Avoid this constructor; it populates the object with dummy values.
    constexpr source_location() : line_(0), file_name_(nullptr) {
    }

    // Wrapper to invoke the private constructor below. This should only be
    // used by the COURIER_LOC macro, hence the name.
    static constexpr source_location DoNotInvokeDirectly(std::uint_least32_t line,
                                                         const char* file_name) {
        return source_location(line, file_name);
    }

    // The line number of the captured source location.
    constexpr std::uint_least32_t line() const {  // NOLINT
        return line_;
    }

    // The file name of the captured source location.
    constexpr const char* file_name() const {  // NOLINT
        return file_name_;
    }

    // column() and function_name() are omitted because we don't have a
    // way to support them.

private:
    // Do not invoke this constructor directly. Instead, use the
    // COURIER_LOC macro below.
    //
    // file_name must outlive all copies of the source_location
    // object, so in practice it should be a string literal.
    constexpr source_location(std::uint_least32_t line, const char* file_name)
        : line_(line), file_name_(file_name) {
    }

    std::uint_least32_t line_;
    const char* file_name_;
};

#define ABSL_LOC absl::source_location::DoNotInvokeDirectly(__LINE__, __FILE__)

class ABSL_MUST_USE_RESULT StatusBuilder {
public:
    StatusBuilder(const StatusBuilder& sb);
    StatusBuilder& operator=(const StatusBuilder& sb);
    // Creates a `StatusBuilder` based on an original status.  If logging is
    // enabled, it will use `location` as the location from which the log message
    // occurs.  A typical user will call this with `DRISHTI_LOC`.
    StatusBuilder(const absl::Status& original_status, absl::source_location location = ABSL_LOC)
        : status_(original_status),
          line_(location.line()),
          file_(location.file_name()),
          stream_(new std::ostringstream) {
    }

    StatusBuilder(absl::Status&& original_status, absl::source_location location = ABSL_LOC)
        : status_(std::move(original_status)),
          line_(location.line()),
          file_(location.file_name()),
          stream_(new std::ostringstream) {
    }

    // Creates a `StatusBuilder` from a drishti status code.  If logging is
    // enabled, it will use `location` as the location from which the log message
    // occurs.  A typical user will call this with `DRISHTI_LOC`.
    StatusBuilder(absl::StatusCode code, absl::source_location location = ABSL_LOC)
        : status_(code, ""),
          line_(location.line()),
          file_(location.file_name()),
          stream_(new std::ostringstream) {
    }

    StatusBuilder(const absl::Status& original_status, const char* file, int line)
        : status_(original_status), line_(line), file_(file), stream_(new std::ostringstream) {
    }

    bool ok() const {  // NOLINT
        return status_.ok();
    }

    template <typename Adaptor>
    auto With(Adaptor&& adaptor) & -> status_internal::PurePolicy<Adaptor, StatusBuilder&> {
        return std::forward<Adaptor>(adaptor)(*this);
    }
    template <typename Adaptor>
    ABSL_MUST_USE_RESULT auto With(
        Adaptor&& adaptor) && -> status_internal::PurePolicy<Adaptor, StatusBuilder&&> {
        return std::forward<Adaptor>(adaptor)(std::move(*this));
    }

    template <typename Adaptor>
    auto With(Adaptor&& adaptor) & -> status_internal::SideEffect<Adaptor, StatusBuilder&> {
        return std::forward<Adaptor>(adaptor)(*this);
    }
    template <typename Adaptor>
    ABSL_MUST_USE_RESULT auto With(
        Adaptor&& adaptor) && -> status_internal::SideEffect<Adaptor, StatusBuilder&&> {
        return std::forward<Adaptor>(adaptor)(std::move(*this));
    }

    template <typename Adaptor>
    auto With(Adaptor&& adaptor) & -> status_internal::Conversion<Adaptor, StatusBuilder&> {
        return std::forward<Adaptor>(adaptor)(*this);
    }
    template <typename Adaptor>
    ABSL_MUST_USE_RESULT auto With(
        Adaptor&& adaptor) && -> status_internal::Conversion<Adaptor, StatusBuilder&&> {
        return std::forward<Adaptor>(adaptor)(std::move(*this));
    }

    StatusBuilder& SetAppend();

    StatusBuilder& SetPrepend();

    StatusBuilder& SetNoLogging();

    template <typename T>
    StatusBuilder& operator<<(const T& msg) {
        if (status_.ok()) {
            return *this;
        }
        *stream_ << msg;
        return *this;
    }

    operator absl::Status() const&;
    operator absl::Status() &&;

    absl::Status JoinMessageToStatus();

private:
    // Specifies how to join the error message in the original status and any
    // additional message that has been streamed into the builder.
    enum class MessageJoinStyle {
        kAnnotate,
        kAppend,
        kPrepend,
    };

    // The status that the result will be based on.
    absl::Status status_;
    // The line to record if this file is logged.
    int line_;
    // Not-owned: The file to record if this status is logged.
    const char* file_;
    bool no_logging_ = false;
    // The additional messages added with `<<`.
    std::unique_ptr<std::ostringstream> stream_;
    // Specifies how to join the message in `status_` and `stream_`.
    MessageJoinStyle join_style_ = MessageJoinStyle::kAnnotate;
};

inline StatusBuilder AlreadyExistsErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kAlreadyExists, location);
}

inline StatusBuilder FailedPreconditionErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kFailedPrecondition, location);
}

inline StatusBuilder InternalErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kInternal, location);
}

inline StatusBuilder InvalidArgumentErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kInvalidArgument, location);
}

inline StatusBuilder NotFoundErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kNotFound, location);
}

inline StatusBuilder UnavailableErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kUnavailable, location);
}

inline StatusBuilder UnimplementedErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kUnimplemented, location);
}

inline StatusBuilder UnknownErrorBuilder(absl::source_location location) {
    return StatusBuilder(absl::StatusCode::kUnknown, location);
}

}  // namespace absl

#endif  // STATUS_BUILDER