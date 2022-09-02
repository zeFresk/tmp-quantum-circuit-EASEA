#ifndef QC_MAPPING_HPP
#define QC_MAPPING_HPP

#include "matrix/matrix.hpp"
#include "maths/pow.hpp"

template <auto inputs, auto outputs, auto tmp, typename T>
constexpr std::array<std::array<T, pow(2, outputs)>, pow(2, inputs)> make_map(std::array<std::array<T, pow(2, outputs)>, pow(2, inputs)> const& arr) {
	return arr;
}

#endif
