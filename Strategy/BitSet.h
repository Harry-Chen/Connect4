#pragma once
#include <cstdint>

struct BitSet {

//	union {
//		uint64_t a[3];
//		uint16_t b[12];
//	};
//
//public:
//
//	BitSet(uint64_t a0, uint64_t a64, uint64_t a128) {
//		a[0] = a0;
//		a[1] = a64;
//		a[2] = a128;
//	}
//
//	bool operator[](size_t pos) {
//		return test(pos);
//	}
//
//	bool operator==(const BitSet &x) const {
//		return a[0] == x.a[0] && a[1] == x.a[1] && a[2] == x.a[2];
//	}
//
//	BitSet operator>>(int x) const {
//		return BitSet(a[1] << (64 - x) | a[0] >> x,
//			a[2] << (64 - x) | a[1] >> x,
//			a[2] >> x);
//	}
//
//	BitSet operator&(const BitSet &x) const {
//		return BitSet(a[0] & x.a[0], a[1] & x.a[1], a[2] & x.a[2]);
//	}
//
//	BitSet operator|(const BitSet &x) const {
//		return BitSet(a[0] | x.a[0], a[1] | x.a[1], a[2] | x.a[2]);
//	}
//
//	BitSet operator+(const BitSet &x) const {
//		return BitSet(a[0] + x.a[0], a[1] + x.a[1], a[2] + x.a[2]);
//	}
//
//	operator bool() const {
//		return (a[0] | a[1] | a[2]) != 0;
//	}
//
//	void set(int x) {
//		a[x >> 6] |= 1ULL << (x & 63);
//	}
//
//	bool test(int x) const {
//		return a[x >> 6] & 1ULL << (x & 63);
//	}

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

	static bits and (const bits& a, const bits& b) {
		return bits{ a.a[0] & b.a[0], a.a[1] & b.a[1], a.a[2] & b.a[2] };
	}

	static bits or (const bits& a, const bits& b) {
		return bits{ a.a[0] | b.a[0], a.a[1] | b.a[1], a.a[2] | b.a[2] };
	}

	static bool notZero(const bits& a) {
		return (a.a[0] | a.a[1] | a.a[2]) != 0;
	}

};