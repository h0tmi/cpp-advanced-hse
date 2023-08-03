#pragma once

template <typename T>
class IntrusiveListItem {
public:
    ~IntrusiveListItem() {
        Unlink();
    }

    void Unlink() {
        next_->prev_ = prev_;
        prev_->next_ = next_;
        next_ = prev_ = this;
    }

    void LinkBefore(IntrusiveListItem* item) {
        Unlink();
        prev_ = item->prev_;
        next_ = item;
        prev_->next_ = this;
        next_->prev_ = this;
    }

    void LinkAfter(IntrusiveListItem* item) {
        Unlink();
        prev_ = item;
        next_ = item->next_;
        prev_->next_ = this;
        next_->prev_ = this;
    }

    T* Next() const {
        return static_cast<T*>(next_);
    }

    T* Prev() const {
        return static_cast<T*>(prev_);
    }

private:
    IntrusiveListItem* next_ = this;
    IntrusiveListItem* prev_ = this;
};

template <typename T>
class IntrusiveList {
public:
    void PushBack(T* ptr) {
        ptr->LinkBefore(&dummy_);
    }

    void PushFront(T* ptr) {
        ptr->LinkAfter(&dummy_);
    }

    T* PopFront() {
        T* ptr = dummy_.Next();
        ptr->Unlink();
        return ptr;
    }

    T* PushFront() {
        T* ptr = dummy_.Prev();
        ptr->Unlink();
        return ptr;
    }

    bool Empty() {
        return dummy_.Next() == &dummy_;
    }

private:
    IntrusiveListItem<T> dummy_;
};
