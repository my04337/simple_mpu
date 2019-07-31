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

		MoveRR  = 0x01, // suf:To(Reg+Access[3+2])|From(Reg+Access[3+2]) - �ėp���W�X�^���ėp���W�X�^�ԓ]��
		MoveRC  = 0x02, // suf:To(Reg+Access[3+2] & opc<0>(Val[16]) - ���l���ėp���W�X�^�ԓ]��		

		MoveRS  = 0x03, // suf:To(Reg+Access[3+2])|From(SpecialReg[3]) - ���ꃌ�W�X�^���ėp���W�X�^�ԓ]��

		Halt = 0x3F,
	};

	// �ėp���W�X�^�w�� : 3bit
	enum class Reg : Byte {
		R0 = 0, R1 = 1, R2 = 2, R3 = 3,
		R4 = 4, R5 = 5, R6 = 6, R7 = 7,
	};

	// �ėp���W�X�^ ����/�ԐڃA�N�Z�X�w�� (3 + 1bit)
	struct RegWithAccess {
		Reg reg;
		bool indirect;
		constexpr RegWithAccess(Reg reg, bool indirect = false) : reg(reg), indirect(indirect) {}

		constexpr Byte to_bits()const noexcept { return static_cast<Byte>((static_cast<Byte>(reg) << 1) | ( indirect ? 0x01 : 0x00)); }
		static constexpr RegWithAccess from_bits(Word bits) { return {static_cast<Reg>((bits & 0x0E) >> 1), ((bits & 0x01) ? true : false)}; }

	};
	inline constexpr RegWithAccess RegIndirect(Reg reg) { return {reg, true}; }

	// �ėp���W�X�^�C�� : 1bit
	enum class RegAccess : Byte
	{
		Reg  = 0x00, // ���C��(���W�X�^�ԍ�, ���l�Ȃ�)
		Addr = 0x01, // ���ڎw�� (�A�h���X��Ԃ�0x0000�`0x00FF�܂ł̃A�N�Z�X�\)
	};	

	// ���ꃌ�W�X�^�w�� : 3bit (+ 2bit)
	enum class SpecialReg : Byte {
		DS = 0, // �f�[�^�Z�O�����g
	};

	// �A�Z���u�� : ���ߑg�ݗ��ăN���X
	class Assembler
	{
	public:
		Assembler(Memory& memory);

		Assembler& seek(size_t pos);

		Assembler& nop();
		Assembler& halt();

		Assembler& move(RegWithAccess to, Word val);
		Assembler& move(RegWithAccess to, RegWithAccess from);
		Assembler& move(RegWithAccess to, SpecialReg from);

		
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