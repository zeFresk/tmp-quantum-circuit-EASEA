#ifndef QC_EVALUATE_HPP
#define QC_EVALUATE_HPP

#include "matrix/initializers.hpp"

#include "circuit_column.hpp"
#include "kron_all.hpp"

#include <span>

static constexpr auto unk = -42.f;

template <auto n, typename Monos, typename Dispatcher>
inline std::remove_cvref_t<decltype(std::declval<Dispatcher>()(0, 0, 0))>
evaluate(CircuitColumn<n> const& col, Monos const& monos, Dispatcher const& dispatcher) {
	if (col.is_monoqubit) {
		return KronAll<n, n, Monos, decltype(col.gates)>::loop(monos, col.gates);
	} else {
		return dispatcher(col.biqubit.gate_id, col.biqubit.control, col.biqubit.target);
	}
}

template <auto basis, auto inputs, auto outputs, auto tmp, auto cost, auto zero_penalty, typename T>
float evaluate(Matrix<T, pow(basis, inputs + outputs + tmp), pow(basis, inputs + outputs + tmp)> const& result,
	       std::array<std::array<T, pow(2, outputs)>, pow(2, inputs)> const& map) {
	static constexpr auto total = pow(basis, inputs + outputs + tmp);
	static constexpr auto inc_inputs = pow(basis, tmp + outputs);
	static constexpr auto inc_outputs = pow(basis, tmp);

	float res = 0.f;

	for (int i = 0; i < total; i += inc_inputs) {
		int j;
		for (j = 0; j < i; j++) {
			res += zero_penalty(result(i, j));
		}
		for (; j < i + inc_inputs; ++j) {
			if ((j - i) % inc_outputs == 0) {
				res += cost(result(i, j), map[i / inc_inputs][(j - i) / inc_outputs]);
			} else {
				res += zero_penalty(result(i, j));
			}
		}
		for (; j < total; j++) {
			res += zero_penalty(result(i, j));
		}
	}

	return res;
}

template <auto basis, auto inputs, auto outputs, auto tmp, typename T>
constexpr Matrix<T, pow(basis, inputs + outputs + tmp), pow(basis, inputs + outputs + tmp)>
target_from_map(std::array<std::array<T, pow(2, outputs)>, pow(2, inputs)> const& map) {
	constexpr auto total = pow(basis, inputs + outputs + tmp);
	constexpr auto inc_inputs = pow(basis, tmp + outputs);
	constexpr auto inc_outputs = pow(basis, tmp);

	Matrix<T, pow(basis, inputs + outputs + tmp), pow(basis, inputs + outputs + tmp)> ret;
	for (auto& e : ret)
		e = T{unk};

	for (int i = 0; i < total; i += inc_inputs) {
		for (int j = i; j < i + inc_inputs; ++j) {
			if ((j - i) % inc_outputs == 0) {
				ret(i, j) = map[i / inc_inputs][(j - i) / inc_outputs];
			}
		}
	}

	return ret;
}

template <auto costf, typename T, auto n>
float evaluate(Matrix<T, n, n> const& qc, Matrix<T, n, n> const& target) {
	float res = 0.f;
	for (int i = 0; i < qc.lines(); ++i) {
		for (int j = 0; j < qc.columns(); ++j) {
			if (target(i, j) != unk)
				res += costf(qc(i, j), target(i, j));
		}
	}
	return res;
}

#endif
