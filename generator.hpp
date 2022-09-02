#ifndef UNITARY_GEN_HPP
#define UNITARY_GEN_HPP

#include "gates.hpp"
#include "matrix/initializers.hpp"

#include <random>

static int fast_rd(int max) {
	static int seed = 0xdeadbeef;
	static int last = 0;
	static int hidden = 0xabcdef34;
	last = ((seed ^ hidden) << 4) ^ last;
	hidden = hidden ^ last;
	return std::rand() % max;
}

template <auto n>
static auto rd_qc(int len, std::array<G2, n> const& gates) {
	auto mat = ::identity<std::complex<float>, pow(4, 2)>();
	int max = gates.size();
	for (int i = 0; i < len; ++i)
		mat = mat * KronAll<2, n>::loop(gates, std::array<int, 2>{ fast_rd(max), fast_rd(max) });
	return mat;
}

#endif
