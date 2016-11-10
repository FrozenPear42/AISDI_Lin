#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

    template<typename Type>
    class Vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        Vector() : mCapacity(INIT_CAPACITY), mCount(0), mData(new Type[INIT_CAPACITY]) { }

        Vector(std::initializer_list<Type> l) : mCapacity(l.size() * 2), mCount(l.size()),
                                                mData(new Type[mCapacity]) {
            std::size_t idx = 0;
            for (auto&& elem : l)
                mData[idx++] = std::move(elem);
        }

        Vector(const Vector& other) : mCapacity(other.mCapacity), mCount(other.mCount), mData(new Type[mCapacity]) {
            for (std::size_t idx = 0; idx < other.mCount; ++idx)
                mData[idx] = other.mData[idx];
        }

        Vector(Vector&& other) : Vector() {
            std::size_t capacity = mCapacity;
            std::size_t count = mCount;
            Type* data = mData;
            mCapacity = std::move(other.mCapacity);
            mCount = std::move(other.mCount);
            mData = std::move(other.mData);
            other.mCapacity = capacity;
            other.mCount = count;
            other.mData = data;
        }

        ~Vector() {
            delete[] mData;
        }

        Vector& operator=(const Vector& other) {
            if (mCapacity <= other.mCapacity)
                realocate(other.mCapacity);
            for (std::size_t idx = 0; idx < other.mCapacity; ++idx)
                mData[idx] = other.mData[idx];
            mCount = other.mCount;
            return *this;
        }

        Vector& operator=(Vector&& other) {
            Type* data = mData;
            mCapacity = std::move(other.mCapacity);
            mCount = std::move(other.mCount);
            mData = std::move(other.mData);
            other.mCapacity = INIT_CAPACITY;
            other.mCount = 0;
            other.mData = data;
            return *this;
        }

        bool isEmpty() const {
            return mCount == 0;
        }

        size_type getSize() const {
            return mCount;
        }

        void append(const Type& item) {
            insert_at(item, mCount);
        }

        void prepend(const Type& item) {
            insert_at(item, 0);
        }

        void insert(const const_iterator& insertPosition, const Type& item) {
            insert_at(item, insertPosition.mIndex);
        }

        Type popFirst() {
            if (mCount == 0) throw std::out_of_range("Can not popFirst, vector is empty");
            Type item = std::move(mData[0]);
            erase_at(0);
            return item;
        }

        Type popLast() {
            if (mCount == 0) throw std::out_of_range("Can not popLast, vector is empty");
            Type item = std::move(mData[mCount - 1]);
            erase_at(mCount - 1);
            return item;
        }

        void erase(const const_iterator& position) {
            erase_at(position.mIndex);
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            std::size_t first = firstIncluded.mIndex;
            std::size_t last = lastExcluded.mIndex;

            if (last > mCount)
                throw std::out_of_range("Erasing end");

            std::size_t diff = last - first;

            for (; last < mCount; ++first, ++last)
                mData[first] = mData[last];
            mCount = mCount - diff;
        }

        iterator begin() {
            return Iterator(*this, 0);
        }

        iterator end() {
            return Iterator(*this, mCount);
        }

        const_iterator cbegin() const {
            return ConstIterator(*this, 0);
        }

        const_iterator cend() const {
            return ConstIterator(*this, mCount);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        const int INIT_CAPACITY = 16;
        std::size_t mCapacity;
        std::size_t mCount;
        Type* mData;

        friend class ConstIterator;

        void realocate() {
            mCapacity *= 2;
            realocate(mCapacity);
        }

        void realocate(std::size_t pSize) {
            if (pSize < mCapacity)
                throw std::exception();
            Type* tmp = new Type[pSize];
            for (std::size_t idx = 0; idx < mCount; ++idx)
                tmp[idx] = mData[idx];
            delete[] mData;
            mData = tmp;
        }

        void insert_at(const Type& pValue, std::size_t pPosition) {
            if (mCount == mCapacity)
                realocate();

            for (std::size_t idx = mCount; idx > pPosition; --idx)
                mData[idx] = mData[idx - 1];
            mData[pPosition] = pValue;
            ++mCount;

        }

        void erase_at(std::size_t pIdx) {
            if (pIdx >= mCount)
                throw std::out_of_range("Erasing out of range");
            for (std::size_t idx = pIdx; idx + 1 < mCount; ++idx)
                mData[idx] = mData[idx + 1];
            --mCount;
        }
    };

    template<typename Type>
    class Vector<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;

        friend class Vector<Type>;

        explicit ConstIterator(const Vector<Type>& pVector, std::size_t pIdx) : mVector(pVector), mIndex(pIdx) { }

        ConstIterator(const ConstIterator& pOther) : mVector(pOther.mVector), mIndex(pOther.mIndex) { }

        reference operator*() const {
            if (mIndex == mVector.mCount)
                throw std::out_of_range("Dereferencing end iterator");
            return mVector.mData[mIndex];
        }

        ConstIterator& operator++() {
            if (mIndex == mVector.mCount)
                throw std::out_of_range("Iterator out of range");
            ++mIndex;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator it(*this);
            operator++();
            return it;
        }

        ConstIterator& operator--() {
            if (mIndex == 0)
                throw std::out_of_range("Iterator out of range");
            --mIndex;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator it(*this);
            operator--();
            return it;
        }

        ConstIterator operator+(difference_type d) const {
            std::size_t new_idx = mIndex + d;
            if (new_idx >= mVector.mCount)
                return ConstIterator(mVector, mVector.mCount);
            return ConstIterator(mVector, new_idx);
        }

        ConstIterator operator-(difference_type d) const {
            if (mIndex < (std::size_t) d)
                throw std::out_of_range("Iterator out of range");
            return ConstIterator(mVector, mIndex - d);
        }

        bool operator==(const ConstIterator& other) const {
            return (mIndex == other.mIndex) && (mVector.mData == other.mVector.mData);
        }

        bool operator!=(const ConstIterator& other) const {
            return !operator==(other);
        }

    protected:
        const Vector<Type>& mVector;
        std::size_t mIndex;
    };

    template<typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(const Vector<Type>& pVector, std::size_t pIdx) : ConstIterator(pVector, pIdx) { }

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

#endif // AISDI_LINEAR_VECTOR_H
