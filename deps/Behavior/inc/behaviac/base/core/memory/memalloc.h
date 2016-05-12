/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Description: Memory allocator helper
//
////////////////////////////////////////////////////////////////////////////////
#ifndef BEHAVIAC_MEMORY_MEMALLOC_H
#define BEHAVIAC_MEMORY_MEMALLOC_H

#include "behaviac/base/core/config.h"

namespace behaviac
{
    class BEHAVIAC_API PtrSizeRegister
    {
    public:
        PtrSizeRegister()
        {
        }
        virtual ~PtrSizeRegister()
        {
        }

        virtual void RegisterPtrSize(const void* ptr, size_t bytes) = 0;
        virtual void UnRegisterPtr(const void* ptr) = 0;

        virtual size_t GetAllocatedSize() const = 0;
        virtual size_t GetMemoryUsage() const = 0;

        static PtrSizeRegister* Create();
        static void Destroy(PtrSizeRegister*);
    };

    /*! \addtogroup Memory
     * @{
     * \addtogroup memallocator
     * @{ */

    /// The IMemAllocator class is the base class for anyone who want to implement his own memory allocator.
    /*!
    */
    class BEHAVIAC_API IMemAllocator
    {
    public:
        IMemAllocator()
        {
        }

        virtual ~IMemAllocator()
        {
        }

        /// to vefify the allocator
        /**
        return true if the allocator is 'correct'
        return false if it is corrupt or anything is wrong
        */
        virtual bool Verify() const
        {
            return true;
        }

        /// To be implemented by derived allocators
        /// Alloc, allocate a block of memory of at least Size bytes
        ///no good way is found to not use virtual
        /*! \param  Size The size of the block to allocate. Must be > 0
        \return A pointer on the block allocated or 0 if not enough memory.
        */
        virtual void* Alloc(size_t size, const char* tag, const char* pFile, unsigned int Line) = 0;

        /// Realloc, reallocate a block of memory of at least Size bytes
        /*! \param  pOldPtr A pointer on the block on which you want to change the size.
        The allocator should be able to handle a null pointer, behavior should be the same as Alloc
        \param  Size    The size of the block to reallocate.
        The allocator should be able to handle a Size == 0, behavior should be the same as Free
        \return A pointer on the new block allocated or 0 if not enough memory.
        */
        virtual void* Realloc(void* pOldPtr, size_t size, const char* tag, const char* pFile, unsigned int Line) = 0;

        /// Free a block of memory previously allocated with Alloc or Realloc
        /*! \param  pData  A pointer on a block of data allocated. Null pointer should be handled correctly by the allocator
        */
        virtual void Free(void* pData, const char* tag, const char* pFile, unsigned int Line) = 0;

        // To be implemented by derived allocators
        /// Alloc, allocate a block of memory of at least Size bytes
        /*! \param  Size The size of the block to allocate. Must be > 0
        \param  alignment The alignment needed for that allocation
        \return A pointer on the block allocated or 0 if not enough memory.
        */
        virtual void* AllocAligned(size_t size, size_t alignment, const char* tag, const char* pFile, unsigned int Line) = 0;

        /// Realloc, reallocates a block of memory of at least Size bytes
        /*!
        \param  pOldPtr A pointer on the block on which you want to change the size
        The allocator should be able to handle a null pointer, behavior should be the same as AllocAligned
        \param  Size    The size of the block to reallocate.
        The allocator should be able to handle a Size == 0, behavior should be the same as Free
        \param  alignment The alignment needed for that allocation
        \return A pointer on the new block allocated or 0 if not enough memory.
        */
        virtual void* ReallocAligned(void* pOldPtr, size_t size, size_t alignment, const char* tag, const char* pFile, unsigned int Line) = 0;

        /// Free a block of memory previously allocated with AllocAligned or ReallocAligned
        /*! \param  pData  A pointer on a block of data allocated. 0 will assert.
        */
        virtual void FreeAligned(void* pData, size_t alignment, const char* tag, const char* pFile, unsigned int Line) = 0;

        // Allocator Information

        /// Returns the name of the allocator
        virtual const char* GetName(void) const
        {
            return "IMemAllocator";
        }

        /// Returns the maximum size that can be allocated
		virtual size_t GetMaxAllocationSize(void) const
        {
			return (size_t)-1;
        }

