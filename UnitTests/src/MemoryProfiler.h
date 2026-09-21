#pragma once
#include <cstdint>

struct AllocationMetrics
{
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage() const { return TotalAllocated - TotalFreed; }
};

// We use extern here, so all test files share the same instance of this struct
extern AllocationMetrics s_Metrics;