/************************************
** Edition:	v1.0.0 Demo
** Author:	Kingsley Chen	
** Date:	2013/06/05
** Purpose:	user-implemented priority queue container
************************************/

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _PRIQUEUE_FF4626F5_1A57_41ba_AE80_6FA5F9001AC7
#define _PRIQUEUE_FF4626F5_1A57_41ba_AE80_6FA5F9001AC7

#include <cassert>
#include <functional>
#include <stdexcept>

template<typename T>
struct Inc 
{
    void operator ()(T& priority, size_t det)
    {
        priority += det;
    }
};

template<typename T>
struct Dec
{
    void operator ()(T& priority, size_t det)
    {
        priority -= det;
    }
};

template<
    typename T,
    typename compare = std::less<T>,
    typename IncPrio = Inc<T>,
    typename DecPrio = Dec<T>>
class PriorityQueue
{
    public:
        typedef T value_type;
        typedef size_t size_type;
        typedef size_t pos_iter;
        enum {NOT_FOUND = -1};

        explicit PriorityQueue(size_type initSize = DEFAULT_INIT_CAPACITY,
                               const compare& cmp = compare(),
                               const IncPrio& inc = IncPrio(),
                               const DecPrio& dec = DecPrio());
        PriorityQueue(const PriorityQueue& other);
        PriorityQueue(const value_type* begin, const value_type* end,
                      const compare& cmp = compare(),
                      const IncPrio& inc = IncPrio(),
                      const DecPrio& dec = DecPrio());
        ~PriorityQueue();
        PriorityQueue& operator =(const PriorityQueue& rhs);

        const value_type& top() const;
        void ExtractTop();
        void insert(const value_type& ele);
        void swap(PriorityQueue& other);
        pos_iter find(const value_type& ele);
        void IncreaseKey(pos_iter pos, unsigned int det);
        void DecreaseKey(pos_iter pos, unsigned int det);
        void Delete(pos_iter pos);

        inline size_type size() const;
        inline size_type capacity() const;
        inline bool empty() const;
    
    private:
        inline bool full() const;
        void MaintainTopDown(pos_iter pos);
        void MaintainBottomUp(pos_iter pos);

        // two main reasons for these code here
        // 1. using macros may bring about chaos
        // 2. coding inside class saves keyboard typing
        inline pos_iter parent(pos_iter pos) const
        {
            return ((pos + 1) >> 1) - 1;
        }

        inline pos_iter left(pos_iter pos) const
        {
            return (pos << 1) + 1;
        }

        inline pos_iter right(pos_iter pos) const
        {
            return (pos + 1) << 1;
        }
        
    private:
        enum {DEFAULT_INIT_CAPACITY = 32, GROWTHRATIO = 2};
        value_type* _internalHeap;
        size_type   _capacity;
        size_type   _size;
        
        compare     _tweakCmp;  // return true if need tweak
        IncPrio     _inc;
        DecPrio     _dec;
};

template<typename T, typename compare, typename IncPrio, typename DecPrio>
PriorityQueue<T,compare,IncPrio,DecPrio>::PriorityQueue(size_type initSize,
                                                        const compare& cmp,
                                                        const IncPrio& inc,
                                                        const DecPrio& dec)
    : _capacity(initSize), _size(0), _tweakCmp(cmp), _inc(inc), _dec(dec)
{
    // god blesses new operator and constructors of value_type won't throw any exception..
    _internalHeap = new value_type[initSize];
}

//TODO: there are a lot of constructors and operator = needed to be done

