#include "harakiri.h"
#include <iterator>
#include <memory>

AwesomeCallback::AwesomeCallback(const std::string& s) {
    data_ = new std::string(s);
};
AwesomeCallback::AwesomeCallback(AwesomeCallback&& other) {
    *data_ = std::move(*(other.data_));
};
AwesomeCallback& AwesomeCallback::operator=(AwesomeCallback&& other) {
    *data_ = std::move(*(other.data_));
    return *this;
};
AwesomeCallback::~AwesomeCallback() {
    delete data_;
}

std::string AwesomeCallback::operator()() const&& {
    std::string s = *data_ + "awesomeness";
    delete this;
    return s;
};
