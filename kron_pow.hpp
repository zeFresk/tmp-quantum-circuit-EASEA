#ifndef KRON_POW_HPP
#define KRON_POW_HPP

#include "matrix.hpp"
#include "maths/pow.hpp"

template <int times, int size, typename T>
consteval Matrix<T, pow(size, times), pow(size, times)> kron_pow(Matrix<T, size, size> const& mat) {
	static_assert(times >= 0);
	if constexpr (times == 0) {
		return Matrix<T, 1, 1>{ 1.f };
	} else if constexpr (times == 1) {
		return mat;
	} else {
		return kron(kron_pow<times - 1>(mat), mat);
	}
}


#endif
