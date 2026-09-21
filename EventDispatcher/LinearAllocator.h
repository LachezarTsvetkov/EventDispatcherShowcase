#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>

class LinearAllocator
{
	public:
	LinearAllocator(size_t size) : m_capacity(size), m_Offset(0) {}

	void* Allocate(size_t size)
	{
		assert(m_Offset + size <= m_capacity && "Allocation exceeds buffer size!");
		void* ptr = reinterpret_cast<char*>(m_Buffer) + m_Offset;
		m_Offset += size;
		return ptr;
	}

	void Reset()
	{
		m_Offset = 0;
	}

	size_t GetCapacity() const { return m_capacity; }
	size_t GetUsedMemory() const { return m_Offset; }
private:
	uint8_t* m_Buffer;
	size_t m_capacity;
	size_t m_Offset;
};

