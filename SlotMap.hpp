#pragma once


#include <cstdint>
#include <limits>
#include <xutility>
#include <vector>
#include <algorithm>
#include <type_traits>



#if 0


void SlotMapTest()
{
    {
        SlotMap<int> s;
        assert(s.empty());
        assert(s.size() == 0);
    }

    {
        SlotMap<int> s1;
        SlotMap<int> s2 = s1;
        assert(s2.empty());
        assert(s2.size() == 0);

        s1.insert(1);
        s1.insert(2);

        SlotMap<int> s3 = std::move(s1);
        assert(s1.size() == 0);
    }

    {
        SlotMap<int> s1;

        auto k0 = s1.insert(0);
        auto k1 = s1.insert(1);

        assert(s1.valid(k0));
        s1.erase(k0);
        assert(!s1.valid(k0));

        for (auto& x : s1)
            assert(x == 1);

        assert(s1.get(k1) != nullptr);
    }


}

#endif





template<typename T, size_t GenerationNum = UINT_MAX>
class SlotMap
{
public:
    using IndexType = unsigned int;// typename std::vector<T>::size_type;
    using GenerationType = typename std::conditional<GenerationNum <= (255), uint8_t, typename std::conditional<GenerationNum <= (65535), uint16_t, uint32_t>::type>::type;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using differnce_type = typename std::vector<T>::difference_type;
    using size_type = typename std::vector<T>::size_type;

    struct Key
    {
        friend class SlotMap;

        Key()
            :index(0), gen(0)
        {

        }

    private:
        Key(IndexType index, GenerationType gen)
            :index(index), gen(gen)
        {}

        IndexType index;
        IndexType gen;
    };

    SlotMap()
    {
        _indices.emplace_back();
    }

    SlotMap(const SlotMap& other)
        :_datas(other._datas), _indices(other._indices), 
         _erases(other._erases), _freeHead(other._freeHead),
        _freeTail(other._freeTail)
    {


    }

    SlotMap(SlotMap&& other)
    {
        _datas = std::move(other._datas);
        _indices = std::move(other._indices);
        _erases = std::move(other._erases);
        _freeHead = other._freeHead;
        _freeTail = other._freeTail;

        other._freeHead = 0;
        other._freeTail = 0;
    }

    SlotMap& operator=(const SlotMap& other)
    {
        _datas = other._datas;
        _indices = other._indices;
        _erases = other._erases;
        _freeHead = other._freeHead;
        _freeTail = other._freeTail;
        return *this;
    }

    SlotMap& operator=(SlotMap&& other)
    {
        _datas = std::move(other._datas);
        _indices = std::move(other._indices);
        _erases = std::move(other._erases);
        _freeHead = other._freeHead;
        _freeTail = other._freeTail;

        other._freeHead = 0;
        other._freeTail = 0;
    }

    void swap(SlotMap<T, GenerationNum>& other)
    {
        std::swap(_datas, other._datas);
        std::swap(_indices, other._indices);
        std::swap(_erases, other._erases);
        std::swap(_freeHead, other._freeHead);
        std::swap(_freeTail, other._freeTail);
    }

    Key insert(const T& v)
    {
        if (_freeHead == _freeTail)
            ExtendIndices();
        _datas.push_back(v);
        _erases.push_back(_freeHead);
        auto old = _freeHead;
        GenerationType gen = PopIndices().gen;
        return Key(old, gen);
    }

    bool valid(Key key) const 
    {
        return key.index < _indices.size() && _indices[key.index].gen == key.gen;
    };

    void erase(Key key)
    {
        if (!valid(key))
            return;
        ++_indices[key.index].gen;
        IndexType dataIndex = _indices[key.index].index;
        IndexType dataLastIndex = _datas.size() - 1;

        _datas[dataIndex] = std::move(_datas[dataLastIndex]);
        _datas.pop_back();

        auto needMovePtrIndex = _erases[dataLastIndex];
        _erases[dataIndex] = needMovePtrIndex;
        _erases.pop_back();

        if (!_datas.empty())
        {
            _indices[needMovePtrIndex].index = dataIndex;
        }

        _indices[_freeTail].index = key.index;
        _freeTail = key.index;
    }

    reference operator[](Key key)
    {
        return _datas[_indices[key.index].index];
    }

    const_reference operator[](Key key) const
    {
        return _datas[_indices[key.index].index];
    }

    bool operator==(const SlotMap<T>& other) const
    {
        return data_ == arg.data_;
    }

    bool operator!=(const SlotMap<T>& other) const
    {
        return data_ != arg.data_;
    }

    pointer get(Key key) 
    {
        return valid(key) ? &(*this)[key] : nullptr;
    };

    const_pointer get(Key key) const
    {
        return valid(key) ? &(*this)[key] : nullptr;
    };

    size_type size() const 
    {
        return _datas.size(); 
    }

    size_type max_size() const 
    {
        return std::numeric_limits<IndexType>::max();
    }

    bool empty() const { return _datas.empty(); }

    iterator begin() { return _datas.begin(); }

    iterator end() { return _datas.end(); }

    const_iterator begin() const { return _datas.begin(); }

    const_iterator end() const { return _datas.end(); }

    const_iterator cbegin() const { return _datas.cbegin(); }

    const_iterator cend() const { return _datas.cend(); }

private:

    void ExtendIndices()
    {
        _indices.emplace_back();
        _freeTail = _indices.size() - 1;
        _indices[_freeHead].index = _freeTail;
    }

    Key& PopIndices()
    {
        Key& key = _indices[_freeHead];
        if (_freeHead != _freeTail)
            _freeHead = key.index;
        key.index = _datas.size() - 1;
        return key;
    }

private:
    std::vector<T> _datas;
    std::vector<Key> _indices;
    std::vector<IndexType> _erases;

    IndexType _freeHead = 0;
    IndexType _freeTail = 0;

};