        /// Returns the real memory size allocated by the allocator for a precise pointer
        /*! \param pData            The pointer on the data block allocated
        \param pRetHeaderSize   [Output] [Optional] When specified, will contains the size of the data added
        by the allocator in front of the block allocated.
        \return                 Return the real total size allocated in memory, including the header & padding
        at the end (for alignment purpose).
        */
		virtual size_t GetAllocatedSize() const = 0;
    };

    /// Call this function to retreive the Allocator used by default
    BEHAVIAC_API IMemAllocator& GetDefaultMemoryAllocator();

    /// Call this function to cleanup the Allocator used by default
    BEHAVIAC_API void CleanupDefaultMemoryAllocator();
}

#define BEHAVIAC_ALLOCATOR_MALLOC_WITHBEHAVIAC_FILELINE(allocator, size, tag, file, line)							(allocator)->Alloc(size, tag, file, line)
#define BEHAVIAC_ALLOCATOR_REALLOC_WITHBEHAVIAC_FILELINE(allocator, p, size, tag, file, line)						(allocator)->Realloc(p, size, tag, file, line)
#define BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHBEHAVIAC_FILELINE(allocator, size, alignment, tag, file, line)			(allocator)->AllocAligned(size, alignment, tag, file, line)
#define BEHAVIAC_ALLOCATOR_REALLOCALIGNED_WITHBEHAVIAC_FILELINE(allocator, p, size, alignment, tag, file, line)		(allocator)->ReallocAligned(p, size, alignment, tag, file, line)
#define BEHAVIAC_ALLOCATOR_FREE_WITHBEHAVIAC_FILELINE(allocator, p, tag, file, line)								(allocator)->Free(p, tag, file, line)
#define BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHBEHAVIAC_FILELINE(allocator, p, alignment, tag, file, line)			(allocator)->FreeAligned(p, alignment, tag, file, line)

#define BEHAVIAC_ALLOCATOR_MALLOC_WITHTAG(allocator, size, tag)														BEHAVIAC_ALLOCATOR_MALLOC_WITHBEHAVIAC_FILELINE(allocator, size, tag, __FILE__, __LINE__)
#define BEHAVIAC_ALLOCATOR_REALLOC_WITHTAG(allocator, p, size, tag)													BEHAVIAC_ALLOCATOR_REALLOC_WITHBEHAVIAC_FILELINE(allocator, p, size, tag, __FILE__, __LINE__)
#define BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHTAG(allocator, size, alignment, tag)									BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHBEHAVIAC_FILELINE(allocator, size, alignment, tag, __FILE__, __LINE__)
#define BEHAVIAC_ALLOCATOR_REALLOCALIGNED_WITHTAG(allocator, p, size, alignment, tag)								BEHAVIAC_ALLOCATOR_REALLOCALIGNED_WITHBEHAVIAC_FILELINE(allocator, p, size, alignment, tag, __FILE__, __LINE__)
#define BEHAVIAC_ALLOCATOR_FREE_WITHTAG(allocator, p, tag)													BEHAVIAC_ALLOCATOR_FREE_WITHBEHAVIAC_FILELINE(allocator, p, tag, __FILE__, __LINE__)
#define BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHTAG(allocator, p, alignment, tag)									BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHBEHAVIAC_FILELINE(allocator, p, alignment, tag, __FILE__, __LINE__)

#define BEHAVIAC_ALLOCATOR_MALLOC(allocator, size)																	BEHAVIAC_ALLOCATOR_MALLOC_WITHTAG(allocator, size, 0)
#define BEHAVIAC_ALLOCATOR_REALLOC(allocator, p, size)																BEHAVIAC_ALLOCATOR_REALLOC_WITHTAG(allocator, p, size, 0)
#define BEHAVIAC_ALLOCATOR_MALLOCALIGNED(allocator, size, alignment)												BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHTAG(allocator, size, alignment, 0)
#define BEHAVIAC_ALLOCATOR_REALLOCALIGNED(allocator, p, size, alignment)											BEHAVIAC_ALLOCATOR_REALLOCALIGNED_WITHTAG(allocator, p, size, alignment, 0)
#define BEHAVIAC_ALLOCATOR_FREE(allocator, p)																BEHAVIAC_ALLOCATOR_FREE_WITHTAG(allocator, p, 0)
#define BEHAVIAC_ALLOCATOR_FREEALIGNED(allocator, p, alignment)												BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHTAG(allocator, p, alignment, 0)

/*! @} */
/*! @} */

#endif//BEHAVIAC_MEMORY_MEMALLOC_H
