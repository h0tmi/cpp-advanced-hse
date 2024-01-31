#pragma once

#include <algorithm>
#include <type_traits>

class ListHook {
public:
    ListHook() {
        front_link_ = nullptr;
        back_link_ = nullptr;
    };

    bool IsLinked() const {
        return front_link_;
    };

    void Unlink() {
        if (front_link_) {
            Linker(front_link_, back_link_);
            front_link_ = back_link_ = nullptr;
        }
    };

    // Must unlink element from list
    ~ListHook() {
        Unlink();
    };

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    static void Linker(ListHook* first, ListHook* second) {
        first->back_link_ = second;
        second->front_link_ = first;
    };
    // that helper function might be useful
    void LinkBefore(ListHook* other) {
        Linker(other, back_link_);
        Linker(this, other);
    };

private:
    ListHook* front_link_;
    ListHook* back_link_;
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(T* other) {
            data_ = other;
        }

        Iterator& operator++() {
            data_ = reinterpret_cast<T*>(data_->front_link_);
            return *this;
        };

        Iterator operator++(int) {
            auto copy = *this;
            ++*this;
            return copy;
        };

        T& operator*() const {
            return *data_;
        };
        T* operator->() const {
            return data_;
        };

        bool operator==(const Iterator& rhs) const {
            return data_ == rhs.data_;
        };
        bool operator!=(const Iterator& rhs) const {
            return !(*this == rhs);
        };

    private:
        T* data_;
    };

    List() {
        dummy_ = new ListHook();
        ListHook::Linker(dummy_, dummy_);
    };
    List(const List&) = delete;
    List(List&& other) {
        dummy_ = new ListHook();
        ListHook::Linker(dummy_, dummy_);
        std::swap(dummy_, other.dummy_);
    };

    // must unlink all elements from list
    ~List() {
        while (!IsEmpty()) {
            dummy_->front_link_->Unlink();
        }
        delete dummy_;
    };

    List& operator=(const List&) = delete;
    List& operator=(List&& other) {
        while (!IsEmpty()) {
            dummy_->front_link_->Unlink();
        }
        std::swap(dummy_, other.dummy_);
        return *this;
    };

    bool IsEmpty() const {
        return Begin() == End();
    };
    // that method is allowed to be O(n)
    size_t Size() const {
        size_t size = 0;

        for (auto i = Begin(); i != End(); ++i) {
            ++size;
        }

        return size;
    };

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        dummy_->LinkBefore(elem);
    };
    void PushFront(T* elem) {
        dummy_->front_link_->LinkBefore(elem);
    };

    T& Front() {
        return *reinterpret_cast<T*>(dummy_->front_link_);
    };
    const T& Front() const {
        return *reinterpret_cast<T*>(dummy_->front_link_);
    };

    T& Back() {
        return *reinterpret_cast<T*>(dummy_->back_link_);
    };
    const T& Back() const {
        return *reinterpret_cast<T*>(dummy_->back_link_);
    };

    void PopBack() {
        auto popped_element = dummy_->back_link_;
        ListHook::Linker(dummy_->back_link_->back_link_, dummy_);
        popped_element->Unlink();
    };
    void PopFront() {
        auto popped_element = dummy_->front_link_;
        ListHook::Linker(dummy_, dummy_->front_link_->front_link_);
        popped_element->Unlink();
    };

    Iterator Begin() const {
        return ++Iterator(reinterpret_cast<T*>(dummy_));
    };
    Iterator End() const {
        return Iterator(reinterpret_cast<T*>(dummy_));
    };

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        return Iterator(element);
    };

private:
    ListHook* dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
