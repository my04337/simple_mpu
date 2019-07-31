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
	as.move(Reg::R1, 0x80);
	as.move(RegIndirect(Reg::R1), 2);
	as.add(Reg::R0, RegIndirect(Reg::R1), Reg::R2);
	as.halt();

	std::cout << ">>> Executing" << std::endl;
	MPU mpu{mem};
	mpu.run();

	std::cout << ">>> Result" << std::endl;
	mem.dump(0, 256);
	std::cout << std::endl;
	mpu.dump();
	
	return 0;
}