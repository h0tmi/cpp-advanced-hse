#include "cow_vector.h"
#include <iostream>

COWVector::COWVector() : data_monitor_(new DataMonitor()){};

COWVector::~COWVector() {
    --data_monitor_->ref_cnt;
    if (data_monitor_->ref_cnt == 0) {
        delete data_monitor_;
    }
};

COWVector::COWVector(const COWVector& other) {
    data_monitor_ = other.data_monitor_;
    ++data_monitor_->ref_cnt;
};

COWVector& COWVector::operator=(const COWVector& other) {
    if (data_monitor_->ref_cnt == 1) {
        data_monitor_ = other.data_monitor_;
        ++data_monitor_->ref_cnt;
    }
    --data_monitor_->ref_cnt;
    data_monitor_ = other.data_monitor_;
    ++data_monitor_->ref_cnt;
    return *this;
};

size_t COWVector::Size() const {
    return data_monitor_->data.size();
}

void COWVector::Resize(size_t size) {
    if (data_monitor_->ref_cnt == 1) {
        data_monitor_->data.resize(size);
        return;
    }

    DataMonitor* new_data_monitor = new DataMonitor();
    new_data_monitor->data = data_monitor_->data;
    new_data_monitor->data.resize(size);
    --data_monitor_->ref_cnt;
    data_monitor_ = new_data_monitor;
}

const std::string& COWVector::Get(size_t at) {
    return data_monitor_->data.at(at);
}

const std::string& COWVector::Back() {
    return data_monitor_->data.back();
}

void COWVector::PushBack(const std::string& value) {
    if (data_monitor_->ref_cnt == 1) {
        data_monitor_->data.push_back(value);
        return;
    }

    DataMonitor* new_data_monitor = new DataMonitor();
    new_data_monitor->data = data_monitor_->data;
    new_data_monitor->data.push_back(value);
    --data_monitor_->ref_cnt;
    data_monitor_ = new_data_monitor;
}

void COWVector::Set(size_t at, const std::string& value) {
    if (data_monitor_->ref_cnt == 1) {
        data_monitor_->data[at] = value;
        return;
    }

    DataMonitor* new_data_monitor = new DataMonitor();
    new_data_monitor->data = data_monitor_->data;
    new_data_monitor->data[at] = value;
    --data_monitor_->ref_cnt;
    data_monitor_ = new_data_monitor;
}
