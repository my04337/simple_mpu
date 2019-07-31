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
	Memory mem{0x80}; // メモリは128バイト
	Assembler as{mem};

	// R0 <= 1
	as.move(Reg::R0, 1); 

	// R1 <= 0x40
	as.move(Reg::R1, 0x40); 

	// *R1 <= 2
	as.move(RegIndirect(Reg::R1), 2); 

	// R2 <= R1 + *R2
	as.add(Reg::R0, RegIndirect(Reg::R1), Reg::R2); 

	// 終了
	as.halt();

	std::cout << ">>> Executing" << std::endl;
	MPU mpu{mem};
	mpu.run();

	std::cout << ">>> Result" << std::endl;
	mem.dump(0, 0x80);
	std::cout << std::endl;
	mpu.dump();
	
	return 0;
}