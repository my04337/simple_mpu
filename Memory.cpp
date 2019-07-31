#include "Memory.h"

#include <iostream>
#include <iomanip>

using namespace mpu;

void Memory::dump(size_t offset, size_t size) 
{
	constexpr size_t bytesPerLine = 16;

	if(offset + size < _bytes.size()) return;
	std::cout << "      ";
	for (size_t i = 0; i < bytesPerLine; ++i) {
		std::cout << " " << std::hex << std::setw(2) << std::setfill('0') << i;
	}
	std::cout << std::endl;
	for(size_t i=0; i<size; ++i) {
		size_t pos = i + offset;
		if(i % bytesPerLine == 0) std::cout << std::hex << std::setw(4) << std::setfill('0') << i << " :";
		std::cout << " " << std::hex << std::setw(2) << std::setfill('0') << (int)_bytes[pos];

		if(i % bytesPerLine == bytesPerLine-1) std::cout << std::endl; 
	}
}
