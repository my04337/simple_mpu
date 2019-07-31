#include "MPU.h"

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace mpu;


MPU::MPU(Memory& mem) 
	: _mem(mem) 
{
	_reg.fill(0x0000); 

	_sp = _bp = static_cast<Word>(mem.size()-1);
}

void MPU::run() 
{
	while (!_halt) {
		step();
	}
}
void MPU::step() 
{
	auto ins = fetch<Word>();
	auto op = prefix(ins);
	switch (op) {
	case OpCode::Nop: op_nop(ins); break;
	case OpCode::Halt: op_halt(ins); break;
	case OpCode::MoveRR: op_move_rr(ins); break;
	case OpCode::MoveRC: op_move_rc(ins); break;
	case OpCode::Add: op_add(ins); break;
	case OpCode::Sub: op_sub(ins); break;
	case OpCode::Mul: op_mul(ins); break;
	case OpCode::Div: op_div(ins); break;
	case OpCode::Mod: op_mod(ins); break;
	}
}

void MPU::dump()const
{
	std::cout 
		<< "PC:" << std::hex << std::setw(4) << std::setfill('0') << _pc << " "
		<< "SP:" << std::hex << std::setw(4) << std::setfill('0') << _sp << " "
		<< "BP:" << std::hex << std::setw(4) << std::setfill('0') << _bp << std::endl;
	for(size_t i=0; i<_reg.size(); ++i) {
		std::cout << "R" << i << ":" << std::hex << std::setw(4) << std::setfill('0') << _reg[i] << " ";
	}
	std::cout << std::endl;
}
Word MPU::reg_read(RegWithAccess reg)const
{
	Word reg_value = _reg[static_cast<size_t>(reg.reg)];
	if (reg.indirect) {
		return _mem.read<Word>(reg_value);
	} else {
		return reg_value;
	}
}
void MPU::reg_write(RegWithAccess reg, Word val)
{
	Word& reg_value = _reg[static_cast<size_t>(reg.reg)];
	if (reg.indirect) {
		_mem.write<Word>(reg_value, val);
	} else {
		reg_value = val;
	}
}


void MPU::op_nop(Word ins) 
{
	// do-nothing
}
void MPU::op_halt(Word ins) 
{
	_halt = true;
}
void MPU::op_move_rc(Word ins)
{
	auto to = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));

	auto val = fetch<Word>();

	
	reg_write(to, val);
}
void MPU::op_move_rr(Word ins)
{
	auto to = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto from = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	
	auto val = reg_read(from);
	
	reg_write(to, val);
}
void MPU::op_add(Word ins)
{
	auto lhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto rhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	auto acc = suffix<Reg, 3>(ins, 8);

	auto val = reg_read(lhs) + reg_read(rhs);
	reg_write(acc, val);
}
void MPU::op_sub(Word ins)
{
	auto lhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto rhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	auto acc = suffix<Reg, 3>(ins, 8);

	auto val = reg_read(lhs) - reg_read(rhs);
	reg_write(acc, val);
}
void MPU::op_mul(Word ins)
{
	auto lhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto rhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	auto acc = suffix<Reg, 3>(ins, 8);

	auto val = reg_read(lhs) * reg_read(rhs);
	reg_write(acc, val);
}
void MPU::op_div(Word ins)
{
	auto lhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto rhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	auto acc = suffix<Reg, 3>(ins, 8);

	auto val = reg_read(lhs) / reg_read(rhs);
	reg_write(acc, val);
}
void MPU::op_mod(Word ins)
{
	auto lhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 0));
	auto rhs = RegWithAccess::from_bits(suffix<Byte, 4>(ins, 4));
	auto acc = suffix<Reg, 3>(ins, 8);

	auto val = reg_read(lhs) % reg_read(rhs);
	reg_write(acc, val);
}