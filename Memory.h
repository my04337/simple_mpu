#pragma once
#include <vector>

namespace mpu {
	// メインメモリ
	class Memory 
	{
	public:
		Memory() { }
		Memory(size_t size) { _bytes.resize(size); }

		size_t size()const noexcept { return _bytes.size(); }

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		Memory& write(size_t pos, T v);

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		T read(size_t pos) const;



		void dump(size_t offset, size_t size);

	private :
		template<class T, typename std::enable_if_t<std::is_scalar_v<T> && sizeof(T) < sizeof(uint64_t)>* = nullptr>
		static constexpr T shift_byte(T v, int from, int to);

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr T swap_endian(T v);

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr T to_big_endian(T v);

		template<class T, typename std::enable_if_t<std::is_scalar_v<T>>* = nullptr>
		static constexpr T from_big_endian(T v);

	private:
		std::vector<uint8_t> _bytes;
	};


	template<class T, typename std::enable_if_t<std::is_scalar_v<T>>*>
	Memory& Memory::write(size_t pos, T v)
	{ 
		v = to_big_endian(v);
		memcpy(&_bytes[pos], &v, sizeof(T));
		return *this; 
	}

	template<class T, typename std::enable_if_t<std::is_scalar_v<T>>*>
	T Memory::read(size_t pos) const
	{ 
		T v;
		memcpy(&v, &_bytes[pos], sizeof(T));
		v = from_big_endian(v);
		return v; 
	}


	template<class T, typename std::enable_if_t<std::is_scalar_v<T> && sizeof(T) < sizeof(uint64_t)>*>
	static constexpr T Memory::shift_byte(T v, int from, int to) 
	{
		return static_cast<T>(((static_cast<uint64_t>(v) >> from*8) & 0xFF) << to*8);
	}

	template<class T, typename std::enable_if_t<std::is_scalar_v<T>>*>
	static constexpr T Memory::swap_endian(T v) 
	{ 
		if constexpr(sizeof(T) == 1) return v;
		else if constexpr(sizeof(T) == 2) return shift_byte(v, 0, 1) | shift_byte(v, 1, 0);
		else if constexpr(sizeof(T) == 4) return shift_byte(v, 0, 3) | shift_byte(v, 1, 2) | shift_byte(v, 2, 1) | shift_byte(v, 3, 0);
		else static_assert(false, "Unsupported data size");
	}

	template<class T, typename std::enable_if_t<std::is_scalar_v<T>>*>
	static constexpr T Memory::to_big_endian(T v) 
	{ 
		// TODO マシンがリトルエンディアンであることが前提
		return swap_endian(v);
	}

	template<class T, typename std::enable_if_t<std::is_scalar_v<T>>*>
	static constexpr T Memory::from_big_endian(T v) 
	{ 
		// TODO マシンがリトルエンディアンであることが前提
		return swap_endian(v);
	}

}