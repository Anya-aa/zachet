#pragma once
#include <cstdlib>
#include <iostream>
#include <cassert>
template <class ValueType>
class SinglyLinkedList
{
public:
    typedef int size_type;
    typedef ValueType value_type;
    struct Node{
    public:
        Node();
        explicit Node(const ValueType& value, Node* next = nullptr);
        ~Node();

        void insertNext(const ValueType& value);
        void removeNext();

        ValueType value;
        Node* next;
        void forceNodeDelete(Node* node);
    };

    class iterator
    {
        friend class SinglyLinkedList<ValueType>;

    public:

        typedef iterator self_type;
        typedef ValueType value_type;
        typedef ValueType& reference;
        typedef ValueType* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;

        iterator(Node *n)
        {
            node = n;
        }
        iterator(const iterator &it)
        {
            node = it.node;
        }

        iterator& operator=(const iterator &it)
        {
            node = it.node;
            return *this;
        }
        bool operator == (const iterator &it) const
        {
            return (node == it.node);
        }


        bool operator!=(const iterator &it) const
        {
            return !(it == *this);
        }

        //переводит итератор на следующий узел списка.
        iterator& operator++()
        {
            if (node == nullptr)
                throw "incremented an empty iterator";

            node = node->next;
            return *this;
        }

        ValueType& operator*() const
        {
            if (node == nullptr)
                throw "tried to dereference an empty iterator";
            return node->value;
        }

    private:
        Node *node;


    };

private:
    Node* _firstNode;
    size_t	_size;

public:
    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList& copyList);
    SinglyLinkedList& operator=(const SinglyLinkedList& copyList);

    SinglyLinkedList(SinglyLinkedList&& moveList) noexcept;
    SinglyLinkedList& operator=(SinglyLinkedList&& moveList) noexcept;

    ~SinglyLinkedList();
    ValueType& operator[](const size_t pos) const;
    Node* getNode(const size_t pos) const;
    void insert(const size_t pos, const ValueType& value);
    void push_back(const ValueType& value);
    void push_front(const ValueType& value);

    void remove(const size_t pos);
    void removeNextNode(Node* node);
    void removeFront();
    void removeBack();

    iterator begin()
    {
        return iterator(_firstNode);
    }
    iterator end()
    {
        return iterator(nullptr);
    }


    size_t size_list() const;

    void print(); // этот метод нужен исключительно для отладки

};

template<class ValueType>
SinglyLinkedList<ValueType>::Node::Node()
{
    this->value = ValueType();
    this->next = nullptr;
}

template<class ValueType>
SinglyLinkedList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

template<class ValueType>
SinglyLinkedList<ValueType>::Node::~Node() {}

template<class ValueType>
void SinglyLinkedList<ValueType>::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;
}

template<class ValueType>
void SinglyLinkedList<ValueType>::Node::removeNext()
{
    Node* removeNode = this->next;
    Node* newNext = removeNode->next;
    delete removeNode;
    this->next = newNext;
}

