#include <array>
#include <vector>
#include <type_traits>
#include <iostream>
#include <iomanip>

#include "Memory.h"
#include "Assembler.h"
#include "MPU.h"

using namespace mpu;

int main()
{
	std::cout << ">>> Assembling" << std::endl;
	Memory mem{0x100};
	Assembler as{mem};
	as.move(Reg::R0, 1);
	as.move(Reg::R1, Reg::R0);
	as.move(Reg::R4, 0x0080);
	as.move(Reg::R4, RegAccess::Far, 0xCCCC);
	as.halt();

	std::cout << ">>> Executing" << std::endl;
	MPU mpu{mem};
	mpu.run();
	mpu.dump();
	mem.dump(0, 256);
	
	return 0;
}