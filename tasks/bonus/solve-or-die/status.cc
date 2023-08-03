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
#include "status.h"
#include "status_payload_printer.h"

#include <cassert>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

namespace absl {

std::string StatusCodeToString(StatusCode code) {
    switch (code) {
        case StatusCode::kOk:
            return "OK";
        case StatusCode::kCancelled:
            return "CANCELLED";
        case StatusCode::kUnknown:
            return "UNKNOWN";
        case StatusCode::kInvalidArgument:
            return "INVALID_ARGUMENT";
        case StatusCode::kDeadlineExceeded:
            return "DEADLINE_EXCEEDED";
        case StatusCode::kNotFound:
            return "NOT_FOUND";
        case StatusCode::kAlreadyExists:
            return "ALREADY_EXISTS";
        case StatusCode::kPermissionDenied:
            return "PERMISSION_DENIED";
        case StatusCode::kUnauthenticated:
            return "UNAUTHENTICATED";
        case StatusCode::kResourceExhausted:
            return "RESOURCE_EXHAUSTED";
        case StatusCode::kFailedPrecondition:
            return "FAILED_PRECONDITION";
        case StatusCode::kAborted:
            return "ABORTED";
        case StatusCode::kOutOfRange:
            return "OUT_OF_RANGE";
        case StatusCode::kUnimplemented:
            return "UNIMPLEMENTED";
        case StatusCode::kInternal:
            return "INTERNAL";
        case StatusCode::kUnavailable:
            return "UNAVAILABLE";
        case StatusCode::kDataLoss:
            return "DATA_LOSS";
        default:
            return "";
    }
}

std::ostream& operator<<(std::ostream& os, StatusCode code) {
    return os << StatusCodeToString(code);
}

namespace status_internal {

static int FindPayloadIndexByUrl(const Payloads* payloads, std::string_view type_url) {
    if (payloads == nullptr)
        return -1;

    for (size_t i = 0; i < payloads->size(); ++i) {
        if ((*payloads)[i].type_url == type_url)
            return i;
    }

    return -1;
}

// Convert canonical code to a value known to this binary.
absl::StatusCode MapToLocalCode(int value) {
    absl::StatusCode code = static_cast<absl::StatusCode>(value);
    switch (code) {
        case absl::StatusCode::kOk:
        case absl::StatusCode::kCancelled:
        case absl::StatusCode::kUnknown:
        case absl::StatusCode::kInvalidArgument:
        case absl::StatusCode::kDeadlineExceeded:
        case absl::StatusCode::kNotFound:
        case absl::StatusCode::kAlreadyExists:
        case absl::StatusCode::kPermissionDenied:
        case absl::StatusCode::kResourceExhausted:
        case absl::StatusCode::kFailedPrecondition:
        case absl::StatusCode::kAborted:
        case absl::StatusCode::kOutOfRange:
        case absl::StatusCode::kUnimplemented:
        case absl::StatusCode::kInternal:
        case absl::StatusCode::kUnavailable:
        case absl::StatusCode::kDataLoss:
        case absl::StatusCode::kUnauthenticated:
            return code;
        default:
            return absl::StatusCode::kUnknown;
    }
}
}  // namespace status_internal

std::optional<std::string> Status::GetPayload(std::string_view type_url) const {
    const auto* payloads = GetPayloads();
    int index = status_internal::FindPayloadIndexByUrl(payloads, type_url);
    if (index != -1)
        return (*payloads)[index].payload;

    return std::nullopt;
}

void Status::SetPayload(std::string_view type_url, std::string payload) {
    if (ok())
        return;

    PrepareToModify();

    status_internal::StatusRep* rep = RepToPointer(rep_);
    if (!rep->payloads) {
        rep->payloads = std::make_unique<status_internal::Payloads>();
    }

    int index = status_internal::FindPayloadIndexByUrl(rep->payloads.get(), type_url);
    if (index != -1) {
        (*rep->payloads)[index].payload = std::move(payload);
        return;
    }

    rep->payloads->push_back({std::string(type_url), std::move(payload)});
}

bool Status::ErasePayload(std::string_view type_url) {
    int index = status_internal::FindPayloadIndexByUrl(GetPayloads(), type_url);
    if (index != -1) {
        PrepareToModify();
        GetPayloads()->erase(GetPayloads()->begin() + index);
        if (GetPayloads()->empty() && message().empty()) {
            // Special case: If this can be represented inlined, it MUST be
            // inlined (EqualsSlow depends on this behavior).
            StatusCode c = static_cast<StatusCode>(raw_code());
            Unref(rep_);
            rep_ = CodeToInlinedRep(c);
        }
        return true;
    }

    return false;
}

void Status::ForEachPayload(
    const std::function<void(std::string_view, const std::string&)>& visitor) const {
    if (auto* payloads = GetPayloads()) {
        bool in_reverse = payloads->size() > 1 && reinterpret_cast<uintptr_t>(payloads) % 13 > 6;

        for (size_t index = 0; index < payloads->size(); ++index) {
            const auto& elem = (*payloads)[in_reverse ? payloads->size() - 1 - index : index];

#ifdef NDEBUG
            visitor(elem.type_url, elem.payload);
#else
            // In debug mode invalidate the type url to prevent users from relying on
            // this string lifetime.

            // NOLINTNEXTLINE intentional extra conversion to force temporary.
            visitor(std::string(elem.type_url), elem.payload);
#endif  // NDEBUG
        }
    }
}

const std::string* Status::EmptyString() {
    static std::string* empty_string = new std::string();
    return empty_string;
}

constexpr const char Status::kMovedFromString[];

const std::string* Status::MovedFromString() {
    static std::string* moved_from_string = new std::string(kMovedFromString);
    return moved_from_string;
}

void Status::UnrefNonInlined(uintptr_t rep) {
    status_internal::StatusRep* r = RepToPointer(rep);
    // Fast path: if ref==1, there is no need for a RefCountDec (since
    // this is the only reference and therefore no other thread is
    // allowed to be mucking with r).
    if (r->ref.load(std::memory_order_acquire) == 1 ||
        r->ref.fetch_sub(1, std::memory_order_acq_rel) - 1 == 0) {
        delete r;
    }
}

Status::Status(absl::StatusCode code, std::string_view msg) : rep_(CodeToInlinedRep(code)) {
    if (code != absl::StatusCode::kOk && !msg.empty()) {
        rep_ = PointerToRep(new status_internal::StatusRep(code, msg, nullptr));
    }
}

int Status::raw_code() const {
    if (IsInlined(rep_)) {
        return static_cast<int>(InlinedRepToCode(rep_));
    }
    status_internal::StatusRep* rep = RepToPointer(rep_);
    return static_cast<int>(rep->code);
}

absl::StatusCode Status::code() const {
    return status_internal::MapToLocalCode(raw_code());
}

void Status::PrepareToModify() {
    // ABSL_RAW_CHECK(!ok(), "PrepareToModify shouldn't be called on OK status.");
    if (IsInlined(rep_)) {
        rep_ = PointerToRep(new status_internal::StatusRep(
            static_cast<absl::StatusCode>(raw_code()), std::string_view(), nullptr));
        return;
    }

    uintptr_t rep_i = rep_;
    status_internal::StatusRep* rep = RepToPointer(rep_);
    if (rep->ref.load(std::memory_order_acquire) != 1) {
        std::unique_ptr<status_internal::Payloads> payloads;
        if (rep->payloads) {
            payloads = std::make_unique<status_internal::Payloads>(*rep->payloads);
        }
        status_internal::StatusRep* const new_rep =
            new status_internal::StatusRep(rep->code, message(), std::move(payloads));
        rep_ = PointerToRep(new_rep);
        UnrefNonInlined(rep_i);
    }
}

bool Status::EqualsSlow(const absl::Status& a, const absl::Status& b) {
    if (IsInlined(a.rep_) != IsInlined(b.rep_))
        return false;
    if (a.message() != b.message())
        return false;
    if (a.raw_code() != b.raw_code())
        return false;
    if (a.GetPayloads() == b.GetPayloads())
        return true;

    const status_internal::Payloads no_payloads;
    const status_internal::Payloads* larger_payloads =
        a.GetPayloads() ? a.GetPayloads() : &no_payloads;
    const status_internal::Payloads* smaller_payloads =
        b.GetPayloads() ? b.GetPayloads() : &no_payloads;
    if (larger_payloads->size() < smaller_payloads->size()) {
        std::swap(larger_payloads, smaller_payloads);
    }
    if ((larger_payloads->size() - smaller_payloads->size()) > 1)
        return false;
    // Payloads can be ordered differently, so we can't just compare payload
    // vectors.
    for (const auto& payload : *larger_payloads) {

        bool found = false;
        for (const auto& other_payload : *smaller_payloads) {
            if (payload.type_url == other_payload.type_url) {
                if (payload.payload != other_payload.payload) {
                    return false;
                }
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

std::string Status::ToStringSlow(StatusToStringMode mode) const {
    std::string text;
    text += absl::StatusCodeToString(code()) + ": " + std::string(message());

    const bool with_payload =
        (mode & StatusToStringMode::kWithPayload) == StatusToStringMode::kWithPayload;

    if (with_payload) {
        status_internal::StatusPayloadPrinter printer = status_internal::GetStatusPayloadPrinter();
        this->ForEachPayload([&](std::string_view type_url, const std::string& payload) {
            std::optional<std::string> result;
            if (printer)
                result = printer(type_url, payload);
            text += " [";
            text += type_url;
            text += "='";
            text += result.has_value() ? *result : std::string(payload);
            text += "']";
        });
    }

    return text;
}

std::ostream& operator<<(std::ostream& os, const Status& x) {
    os << x.ToString(StatusToStringMode::kWithEverything);
    return os;
}

Status AbortedError(std::string_view message) {
    return Status(absl::StatusCode::kAborted, message);
}

Status AlreadyExistsError(std::string_view message) {
    return Status(absl::StatusCode::kAlreadyExists, message);
}

Status CancelledError(std::string_view message) {
    return Status(absl::StatusCode::kCancelled, message);
}

Status DataLossError(std::string_view message) {
    return Status(absl::StatusCode::kDataLoss, message);
}

Status DeadlineExceededError(std::string_view message) {
    return Status(absl::StatusCode::kDeadlineExceeded, message);
}

Status FailedPreconditionError(std::string_view message) {
    return Status(absl::StatusCode::kFailedPrecondition, message);
}

Status InternalError(std::string_view message) {
    return Status(absl::StatusCode::kInternal, message);
}

Status InvalidArgumentError(std::string_view message) {
    return Status(absl::StatusCode::kInvalidArgument, message);
}

Status NotFoundError(std::string_view message) {
    return Status(absl::StatusCode::kNotFound, message);
}

Status OutOfRangeError(std::string_view message) {
    return Status(absl::StatusCode::kOutOfRange, message);
}

Status PermissionDeniedError(std::string_view message) {
    return Status(absl::StatusCode::kPermissionDenied, message);
}

Status ResourceExhaustedError(std::string_view message) {
    return Status(absl::StatusCode::kResourceExhausted, message);
}

Status UnauthenticatedError(std::string_view message) {
    return Status(absl::StatusCode::kUnauthenticated, message);
}

Status UnavailableError(std::string_view message) {
    return Status(absl::StatusCode::kUnavailable, message);
}

Status UnimplementedError(std::string_view message) {
    return Status(absl::StatusCode::kUnimplemented, message);
}

Status UnknownError(std::string_view message) {
    return Status(absl::StatusCode::kUnknown, message);
}

bool IsAborted(const Status& status) {
    return status.code() == absl::StatusCode::kAborted;
}

bool IsAlreadyExists(const Status& status) {
    return status.code() == absl::StatusCode::kAlreadyExists;
}

bool IsCancelled(const Status& status) {
    return status.code() == absl::StatusCode::kCancelled;
}

bool IsDataLoss(const Status& status) {
    return status.code() == absl::StatusCode::kDataLoss;
}

bool IsDeadlineExceeded(const Status& status) {
    return status.code() == absl::StatusCode::kDeadlineExceeded;
}

bool IsFailedPrecondition(const Status& status) {
    return status.code() == absl::StatusCode::kFailedPrecondition;
}

bool IsInternal(const Status& status) {
    return status.code() == absl::StatusCode::kInternal;
}

bool IsInvalidArgument(const Status& status) {
    return status.code() == absl::StatusCode::kInvalidArgument;
}

bool IsNotFound(const Status& status) {
    return status.code() == absl::StatusCode::kNotFound;
}

bool IsOutOfRange(const Status& status) {
    return status.code() == absl::StatusCode::kOutOfRange;
}

bool IsPermissionDenied(const Status& status) {
    return status.code() == absl::StatusCode::kPermissionDenied;
}

bool IsResourceExhausted(const Status& status) {
    return status.code() == absl::StatusCode::kResourceExhausted;
}

bool IsUnauthenticated(const Status& status) {
    return status.code() == absl::StatusCode::kUnauthenticated;
}

bool IsUnavailable(const Status& status) {
    return status.code() == absl::StatusCode::kUnavailable;
}

bool IsUnimplemented(const Status& status) {
    return status.code() == absl::StatusCode::kUnimplemented;
}

bool IsUnknown(const Status& status) {
    return status.code() == absl::StatusCode::kUnknown;
}

}  // namespace absl