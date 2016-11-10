#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

    template<typename Type>
    class LinkedList {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;

        friend class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        struct Node;
        struct NodeExtended;

        LinkedList() : mCount(0) {
            mHead = new Node(nullptr, nullptr);
            mTail = new Node(nullptr, nullptr);
            mHead->next = mTail;
            mTail->prev = mHead;
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList() {
            for (auto&& element : l)
                append(std::forward<decltype(element)>(element));
            mCount = l.size();
        }

        LinkedList(const LinkedList& other) : LinkedList() {
            for (auto&& element : other)
                append(element);
            mCount = other.mCount;
        }

        LinkedList(LinkedList&& other) : LinkedList() {
            Node* head = mHead;
            Node* tail = mTail;
            mHead = std::move(other.mHead);
            mTail = std::move(other.mTail);
            mCount = std::move(other.mCount);
            other.mHead = head;
            other.mTail = tail;
            other.mCount = 0;
        }

        ~LinkedList() {
            clear();
            delete mHead;
            delete mTail;
        }

        LinkedList& operator=(const LinkedList& other) {
            clear();
            for (auto&& element : other)
                append(element);
            mCount = other.mCount;
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) {
            clear();
            Node* head = mHead;
            Node* tail = mTail;
            mHead = std::move(other.mHead);
            mTail = std::move(other.mTail);
            mCount = std::move(other.mCount);
            other.mHead = head;
            other.mTail = tail;
            other.mCount = 0;
            return *this;
        }

        bool isEmpty() const {
            return (mHead->next == mTail);
        }

        size_type getSize() const {
            return mCount;
        }

        void append(const Type& item) {
            insert(end(), item);
        }

        void prepend(const Type& item) {
            insert(begin(), item);
        }

        void insert(const const_iterator& insertPosition, const Type& item) {
            Node* before = insertPosition.mNode;
            Node* node = new NodeExtended(before->prev, before, item);
            before->prev->next = node;
            before->prev = node;
            ++mCount;
        }

        Type popFirst() {
            if (mHead->next == mTail)
                throw std::out_of_range("Popping from empty list");

            Type ret = dynamic_cast<NodeExtended*>(mHead->next)->data;
            erase(begin());
            return ret;
        }

        Type popLast() {
            if (mHead->next == mTail)
                throw std::out_of_range("Popping from empty list");

            Type ret = dynamic_cast<NodeExtended*>(mTail->prev)->data;
            erase(--end());
            return ret;
        }

        void erase(const const_iterator& possition) {
            Node* node = possition.mNode;
            if (node == mTail)
                throw std::out_of_range("Removing end of list");
            node->prev->next = node->next;
            node->next->prev = node->prev;
            delete node;
            --mCount;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            Node* first = firstIncluded.mNode;
            Node* last = lastExcluded.mNode;
            if (first == last)
                return;

            first->prev->next = last;
            last->prev = first->prev;

            Node* node;
            while (first != last) {
                node = first;
                first = first->next;
                delete node;
                --mCount;
            }
        }

        iterator begin() {
            return Iterator(*this, mHead->next);
        }

        iterator end() {
            return Iterator(*this, mTail);
        }

        const_iterator cbegin() const {
            return ConstIterator(*this, mHead->next);
        }

        const_iterator cend() const {
            return ConstIterator(*this, mTail);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        Node* mHead;
        Node* mTail;
        std::size_t mCount;

        void clear() {
            if (mCount == 0) return;
            Node* node = mHead->next;
            while (node != mTail) {
                Node* tmp = node;
                node = node->next;
                delete tmp;
            }
            mHead->next = mTail;
            mTail->prev = mHead;
            mCount = 0;
        }
    };

    template<typename Type>
    struct LinkedList<Type>::Node {
        Node* prev;
        Node* next;

        Node(Node* pPrev, Node* pNext) : prev(pPrev), next(pNext) { };

        virtual ~Node() { }
    };

    template<typename Type>
    struct LinkedList<Type>::NodeExtended : public LinkedList<Type>::Node {
        Type data;

        NodeExtended(Node* pPrev, Node* pNext, const Type& pData) : Node(pPrev, pNext), data(pData) { };

        NodeExtended(Node* pPrev, Node* pNext, Type&& pData) : Node(pPrev, pNext), data(std::move(pData)) { };

        virtual ~NodeExtended() { }
    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        friend class LinkedList<Type>;

        explicit ConstIterator(const LinkedList& pList, Node* pNode) : mList(pList), mNode(pNode) { }

        reference operator*() const {
            if (mNode == mList.mTail || mNode == mList.mHead)
                throw std::out_of_range("Dereferencing invalid iterator");
            return dynamic_cast<NodeExtended*>(mNode)->data;
        }

        ConstIterator& operator++() {
            if (mNode == mList.mTail)
                throw std::out_of_range("Trying to increment end iterator");
            mNode = mNode->next;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator copy(*this);
            operator++();
            return copy;
        }

        ConstIterator& operator--() {
            if (mNode->prev == mList.mHead)
                throw std::out_of_range("Trying to decrement end iterator");
            mNode = mNode->prev;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator copy(*this);
            operator--();
            return copy;
        }

        ConstIterator operator+(difference_type d) const {
            Node* node = mNode;
            difference_type offset = 0;
            while (node != mList.mTail && offset - d < 0) {
                node = node->next;
                offset++;
            }
            return ConstIterator(mList, node);
        }

        ConstIterator operator-(difference_type d) const {
            Node* node = mNode;
            difference_type offset = 0;
            while (node != mList.mHead && offset - d < 0) {
                node = node->prev;
                offset++;
            }
            return ConstIterator(mList, node);
        }

        bool operator==(const ConstIterator& other) const {
            return mNode == other.mNode;
        }

        bool operator!=(const ConstIterator& other) const {
            return !operator==(other);
        }

    protected:
        const LinkedList& mList;
        Node* mNode;
    };

    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(const LinkedList& pList, Node* pNode) : ConstIterator(pList, pNode) { }

        Iterator(const ConstIterator& other) : ConstIterator(other) { }

        Iterator& operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
