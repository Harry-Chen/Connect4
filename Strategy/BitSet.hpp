#ifndef BITSET_HPP
#define BITSET_HPP
#include <cstdint>

struct BitSet {

	typedef union {
		struct {
			uint64_t a0;
			uint64_t a1;
			uint64_t a2;
		} raw_data;
		uint64_t a[3];
	} bits;

	static bits getBits(uint64_t a0, uint64_t a64, uint64_t a128) {
		return bits{ a0, a64, a128 };
	}

	static void set(bits &data, size_t pos) {
		data.a[pos >> 6] |= 1ULL << (pos & 63);
	}

	static bool test(const bits& data, size_t pos) {
		return data.a[pos >> 6] & 1ULL << (pos & 63);
	}

	static bits rightShift(const bits& data, int x) {
		return bits{ data.a[1] << (64 - x) | data.a[0] >> x,
			data.a[2] << (64 - x) | data.a[1] >> x,
			data.a[2] >> x };
	}

	static bits andWith(const bits& a, const bits& b) {
		return bits{ a.a[0] & b.a[0], a.a[1] & b.a[1], a.a[2] & b.a[2] };
	}

	static bits orWith(const bits& a, const bits& b) {
		return bits{ a.a[0] | b.a[0], a.a[1] | b.a[1], a.a[2] | b.a[2] };
	}

	static bool notZero(const bits& a) {
		return (a.a[0] | a.a[1] | a.a[2]) != 0;
	}

};

#endif