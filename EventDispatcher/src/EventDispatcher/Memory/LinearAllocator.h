#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>

class LinearAllocator
{
	public:
	LinearAllocator(size_t size) : m_capacity(size), m_Offset(0), m_Buffer(new uint8_t[size]) {}

	~LinearAllocator()
	{
		delete[] m_Buffer;
		m_Buffer = nullptr;
	}

	// Delete copy constructor and assignment operator to prevent copying
	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator=(const LinearAllocator&) = delete;

	void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t))
	{
		size_t padding = 0;
		uintptr_t currentAddress = reinterpret_cast<uintptr_t>(m_Buffer + m_Offset);

		// We use allignment to ensure that the returned address will be 'alligned' in the CPU memory, which normally reads data in chunks of 4, 8, or 16 bytes. We do this to avoid performance issues that can arise from unaligned memory access.
		if (alignment != 0 && currentAddress % alignment != 0)
		{
			padding = alignment - (currentAddress % alignment);
		}

		// Check if we have enough space in the buffer for the requested size and padding
		if (m_Offset + padding + size > m_capacity)
		{
			assert(false && "Allocation exceeds buffer size! Increase capacity.");
			return nullptr;
		}

		// Move the offset forward past the padding
		m_Offset += padding;

		// Save the aligned address to return
		void* alignedAddress = m_Buffer + m_Offset;

		// Bump the allocator forward by the object's size
		m_Offset += size;

		return alignedAddress;
	}

	// This should be called at the end of a frame or when you want to reset the allocator for reuse.
	void Reset()
	{
		// We do not have to clear the memory, as setting the offset back to 0 effectively "forgets" the previous allocations and will override any existing data in future allocations.
		m_Offset = 0;
	}

	size_t GetCapacity() const { return m_capacity; }
	size_t GetUsedMemory() const { return m_Offset; }
private:
	uint8_t* m_Buffer;
	size_t m_capacity, m_Offset;
};

