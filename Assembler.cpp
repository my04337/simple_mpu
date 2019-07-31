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
Assembler& Assembler::add(RegWithAccess lhs, RegWithAccess rhs, Reg acc)
{
	return emit(prefix(OpCode::Add) | suffix<4>(lhs.to_bits(), 0) | suffix<4>(rhs.to_bits(), 4) | suffix<3>(acc, 8));
}
Assembler& Assembler::sub(RegWithAccess lhs, RegWithAccess rhs, Reg acc)
{
	return emit(prefix(OpCode::Sub) | suffix<4>(lhs.to_bits(), 0) | suffix<4>(rhs.to_bits(), 4) | suffix<3>(acc, 8));
}
Assembler& Assembler::mul(RegWithAccess lhs, RegWithAccess rhs, Reg acc)
{
	return emit(prefix(OpCode::Mul) | suffix<4>(lhs.to_bits(), 0) | suffix<4>(rhs.to_bits(), 4) | suffix<3>(acc, 8));
}
Assembler& Assembler::div(RegWithAccess lhs, RegWithAccess rhs, Reg acc)
{
	return emit(prefix(OpCode::Div) | suffix<4>(lhs.to_bits(), 0) | suffix<4>(rhs.to_bits(), 4) | suffix<3>(acc, 8));
}
Assembler& Assembler::mod(RegWithAccess lhs, RegWithAccess rhs, Reg acc)
{
	return emit(prefix(OpCode::Mod) | suffix<4>(lhs.to_bits(), 0) | suffix<4>(rhs.to_bits(), 4) | suffix<3>(acc, 8));
}
