#pragma once
#include <cstdint>
#include <intrin.h>

class BitSet
{
public:
	BitSet(uint64_t a0, uint64_t a64, uint64_t a128) {
		a[0] = a0;
		a[1] = a64;
		a[2] = a128;
	}

	bool operator[](size_t pos) {
		return test(pos);
	}

	bool operator==(const BitSet &x) const {
		return a[0] == x.a[0] && a[1] == x.a[1] && a[2] == x.a[2];
	}

	BitSet operator>>(int x) const {
		return BitSet(a[1] << (64 - x) | a[0] >> x,
			a[2] << (64 - x) | a[1] >> x,
			a[2] >> x);
	}

	BitSet operator&(const BitSet &x) const {
		return BitSet(a[0] & x.a[0], a[1] & x.a[1], a[2] & x.a[2]);
	}

	BitSet operator|(const BitSet &x) const {
		return BitSet(a[0] | x.a[0], a[1] | x.a[1], a[2] | x.a[2]);
	}

	BitSet operator+(const BitSet &x) const {
		return BitSet(a[0] + x.a[0], a[1] + x.a[1], a[2] + x.a[2]);
	}

	operator bool() const {
		return (a[0] | a[1] | a[2]) != 0;
	}

	void set(int x) {
		if ((x >> 6) >= 3) abort();
		a[x >> 6] |= 1ULL << (x & 63);
	}

	bool test(int x) const {
		return a[x >> 6] & 1ULL << (x & 63);
	}

	unsigned int popcount64(uint64_t x) const {
		x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
		x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
		x = (x & 0x0F0F0F0F0F0F0F0FULL) + ((x >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		return (x * 0x0101010101010101ULL) >> 56;
	}

	int pop() const {
		return popcount64(a[0]) + popcount64(a[1]) + popcount64(a[2]);
	}

	union {
		uint64_t a[3];
		uint16_t b[12];
	};
};