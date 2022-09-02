#ifndef QC_COLUMN_HPP
#define QC_COLUMN_HPP

#include <array>
#include <iostream>
#include <tuple>

struct Biqubit { // 24 bits
	unsigned char gate_id; // 0 -> 255
	unsigned char control; // 0 -> 255
	unsigned char target; // 0 -> 255
};

template <auto n>
struct CircuitColumn { // 32 bits ? 64 bits ?
	bool is_monoqubit : 1;
	union {
		Biqubit biqubit;
		std::array<unsigned char, n> gates;
	};

	template <typename Array>
	void print(std::ostream& os, Array const& map) const {
		if (is_monoqubit) {
			for (int i = 0; i < n; ++i) {
				if (i > 0)
					os << " x ";
				os << std::get<0>(map)[gates[i]];
			}
		} else {
			os << std::get<1>(map)[biqubit.gate_id] << "(ctrl=" << static_cast<int>(biqubit.control) << ", targ=" << static_cast<int>(biqubit.target) << ")";
		}
	}
};

#endif
