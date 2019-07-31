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
	}
}

void MPU::dump()const
{
	std::cout 
		<< "PC:" << std::hex << std::setw(4) << std::setfill('0') << _pc << " "
		<< "SR:" << std::hex << std::setw(4) << std::setfill('0') << _seg << " "
		<< "SP:" << std::hex << std::setw(4) << std::setfill('0') << _sp << " "
		<< "BP:" << std::hex << std::setw(4) << std::setfill('0') << _bp << std::ends;
	for(size_t i=0; i<_reg.size(); ++i) {
		std::cout << "R" << i << ":" << std::hex << std::setw(4) << std::setfill('0') << _reg[i] << " ";
	}
	std::cout << std::endl;
}
Word MPU::reg_read(Reg reg, RegAccess access)const
{
	Word reg_value = _reg[static_cast<size_t>(reg)];
	switch (access) {
	case RegAccess::Reg:  return reg_value;
	case RegAccess::Far:  return _mem.read<Word>(reg_value);
	case RegAccess::Near: return _mem.read<Word>(_seg + reg_value);
	case RegAccess::Base: return _mem.read<Word>(_bp + reg_value);
	}
	assert(false);
}
void MPU::reg_write(Reg reg, RegAccess access, Word val)
{
	Word& reg_value = _reg[static_cast<size_t>(reg)];
	switch (access) {
	case RegAccess::Reg:  reg_value = val; break;
	case RegAccess::Far:  _mem.write<Word>(reg_value, val); break;
	case RegAccess::Near: _mem.write<Word>(_seg + reg_value, val); break;
	case RegAccess::Base: _mem.write<Word>(_bp + reg_value, val); break;
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
	auto to = suffix<Reg, 3>(ins, 0);
	auto to_acc = suffix<RegAccess, 2>(ins, 3);

	auto val = fetch<Word>();

	
	reg_write(to, to_acc, val);
}
void MPU::op_move_rr(Word ins)
{
	auto to = suffix<Reg, 3>(ins, 0);
	auto to_acc = suffix<RegAccess, 2>(ins, 3);
	auto from = suffix<Reg, 3>(ins, 5);
	auto from_acc = suffix<RegAccess, 2>(ins, 8);

	auto val = reg_read(from, from_acc);
	
	reg_write(to, to_acc, val);
}