template<class ValueType>
void SinglyLinkedList<ValueType>::Node::forceNodeDelete(Node *node)
{
    if (node == nullptr) {
        return;
    }
    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

template<class ValueType>
SinglyLinkedList<ValueType>::SinglyLinkedList()
{
    _firstNode = nullptr;
    this->_size = 0;
}

template<class ValueType>
SinglyLinkedList<ValueType>::SinglyLinkedList(const SinglyLinkedList& copyList)
{
    this->_size = copyList._size;
    this->_firstNode = new Node(copyList._firstNode->value);

    Node* currentNode = this->_firstNode;
    Node* currentCopyNode = copyList._firstNode;

    while (currentCopyNode->next) {
        currentCopyNode = currentCopyNode->next;
        currentNode->next = new Node (currentCopyNode->value);
        currentNode = currentNode->next;
    }
}

template<class ValueType>
SinglyLinkedList<ValueType>& SinglyLinkedList<ValueType>:: operator=(const SinglyLinkedList& copyList)
{
    if (this == &copyList) {
        return *this;
    }
    SinglyLinkedList bufList(copyList);
    this->_size = bufList._size;
    if (this->_firstNode != nullptr)
    {
        _firstNode->forceNodeDelete(_firstNode);
    }
    this->_firstNode = new Node(bufList._firstNode->value);
    Node* currentNode = this->_firstNode;
    Node* currentCopyNode = copyList._firstNode;

    while (currentCopyNode->next) {
        currentCopyNode = currentCopyNode->next;
        currentNode->next = new Node (currentCopyNode->value);
        currentNode = currentNode->next;
    }


    return *this;
}

template<class ValueType>
SinglyLinkedList<ValueType>::SinglyLinkedList(SinglyLinkedList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_firstNode = moveList._firstNode;

    moveList._size = 0;
    moveList._firstNode = nullptr;
}

template<class ValueType>
SinglyLinkedList<ValueType>& SinglyLinkedList<ValueType>::operator=(SinglyLinkedList&& moveList) noexcept
{
    if (this == &moveList) {
        return *this;
    }
    _firstNode->forceNodeDelete(_firstNode);
    this->_size = moveList._size;
    this->_firstNode = moveList._firstNode;

    moveList._size = 0;
    moveList._firstNode = nullptr;

    return *this;
}

template<class ValueType>
SinglyLinkedList<ValueType>:: ~SinglyLinkedList()
{
    _firstNode->forceNodeDelete(this->_firstNode);
}

template<class ValueType>
ValueType& SinglyLinkedList<ValueType>:: operator[](const size_t pos) const
{
    return getNode(pos)->value;
}

template<class ValueType>
class SinglyLinkedList<ValueType>::Node* SinglyLinkedList<ValueType>::getNode(const size_t pos) const
{
    if (pos < 0) {
        throw std::out_of_range ("pos < 0");

    }
    else if (pos >= this->_size) {
        throw std::out_of_range ("pos >= this->_size");
    }

    Node* bufNode = this->_firstNode;
    for (int i = 0; i < pos; ++i) {
        bufNode = bufNode->next;
    }

    return bufNode;
}

template<class ValueType>
void SinglyLinkedList<ValueType>::insert(const size_t pos, const ValueType& value)
{
    if (pos < 0) {
        throw std::out_of_range ("pos < 0");
    }
    else if (pos > this->_size) {
        throw std::out_of_range ("pos > this->_size");
    }

    if (pos == 0) {
        push_front(value);
    }
    else{
        Node* bufNode = this->_firstNode;
        for (size_t i = 0; i < pos - 1; i++) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(value);
        _size++;
    }
}

template<class ValueType>
void SinglyLinkedList<ValueType>::push_back(const ValueType& value)
{
    if (_size == 0)
    {
        push_front(value);
        return;
    }
    else
    {
        insert(_size, value);
    }
}

template<class ValueType>
void SinglyLinkedList<ValueType>::push_front(const ValueType& value)
{
    _firstNode = new Node(value, nullptr);
    ++_size;
}

template<class ValueType>
void SinglyLinkedList<ValueType>:: remove(const size_t pos)
{
    if (pos < 0)
        throw std::out_of_range("pos< 0");

    if (pos > this->_size)
        throw std::out_of_range ("pos > this->_size");
    if (pos == 0) {
        removeFront();
    }
    else {
        Node* node = getNode(pos - 1);
        removeNextNode(node);
    }
}
template <class ValueType>
void SinglyLinkedList<ValueType>:: removeNextNode(Node* node)
{
    Node *deletedNode = node->next;
    node->next = node->next->next;
    delete deletedNode;
    _size--;
}

template<class ValueType>
void SinglyLinkedList<ValueType>::removeFront()
{
    if (_size == 0)
    {
        throw std::runtime_error("list is empty");
    }
    Node *node = _firstNode;
    this->_firstNode = this->_firstNode->next;
    delete node;
    this->_size--;
}

template<class ValueType>
void SinglyLinkedList<ValueType>:: removeBack()
{
    if (_size == 0)
    {
        throw std::runtime_error("list is empty");
    }
    if (_size == 1)
    {
        delete _firstNode;
        _firstNode = nullptr;
        _size--;
    }
    else
    {
        Node* node = _firstNode;
        while(node->next->next)
        {
            node = node->next;
        }
        delete node->next;
        node->next = nullptr;
        _size--;
    }
}


template<class ValueType>
size_t SinglyLinkedList<ValueType> :: size_list() const
{
    return _size;
}

template<class ValueType>
void SinglyLinkedList<ValueType>::print()
{
    Node* node = _firstNode;
    while(node->next != nullptr)
    {
        std::cout << node->value << std::endl;
        node = node->next;
    }
    std::cout << node->value<< std::endl;
}

