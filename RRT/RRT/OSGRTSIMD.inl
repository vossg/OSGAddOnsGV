/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

OSG_BEGIN_NAMESPACE

template<typename _Tp> inline
AlignedAllocator<_Tp>::AlignedAllocator(void) throw()
{
}
    
template<typename _Tp> inline
AlignedAllocator<_Tp>::AlignedAllocator(const AlignedAllocator &) throw() 
{
}
    
template<typename _Tp> 
template<typename _Tp1> inline
AlignedAllocator<_Tp>::AlignedAllocator(const AlignedAllocator<_Tp1> &) throw()
{ 
}
    
template<typename _Tp> inline
AlignedAllocator<_Tp>::~AlignedAllocator() throw() 
{
}
    
template<typename _Tp> inline
typename AlignedAllocator<_Tp>::pointer 
    AlignedAllocator<_Tp>::address(reference __x) const 
{
    return &__x; 
}

template<typename _Tp> inline
typename AlignedAllocator<_Tp>::const_pointer 
    AlignedAllocator<_Tp>::address(const_reference __x) const 
{
    return &__x; 
}


template<typename _Tp> inline
typename AlignedAllocator<_Tp>::pointer 
    AlignedAllocator<_Tp>::allocate(size_type __n, const void *)
{ 
#ifndef WIN32
	if (__builtin_expect(__n > this->max_size(), false))
        std::__throw_bad_alloc();
#endif

    void *returnValue;
    
#if defined(__APPLE__) // Already aligned
    returnValue = malloc(__n * sizeof(_Tp));
#elif defined(OSG_HAS_POSIXMEMALIGN)
    posix_memalign(&returnValue, 16, __n * sizeof(_Tp));
#elif defined(WIN32)
	returnValue = _aligned_malloc(__n * sizeof(_Tp), 16);
#else
# error "don't know how to allocate alligned mem"
#endif

    return static_cast<_Tp *>(returnValue);
}

template<typename _Tp> inline
void AlignedAllocator<_Tp>::deallocate(pointer __p, size_type)
{ 
#if defined(__APPLE__) || defined(OSG_HAS_POSIXMEMALIGN)
    free(__p); 
#elif defined(WIN32)
    _aligned_free(__p);
#else
# error "don't know how to allocate alligned mem"
#endif
}

template<typename _Tp> inline
typename AlignedAllocator<_Tp>::size_type 
    AlignedAllocator<_Tp>::max_size() const throw() 
{ 
    return size_t(-1) / sizeof(_Tp); 
}

template<typename _Tp> inline
void  AlignedAllocator<_Tp>::construct(pointer __p, const _Tp &__val) 
{ 
    ::new(__p) _Tp(__val); 
}

template<typename _Tp> inline
void  AlignedAllocator<_Tp>::destroy(pointer __p) 
{
    __p->~_Tp(); 
}

OSG_END_NAMESPACE