template<typename T, typename compare, typename IncPrio, typename DecPrio>
PriorityQueue<T,compare,IncPrio,DecPrio>::~PriorityQueue()
{
    delete [] _internalHeap;

    _internalHeap = nullptr;
    _capacity = 0;
    _size = 0;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
inline typename PriorityQueue<T,compare,IncPrio,DecPrio>::size_type
    PriorityQueue<T,compare,IncPrio,DecPrio>::capacity() const
{
    return _capacity;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
inline typename PriorityQueue<T,compare,IncPrio,DecPrio>::size_type
    PriorityQueue<T,compare,IncPrio,DecPrio>::size() const
{
    return _size;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
inline bool PriorityQueue<T,compare,IncPrio,DecPrio>::empty() const
{
    return size() == 0 ? true : false;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
inline bool PriorityQueue<T,compare,IncPrio,DecPrio>::full() const
{
    return size() == capacity() ? true : false;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
const typename PriorityQueue<T,compare,IncPrio,DecPrio>::value_type& 
    PriorityQueue<T,compare,IncPrio,DecPrio>::top() const
{
    if (empty())
    {
        throw std::length_error("The queue must not be empty!");
    }

    return _internalHeap[0];
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::ExtractTop()
{
    if (empty())
    {
        throw std::length_error("The queue must not be empty!");
    }

    using std::swap;
    swap(_internalHeap[0], _internalHeap[size()-1]);
    --_size;

    // if last element had been extracted, no need for MaintainTopDown
    if (!empty())
    {
        MaintainTopDown(0);
    }    
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::insert(const value_type& ele)
{
    if (full())
    {
        value_type* tmpBuf = new value_type[capacity()*GROWTHRATIO];
        _capacity *= GROWTHRATIO;

        std::copy(_internalHeap, _internalHeap + size(), tmpBuf);
        
        delete [] _internalHeap;
        _internalHeap = tmpBuf;
    }

    assert(size() < capacity());
    _internalHeap[size()] = ele;
    ++_size;

    MaintainBottomUp(size() - 1);           
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::MaintainTopDown(pos_iter pos)
{
    assert(pos >= 0 && pos < size());

    bool isMaintained = false;
    pos_iter posChecking = pos;
    value_type val = _internalHeap[posChecking];

    while (!isMaintained && left(posChecking) < size())
    {
        pos_iter j = left(posChecking);
        if (j < size() - 1 && _tweakCmp(_internalHeap[j], _internalHeap[j+1]))
        {
            ++j;
        }

        // tweak positions
        if (_tweakCmp(val, _internalHeap[j]))
        {
            _internalHeap[posChecking] = _internalHeap[j];
            posChecking = j;
        }
        else
        {
            isMaintained = true;
        }
    }

    _internalHeap[posChecking] = val;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::MaintainBottomUp(pos_iter pos)
{
    assert(pos >= 0 && pos < size());

    pos_iter posChecking = pos;
    value_type val = _internalHeap[posChecking];

    while (posChecking > 0 && 
            _tweakCmp(_internalHeap[parent(posChecking)], _internalHeap[posChecking]))
    {
        _internalHeap[posChecking] = _internalHeap[parent(posChecking)];
        posChecking = parent(posChecking);
    }

    _internalHeap[posChecking] = val;
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
typename PriorityQueue<T,compare,IncPrio,DecPrio>::pos_iter
    PriorityQueue<T,compare,IncPrio,DecPrio>::find(const value_type& ele)
{
    for (pos_iter it = 0; it < size(); ++it)
    {
        if (_internalHeap[it] == ele)
        {
            return it;
        }
    }

    return pos_iter(NOT_FOUND);
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::IncreaseKey(pos_iter pos, unsigned int det)
{
    if (0 == det)
    {
        return;
    }

    if (pos < 0 || pos >= size())
    {
        throw std::out_of_range("pos is not valid!");
    }

    _inc(_internalHeap[pos], det);
    MaintainBottomUp(pos);
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::DecreaseKey(pos_iter pos, unsigned int det)
{
    if (0 == det)
    {
        return;
    }

    if (pos < 0 || pos >= size())
    {
        throw std::out_of_range("pos is not valid");
    }

    _dec(_internalHeap[pos], det);
    MaintainTopDown(pos);
}


template<typename T, typename compare, typename IncPrio, typename DecPrio>
void PriorityQueue<T,compare,IncPrio,DecPrio>::Delete(pos_iter pos)
{
    if (pos < 0 || pos >= size())
    {
        throw std::out_of_range("pos is not valid");
    }

    using std::swap;
    swap(_internalHeap[pos], _internalHeap[size()-1]);
    
    --_size;

    if (_tweakCmp(_internalHeap[pos], _internalHeap[parent(pos)]))
    {
        MaintainBottomUp(pos);
    } 
    else
    {
        MaintainTopDown(pos);
    }
}

#endif