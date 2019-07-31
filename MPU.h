#pragma once

#include "Memory.h"
#include "Assembler.h"

#include <array>
#include <bitset>

namespace mpu 
{
	// 仮想マシンクラス
	class MPU
	{
	public:
		MPU(Memory& mem);

		void run();

		void step();

		void dump()const;

	private:
		static constexpr OpCode prefix(Word ins)noexcept { return static_cast<OpCode>(ins >> 10); }
		template<class T, size_t bits, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr T suffix(Word v, int offset) { 
			Word mask = static_cast<Word>(std::bitset<bits>().flip().to_ulong());
			int shift = (10 - offset - bits);
			return static_cast<T>((v >> shift) & mask);  
		}

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		T fetch() { T v{_mem.read<T>(_pc)}; _pc += sizeof(T); return v; }

		Word reg_read(RegWithAccess reg)const;

		void reg_write(RegWithAccess reg, Word val);

				
		void op_nop(Word ins);
		void op_halt(Word ins);
		void op_move_rr(Word ins);
		void op_move_rc(Word ins);

	private:
		Memory& _mem;
		std::array<Word, 8> _reg; // 汎用レジスタ
		Word _pc = 0; // プログラムカウンタ
		Word _sp = 0; // スタックポインタ
		Word _bp = 0; // ベースポインタ
		bool _halt = false;
	};
}