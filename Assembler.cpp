#include "Assembler.h"

using namespace mpu;

Assembler::Assembler(Memory& memory) 
	: _mem(memory)
{
}

Assembler& Assembler::seek(size_t pos)
{
	_pos = pos;
	return *this;
}

Assembler& Assembler::nop()  
{
	return emit(prefix(OpCode::Nop)); 
}
Assembler& Assembler::halt() 
{ 
	return emit(prefix(OpCode::Halt)); 
}

Assembler& Assembler::move(RegWithAccess to, RegWithAccess from)
{
	return emit(prefix(OpCode::MoveRR) | suffix<4>(to.to_bits(), 0) | suffix<4>(from.to_bits(), 4));
}
Assembler& Assembler::move(RegWithAccess to, Word val)
{
	return emit(prefix(OpCode::MoveRC) | suffix<4>(to.to_bits(), 0)).emit(val);
}

Assembler& Assembler::move(RegWithAccess to, SpecialReg from)
{
	return emit(prefix(OpCode::MoveRS) | suffix<4>(to.to_bits(), 0) | suffix<3>(from, 4));
}