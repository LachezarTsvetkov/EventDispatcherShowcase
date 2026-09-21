#include <gtest/gtest.h>
#include <cstdlib>
#include "MemoryProfiler.h"

AllocationMetrics s_Metrics;

void* operator new(size_t size)
{
	s_Metrics.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size) noexcept
{
	s_Metrics.TotalFreed += size;
	free(memory);
}

void operator delete(void* memory) noexcept
{
	free(memory);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}