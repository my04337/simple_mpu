#pragma once

#include "Memory.h"

#include <bitset>

namespace mpu
{
	using Byte = uint8_t;
	using Word = uint16_t;

	// 命令プレフィックス : 5bit (サフィックスは11bitまで)
	enum class OpCode : Byte
	{
		Nop = 0x00,

		MoveRR  = 0x01, // suf:To(Reg+Access[3+1])|From(Reg+Access[3+1]) - 汎用レジスタ→汎用レジスタ間転送
		MoveRC  = 0x02, // suf:To(Reg+Access[3+1] & opc<0>(Val[16]) - 即値→汎用レジスタ間転送		

		Add = 0x08,  // suf:lhs(Reg+Access[3+1])|rhs(Reg+Access[3+1])|acc(Reg[3])
		Sub = 0x09,  // suf:lhs(Reg+Access[3+1])|rhs(Reg+Access[3+1])|acc(Reg[3])
		Mul = 0x0A,  // suf:lhs(Reg+Access[3+1])|rhs(Reg+Access[3+1])|acc(Reg[3])
		Div = 0x0B,  // suf:lhs(Reg+Access[3+1])|rhs(Reg+Access[3+1])|acc(Reg[3])
		Mod = 0x0C,  // suf:lhs(Reg+Access[3+1])|rhs(Reg+Access[3+1])|acc(Reg[3])

		Halt = 0x1F,
	};

	// 汎用レジスタ指定 : 3bit
	enum class Reg : Byte {
		R0 = 0, R1 = 1, R2 = 2, R3 = 3,
		R4 = 4, R5 = 5, R6 = 6, R7 = 7,
	};

	// 汎用レジスタ 直接/間接アクセス指定 (3 + 1bit)
	struct RegWithAccess {
		Reg reg;
		bool indirect;
		constexpr RegWithAccess(Reg reg, bool indirect = false) : reg(reg), indirect(indirect) {}

		constexpr Byte to_bits()const noexcept { return static_cast<Byte>((static_cast<Byte>(reg) << 1) | ( indirect ? 0x01 : 0x00)); }
		static constexpr RegWithAccess from_bits(Word bits) { return {static_cast<Reg>((bits & 0x0E) >> 1), ((bits & 0x01) ? true : false)}; }

	};
	inline constexpr RegWithAccess RegIndirect(Reg reg) { return {reg, true}; }

	// 汎用レジスタ修飾 : 1bit
	enum class RegAccess : Byte
	{
		Reg  = 0x00, // 無修飾(レジスタ番号, 即値など)
		Addr = 0x01, // 直接指定 (アドレス空間の0x0000～0x00FFまでのアクセス可能)
	};	

	// アセンブラ : 命令組み立てクラス
	class Assembler
	{
	public:
		Assembler(Memory& memory);

		Assembler& seek(size_t pos);

		Assembler& nop();
		Assembler& halt();

		Assembler& move(RegWithAccess to, Word val);
		Assembler& move(RegWithAccess to, RegWithAccess from);

		Assembler& add(RegWithAccess lhs, RegWithAccess rhs, Reg acc);
		Assembler& sub(RegWithAccess lhs, RegWithAccess rhs, Reg acc);
		Assembler& mul(RegWithAccess lhs, RegWithAccess rhs, Reg acc);
		Assembler& div(RegWithAccess lhs, RegWithAccess rhs, Reg acc);
		Assembler& mod(RegWithAccess lhs, RegWithAccess rhs, Reg acc);

		
	private:
		Assembler& emit(Word v) { _mem.write<Word>(_pos, v); _pos += sizeof(Word); return *this; }

		static constexpr Word prefix(OpCode opc)noexcept { return static_cast<Word>(opc) << 11; }

		template<size_t bits, class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr Word suffix(T v, int offset) {
			Word mask = static_cast<Word>(std::bitset<bits>().flip().to_ulong());
			int shift = (11 - offset - bits);
			return static_cast<Word>(static_cast<Word>(v) & mask) << shift;
		}

	private:
		Memory& _mem;
		size_t _pos = 0;
	};

}