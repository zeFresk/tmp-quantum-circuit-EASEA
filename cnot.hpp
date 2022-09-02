#ifndef QC_CNOT_HPP
#define QC_CNOT_HPP

#include "matrix/matrix.hpp"
#include "maths/pow.hpp"

#include "kron_pow.hpp"

#include <complex>

using gates_value_t = std::complex<float>;

/* Inneficient, for tests only ! */

static constexpr Matrix<gates_value_t, 4, 4> swapg{ { 1.f, 0.f, 0.f, 0.f },
					    { 0.f, 0.f, 1.f, 0.f },
					    { 0.f, 1.f, 0.f, 0.f },
					    { 0.f, 0.f, 0.f, 1.f } };
static constexpr Matrix<gates_value_t, 4, 4> cnotg{ { 1.f, 0.f, 0.f, 0.f },
					    { 0.f, 1.f, 0.f, 0.f },
					    { 0.f, 0.f, 0.f, 1.f },
					    { 0.f, 0.f, 1.f, 0.f } };
static constexpr Matrix<gates_value_t, 2, 2> idg{ 1, 0, 0, 1 };

enum class gates { id, swap, cnot };

template <int nbqubits, int control, int target>
consteval Matrix<gates_value_t, pow(2, nbqubits), pow(2, nbqubits)> cnot() {
	static_assert(control != target);
	static_assert(nbqubits >= 2);
	static_assert(control > 0 && target > 0);

	if constexpr (target == control + 1) { // next to each other
		auto pre = kron_pow<control - 1>(idg);
		auto gate = cnotg;
		auto post = kron_pow<nbqubits - target>(idg);
		return kron(pre, gate, post);
	} else if constexpr (control == target + 1) { // switched
		auto pre = kron_pow<target - 1>(idg);
		auto gate = swapg;
		auto post = kron_pow<nbqubits - control>(idg);
		return kron(pre, gate, post) * cnot<nbqubits, target, control>() * kron(pre, gate, post);
	} else if constexpr (target > control) {
		auto until_control = kron_pow<control>(idg);
		auto control_to_target = kron_pow<target - control - 2>(idg);
		auto gate = swapg;
		auto post = kron_pow<nbqubits - target>(idg);
		return kron(until_control, control_to_target, gate, post) * cnot<nbqubits, control, target - 1>() * kron(until_control, control_to_target, gate, post);
	} else if constexpr (control > target) {
		auto until_target = kron_pow<target>(idg);
		auto target_to_control = kron_pow<control - target - 2>(idg);
		auto gate = swapg;
		auto post = kron_pow<nbqubits - control>(idg);
		return kron(until_target, target_to_control, gate, post) * cnot<nbqubits, control - 1, target>() * kron(until_target, target_to_control, gate, post);
	}
}

template <int i, int j, int n, int k>
consteval void internal_for(std::array<Matrix<gates_value_t, pow(2, n), pow(2, n)>, n*(n - 1)>& ret) {
	if constexpr (i == j) {
		internal_for<i, j + 1, n, k>(ret);
	} else if constexpr (j >= n) {
		internal_for<i + 1, 0, n, k>(ret);
	} else if constexpr (i >= n) {
		return;
	} else {
		ret[k] = cnot<n, i + 1, j + 1>();
		internal_for<i, j + 1, n, k + 1>(ret);
	}
}

template <int n>
consteval std::array<Matrix<gates_value_t, pow(2, n), pow(2, n)>, n*(n - 1)> all_cnots() {
	std::array<Matrix<gates_value_t, pow(2, n), pow(2, n)>, n*(n - 1)> ret;
	internal_for<0, 0, n, 0>(ret);
	return ret;
}

template <int n>
struct GlobalCnots {
	static constexpr auto cnots = all_cnots<n>();
	Matrix<gates_value_t, pow(2, n), pow(2, n)> const& operator()(int control, int target) const {
		int k = 0;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (i == j)
					continue;
				if (i == control && target == j)
					return cnots[k];
				k++;
			}
		}
		return cnots[-1];
	}
};

template <int n>
constexpr Matrix<gates_value_t, pow(2, n), pow(2, n)> const& cnot(int control, int target) {
	return GlobalCnots<n>{}(control, target);
}

#endif
