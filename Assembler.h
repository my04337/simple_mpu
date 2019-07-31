#pragma once

#include "Memory.h"

#include <bitset>

namespace mpu
{
	using Byte = uint8_t;
	using Word = uint16_t;

	// ���߃v���t�B�b�N�X : 6bit (�T�t�B�b�N�X��10bit�܂�)
	enum class OpCode : Byte
	{
		Nop = 0x00,

		MoveRR  = 0x01, // suf(To(Reg[3] | Mod[2]) | From(Reg[3] | Mod[2])) - ���W�X�^(������)�����W�X�^(������)�ԓ]��
		MoveRC  = 0x02, // suf(To(Reg[3] | Mod[2]) From(Mod[2]) opc<0>(Val[16]) - ���l�����W�X�^(������)�ԓ]��		

		Halt = 0x3F,
	};

	// ���W�X�^�w�� : 3bit (+ 2bit)
	enum class Reg : Byte {
		R0 = 0, R1 = 1, R2 = 2, R3 = 3,
		R4 = 4, R5 = 5, R6 = 6, R7 = 7,
	};
	// move�C�� : 2bit
	enum class RegAccess : Byte
	{
		Reg   = 0x00, // ���C��(���W�X�^�ԍ�, ���l�Ȃ�)
		Far   = 0x01, // ���ڎw�� (�A�h���X��Ԃ�0x0000�`0x00FF�܂ł̃A�N�Z�X�\)
		Near  = 0x02, // �Ԑڎw�� : �Z�O�����g���W�X�^����̃I�t�Z�b�g�w��
		Base  = 0x03, // �Ԑڎw�� : �x�[�X���W�X�^����̃I�t�Z�b�g�w��
	};	

	// �A�Z���u�� : ���ߑg�ݗ��ăN���X
	class Assembler
	{
	public:
		Assembler(Memory& memory);

		Assembler& seek(size_t pos);

		Assembler& nop();
		Assembler& halt();
		
		Assembler& move(Reg to, Reg from) { return move(to, RegAccess::Reg, from, RegAccess::Reg); }
		Assembler& move(Reg to, RegAccess to_acc, Reg from, RegAccess from_acc);
		
		Assembler& move(Reg to, Word val) { return move(to, RegAccess::Reg, val); }
		Assembler& move(Reg to, RegAccess to_acc, Word val);
		
	private:
		Assembler& emit(Word v) { _mem.write<Word>(_pos, v); _pos += sizeof(Word); return *this; }

		static constexpr Word prefix(OpCode opc)noexcept { return static_cast<Word>(opc) << 10; }

		template<size_t bits, class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr Word suffix(T v, int offset) {
			Word mask = static_cast<Word>(std::bitset<bits>().flip().to_ulong());
			int shift = (10 - offset - bits);
			return static_cast<Word>(static_cast<Word>(v) & mask) << shift;
		}

	private:
		Memory& _mem;
		size_t _pos = 0;
	};

}