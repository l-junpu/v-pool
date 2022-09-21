#include "pool_header_includes.h"

struct SampleData
{
	SampleData(int x_ = 69) : x{ x_ }
	{ }

	int x;
};

int main()
{
	Tools::VirtualMemory mem;

	// Initialize Pool To Store Minimum 100 SampleData
	mem.Init<SampleData>(100);

	// Append Sample Data
	mem.Append<SampleData>();
}