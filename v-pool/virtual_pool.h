#pragma once
#ifndef VIRTUAL_POOL
#define VIRTUAL_POOL

#include "type_defines.h"

#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>

#include <cmath> // ceil
#include <Windows.h> // memory
#include <cassert> // assert
#include <concepts> // concepts

namespace Tools
{
	namespace Defines
	{
		template <typename T>
		concept TriviallyConstructibleData = std::is_trivially_constructible_v<T>;
	}


	class VirtualMemory final : public Type::NoCopy, Type::NoMove
	{
	public:

		using MemoryPool = std::byte*;
		static constexpr auto invalid_index_v = UINT32_MAX;

		/*
			Construction
		*/
		VirtualMemory(uint32_t PageSize = 4096) noexcept;


		/*
			Initializing
		*/
		template <typename T>
		void Init(uint32_t MaxDataElements) noexcept;


		/*
			Data Management
		*/
		template <typename T_DATA, typename... T_ARGS>
		inline uint32_t Append(void) noexcept;

		template <Defines::TriviallyConstructibleData T_DATA>
		uint32_t Append(void) noexcept;

		template <typename T_DATA>
		inline uint32_t Remove(const uint32_t Index) noexcept;

		inline void Reset(void) noexcept;


		/*
			Get Data
		*/
		template <typename T>
		T* FindData(const uint32_t Index) const noexcept;

		template <typename T>
		T& GetData(const uint32_t Index) const noexcept;


	private:

		/*
			Private - Helper Functions
		*/
		inline uint32_t ComputePageIndex(uint32_t DataIndex) const noexcept;


		MemoryPool            m_Pool            { };
		uint32_t              m_DataCount      = 0;
		const uint32_t        m_PageSize       = 4096;
		uint8_t               m_DataSize       = 0;
	};




	///*
	//	Template Definitions
	//*/
	//template <typename T>
	//void VirtualMemory::Init(uint32_t MaxDataElements) noexcept
	//{
	//	// Compute Required Memory
	//	uint32_t m_DataSize    = sizeof(T);
	//	uint32_t RequiredBytes = MaxDataElements * m_DataSize;
	//	uint32_t PagesRequired = static_cast<uint32_t>( std::ceil(RequiredBytes / static_cast<float>(m_PageSize)) );

	//	// Reserve Memory Pool - No Access Allowed Yet
	//	m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(nullptr, PagesRequired, MEM_RESERVE, PAGE_NOACCESS) );

	//	// Assert If Memory Pool Was Not Allocated Successfully
	//	assert(m_Pool != nullptr);
	//}

	//template <typename T_DATA, typename... T_ARGS>
	//inline uint32_t VirtualMemory::Append(void) noexcept
	//{
	//	std::cout << "Append T_ARGS..." << std::endl;
	//	auto CurrentIndex = ComputePageIndex(m_DataCount);
	//	auto AppendedIndex = ComputePageIndex(m_DataCount + 1);

	//	// If Page Indexes Don't Match, Current Page Has Overflowed - Gotta do an additional check somewhere
	//	// to determine if mem_reserve is maxed out
	//	if (CurrentIndex != AppendedIndex)
	//	{
	//		auto AppendAddress = m_Pool + CurrentIndex * m_PageSize;
	//		m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(AppendAddress, m_PageSize, MEM_COMMIT, PAGE_READWRITE) );
	//	}

	//	// Placement New
	//	auto PtrAddress = m_Pool + m_DataCount * m_DataSize;
	//	new(PtrAddress) T_DATA(T_ARGS...);

	//	// Update Data Count
	//	return m_DataCount++;
	//}

	//template <Defines::TriviallyConstructibleData T_DATA>
	//inline uint32_t VirtualMemory::Append(void) noexcept
	//{
	//	std::cout << "Append Default" << std::endl;
	//	auto CurrentIndex = ComputePageIndex(m_DataCount);
	//	auto AppendedIndex = ComputePageIndex(m_DataCount + 1);

	//	// If Page Indexes Don't Match, Current Page Has Overflowed - Gotta do an additional check somewhere
	//	// to determine if mem_reserve is maxed out
	//	if (CurrentIndex != AppendedIndex)
	//	{
	//		auto AppendAddress = m_Pool + CurrentIndex * m_PageSize;
	//		m_Pool = std::bit_cast<MemoryPool>( VirtualAlloc(AppendAddress, m_PageSize, MEM_COMMIT, PAGE_READWRITE) );
	//	}

	//	// Placement New
	//	auto PtrAddress = m_Pool + m_DataCount * m_DataSize;
	//	new(PtrAddress) T_DATA;

	//	// Update Data Count
	//	return m_DataCount++;
	//}

	//template <typename T_DATA>
	//inline uint32_t VirtualMemory::Remove(const uint32_t Index) noexcept
	//{
	//	// Assert In Range

	//	// Update Data Count
	//	--m_DataCount;

	//	auto& PtrAddress = m_Pool + Index * m_DataSize;

	//	// Destructor Is Not Trivial - Manually Destroy
	//	if (!std::is_trivially_constructible_v<T_DATA>)
	//	{
	//		std::destroy_at(PtrAddress);
	//	}

	//	// Not Removing Last - Swap Last Element With Deleted Element
	//	if (Index != m_DataCount)
	//	{
	//		memcpy( &PtrAddress
	//			  , &(m_Pool + m_DataCount)
	//			  , m_DataSize );
	//	}

	//	// Pool Is Empty - Deallocate Pool
	//	auto AppendedIndex = ComputePageIndex(m_DataCount + 1);
	//	if (AppendedIndex != ComputePageIndex(m_DataCount))
	//	{
	//		const auto PageAddress = &(m_Pool + AppendedIndex * m_PageSize);
	//		VirtualFree(PageAddress, m_PageSize, MEM_DECOMMIT);
	//	}

	//	// Return Local Pool Index That Was Shifted
	//	return m_DataCount;
	//}
}

#endif