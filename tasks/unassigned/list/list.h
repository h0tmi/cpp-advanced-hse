#pragma once

template <typename T>
class List {
public:
    class Iterator {
    public:
        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        T& operator*() const;
        T* operator->() const;

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;
    };

    List();
    List(const List&);
    List(List&&);
    ~List();

    List& operator=(const List&);
    List& operator=(List&&);

    bool IsEmpty() const;
    size_t Size() const;

    void PushBack(const T& elem);
    void PushBack(T&& elem);
    void PushFront(const T& elem);
    void PushFront(T&& elem);

    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;

    void PopBack();
    void PopFront();

    Iterator Begin();
    Iterator End();
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list);

template <typename T>
typename List<T>::Iterator end(List<T>& list);
