/************************************
** Edition:	v1.0.0 Demo
** Author:	Kingsley Chen	
** Date:	2013/10/22
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
#include <iterator>
#include <cstdio>

namespace KCSTL
{
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
            typedef __int64 pos_iter;

            explicit PriorityQueue(size_type initSize = DEFAULT_INIT_CAPACITY,
                                   const compare& cmp = compare(),
                                   const IncPrio& inc = IncPrio(),
                                   const DecPrio& dec = DecPrio())
                : _capacity(initSize), _size(0), _tweakCmp(cmp), _inc(inc), _dec(dec)
            {
                // god blesses new operator and constructors of value_type won't throw any exception..
                _internalHeap = new value_type[initSize];
            }

            // set the capacity as the size of the other
            // due to being consistent with STL container
            PriorityQueue(const PriorityQueue& other)
                : _capacity(other.size()), _size(other.size()), _tweakCmp(other._tweakCmp), _inc(other._inc), _dec(other._dec)
            {
                _internalHeap = new value_type[_capacity];
                std::copy(other._internalHeap, other._internalHeap + other.size(), _internalHeap);
            }
            
            PriorityQueue(const value_type* begin, const value_type* end,
                          const compare& cmp = compare(),
                          const IncPrio& inc = IncPrio(),
                          const DecPrio& dec = DecPrio())
                : _tweakCmp(cmp), _inc(inc), _dec(dec)
            {
                size_t len = std::distance(begin, end);
                assert(len > 0);

                _capacity = len;
                _size = len;
                _internalHeap = new value_type[len];
                std::copy(begin, end, _internalHeap);

                MakeHeap();
            }

            ~PriorityQueue()
            {
                delete [] _internalHeap;

                _internalHeap = nullptr;
                _capacity = 0;
                _size = 0;
            }

            PriorityQueue& operator =(const PriorityQueue& rhs)
            {
                if (this == &rhs)
                {            
                    return *this;
                }

                if (capacity() < rhs.size())
                {
                    // if new operator throws an exception, following code can make sure
                    // _internalHeap and _capacity remain unchanged
                    value_type* pOrigin = _internalHeap;

                    _internalHeap = new value_type[capacity() * GROWTHRATIO];
                    _capacity *= GROWTHRATIO;

                    delete [] pOrigin;
                }

                assert(capacity() >= rhs.size());
                std::copy(rhs._internalHeap, rhs._internalHeap + rhs.size(), _internalHeap);
                _size = rhs.size();

                _tweakCmp = rhs._tweakCmp;
                _inc = rhs._inc;
                _dec = rhs._dec;
                
                return *this;
            }

            size_type size() const
            {
                return _size;
            }

            size_type capacity() const
            {
                return _capacity;
            }

            bool empty() const
            {
                return size() == 0;
            }

            value_type& top()
            {
                if (empty())
                {
                    throw std::length_error("The queue must not be empty!");
                }

                return _internalHeap[0];
            }

            const value_type& top() const
            {
                if (empty())
                {
                    throw std::length_error("The queue must not be empty!");
                }

                return _internalHeap[0];
            }

            void ExtractTop()
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

            void insert(const value_type& ele)
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

            pos_iter find(const value_type& ele)
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

            void IncreasePriority(pos_iter pos, unsigned int det)
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

            void DecreasePriority(pos_iter pos, unsigned int det)
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

            void Delete(pos_iter pos)
            {
                if (pos < 0 || pos >= size())
                {
                    throw std::out_of_range("pos is not valid");
                }

                using std::swap;
                swap(_internalHeap[pos], _internalHeap[size()-1]);

                --_size;

                if (_tweakCmp(_internalHeap[parent(pos)], _internalHeap[pos]))
                {
                    MaintainBottomUp(pos);
                } 
                else
                {
                    MaintainTopDown(pos);
                }
            }

            void swap(PriorityQueue& other) throw()
            {
                using std::swap;

                // internal data structures
                swap(_internalHeap, other._internalHeap);
                swap(_size, other._size);
                swap(_capacity, other._capacity);

                // swap controlling function objects
                swap(_tweakCmp, other._tweakCmp);
                swap(_inc, other._inc);
                swap(_dec, other._dec);
            }

        private:
            bool full() const
            {
                return capacity() == size();
            }

            void MakeHeap()
            {
                for (pos_iter it = parent(size() - 1); it >= 0; --it)
                {
                    MaintainTopDown(it);
                }
            }

            void MaintainTopDown(pos_iter pos)
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

            void MaintainBottomUp(pos_iter pos)
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

        public:
            static const pos_iter NOT_FOUND = -1;
        
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
    void swap(PriorityQueue<T,compare,IncPrio,DecPrio>& lhs, 
        PriorityQueue<T,compare,IncPrio,DecPrio>& rhs)
    {
        lhs.swap(rhs);
    }
}
#endif