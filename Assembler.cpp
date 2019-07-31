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

Assembler& Assembler::move(Reg to, RegAccess to_acc, Reg from, RegAccess from_acc)
{
	return emit(prefix(OpCode::MoveRR) | suffix<3>(to, 0) | suffix<2>(to_acc, 3) | suffix<3>(from, 5) | suffix<2>(from_acc, 8));
}
Assembler& Assembler::move(Reg to, RegAccess to_acc, Word val)
{
	return emit(prefix(OpCode::MoveRC) | suffix<3>(to, 0) | suffix<2>(to_acc, 3)).emit(val);
}