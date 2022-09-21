#pragma once
#ifndef VIRTUAL_POOL_INLINE
#define VIRTUAL_POOL_INLINE

#include "virtual_pool.h"

namespace Tools
{
	VirtualMemory::VirtualMemory(uint32_t PageSize) noexcept :
		m_PageSize{ PageSize }
	{
		assert(m_PageSize != 0);
	}

	inline void VirtualMemory::Reset(void) noexcept
	{

	}

	inline uint32_t VirtualMemory::ComputePageIndex(uint32_t DataIndex) const noexcept
	{
		uint32_t Index = std::ceil( (DataIndex * m_DataSize) / static_cast<float>(m_PageSize) );
		return Index;
	}
 
	template <typename T>
	void VirtualMemory::Init(uint32_t MaxDataElements) noexcept
	{
		// Compute Required Memory
		uint32_t m_DataSize    = sizeof(T);
		uint32_t RequiredBytes = MaxDataElements * m_DataSize;
		uint32_t PagesRequired = static_cast<uint32_t>( std::ceil(RequiredBytes / static_cast<float>(m_PageSize)) );

		// Reserve Memory Pool - No Access Allowed Yet
		m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(nullptr, PagesRequired, MEM_RESERVE, PAGE_NOACCESS) );

		// Assert If Memory Pool Was Not Allocated Successfully
		assert(m_Pool != nullptr);
	}

	template <typename T_DATA, typename... T_ARGS>
	inline uint32_t VirtualMemory::Append(void) noexcept
	{
		std::cout << "Append T_ARGS..." << std::endl;
		auto CurrentIndex = ComputePageIndex(m_DataCount);
		auto AppendedIndex = ComputePageIndex(m_DataCount + 1);

		// If Page Indexes Don't Match, Current Page Has Overflowed - Gotta do an additional check somewhere
		// to determine if mem_reserve is maxed out
		if (CurrentIndex != AppendedIndex)
		{
			auto AppendAddress = m_Pool + CurrentIndex * m_PageSize;
			m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(AppendAddress, m_PageSize, MEM_COMMIT, PAGE_READWRITE) );
		}

		// Placement New
		auto PtrAddress = m_Pool + m_DataCount * m_DataSize;
		new(PtrAddress) T_DATA(T_ARGS...);

		// Update Data Count
		return m_DataCount++;
	}

	template <Defines::TriviallyConstructibleData T_DATA>
	inline uint32_t VirtualMemory::Append(void) noexcept
	{
		std::cout << "Append Default" << std::endl;
		auto CurrentIndex = ComputePageIndex(m_DataCount);
		auto AppendedIndex = ComputePageIndex(m_DataCount + 1);

		// If Page Indexes Don't Match, Current Page Has Overflowed - Gotta do an additional check somewhere
		// to determine if mem_reserve is maxed out
		if (CurrentIndex != AppendedIndex)
		{
			auto AppendAddress = m_Pool + CurrentIndex * m_PageSize;
			m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(AppendAddress, m_PageSize, MEM_COMMIT, PAGE_READWRITE) );
		}

		// Placement New
		auto PtrAddress = m_Pool + m_DataCount * m_DataSize;
		new(PtrAddress) T_DATA;

		// Update Data Count
		return m_DataCount++;
	}

	template <typename T_DATA>
	inline uint32_t VirtualMemory::Remove(const uint32_t Index) noexcept
	{
		// Assert In Range

		// Update Data Count
		--m_DataCount;

		auto& PtrAddress = m_Pool + Index * m_DataSize;

		// Destructor Is Not Trivial - Manually Destroy
		if (!std::is_trivially_constructible_v<T_DATA>)
		{
			std::destroy_at(PtrAddress);
		}

		// Not Removing Last - Swap Last Element With Deleted Element
		if (Index != m_DataCount)
		{
			memcpy( &PtrAddress
				  , &(m_Pool + m_DataCount)
				  , m_DataSize );
		}

		// Pool Is Empty - Deallocate Pool
		auto AppendedIndex = ComputePageIndex(m_DataCount + 1);
		if (AppendedIndex != ComputePageIndex(m_DataCount))
		{
			const auto PageAddress = &(m_Pool + AppendedIndex * m_PageSize);
			VirtualFree(PageAddress, m_PageSize, MEM_DECOMMIT);
		}

		// Return Local Pool Index That Was Shifted
		return m_DataCount;
	}

	template <typename T>
	T* VirtualMemory::FindData(const uint32_t Index) const noexcept
	{
		
	}

	template <typename T>
	T& VirtualMemory::GetData(const uint32_t Index) const noexcept
	{

	}
}

#endif