#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook();

    bool IsLinked() const;

    void Unlink();

    // Must unlink element from list
    ~ListHook();

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    // that helper function might be useful
    void LinkBefore(ListHook* other);
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator& operator++();
        Iterator operator++(int);

        T& operator*() const;
        T* operator->() const;

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;
    };

    List();
    List(const List&) = delete;
    List(List&& other);

    // must unlink all elements from list
    ~List();

    List& operator=(const List&) = delete;
    List& operator=(List&& other);

    bool IsEmpty() const;
    // that method is allowed to be O(n)
    size_t Size() const;

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem);
    void PushFront(T* elem);

    T& Front();
    const T& Front() const;

    T& Back();
    const T& Back() const;

    void PopBack();
    void PopFront();

    Iterator Begin();
    Iterator End();

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element);

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
