#ifndef QUANTUM_CIRCUIT_HPP
#define QUANTUM_CIRCUIT_HPP

#include "circuit_column.hpp"
#include "fastrand.hpp"
#include "kron_pow.hpp"

#include "matrix/initializers.hpp"

#include <vector>
#include <iostream>

static constexpr auto reserved_size = 16;
static constexpr auto biq_bias = 2; // Add chances to draw Biqubit gate
static constexpr auto prctDel = 1; // 1% of deletion

namespace helper
{

template <typename T>
struct infos {};

template <typename T, int l, int c>
struct infos<Matrix<T, l, c>> {
	static constexpr int lines = l;
	static constexpr int colons = c;
};
} // namespace helper

template <auto n, auto monoq_gates, auto dispatcher, auto max_size>
class QuantumCircuit {
	/*static_assert(kron_pow<n>(std::declval<decltype(*monoq_gates.begin())>()).columns() ==
			      dispatcher(0, 0, 0).columns(),
		      "Mono qubits and multi qubits gates don't match");
	static_assert(kron_pow<n>(std::declval<decltype(*monoq_gates.begin())>()).lines() ==
			      dispatcher(0, 0, 0).lines(),
		      "Mono qubits and multi qubits gates don't match");
		      */

    private:
	std::vector<CircuitColumn<n>> cols;

	inline void col_mutate(CircuitColumn<n>& c) {
		auto rd = fastrand<int, 0, monoq_gates + dispatcher.size() + biq_bias>();
		if (rd > monoq_gates) { // multi
			auto id = fastrand<int, 0, dispatcher.size() - 1>();
			auto ctrl = fastrand<int, 0, n - 1>();
			decltype(ctrl) target;
			while ((target = fastrand<int, 0, n - 1>()) == ctrl)
				;
			c.is_monoqubit = false;
			c.biqubit.gate_id = static_cast<unsigned char>(id);
			c.biqubit.control = static_cast<unsigned char>(ctrl);
			c.biqubit.target = static_cast<unsigned char>(target);
		} else {
			c.is_monoqubit = true;
			for (auto& g : c.gates)
				g = static_cast<unsigned char>(fastrand<int, 0, monoq_gates - 1>());
		}
	}

    public:
	QuantumCircuit() {
		cols.reserve(std::max(reserved_size, 4 * max_size));
		randomize(1, max_size / 2); //reserved_size);
	}

	using matrix_t = std::remove_cvref_t<decltype(dispatcher(0, 0, 0))>;

	auto size() const { return cols.size(); }

	std::size_t biqsize() const {
		std::size_t ret = 0;
		for (auto const& c : cols)
			if (!c.is_monoqubit)
				ret++;
		return ret;
	}

	decltype(cols.cbegin()) begin() const { return cols.cbegin(); }

	decltype(cols.cend()) end() const { return cols.cend(); }

	void randomize(int minlen, int maxlen) {
		auto len = fastrand<unsigned>(minlen, maxlen);
		cols.resize(len);
		for (auto& c : cols) {
			col_mutate(c);
		}
	}

	void mutate(int nb_muts) {
		for (int i = 0; i < nb_muts; ++i) {
			auto col = fastrand<std::size_t>(0, cols.size() - 1);
			auto mut_type = fastrand<int, 0, 2>(); // 2 types de mutation
			if (mut_type == 0) { // recreate column
				col_mutate(cols[col]);
			} else if (cols[col].is_monoqubit) { // small mutation
				auto g_idx = fastrand<int, 0, n - 1>();
				cols[col].gates[g_idx] = fastrand<int, 0, monoq_gates - 1>();
			} else { // biqubit gate: change target and control
				int new_ctrl = fastrand<int, 0, n - 1>();
				cols[col].biqubit.control = new_ctrl;
				int new_targ = 0;
				while ((new_targ = fastrand<int, 0, n - 1>()) == cols[col].biqubit.control)
					;
				cols[col].biqubit.target = new_targ;
			}
		}
	}

	void crossover(QuantumCircuit const& oth) {
		// inplace
		// 2 locus xover
		// so size can increase or decrease
		auto start1 = fastrand<unsigned>(0, cols.size() - 1);
		auto from1 = fastrand<unsigned>(0, cols.size() - start1);
		auto start2 = fastrand<unsigned>(0, oth.cols.size() - 1);
		auto from2 = fastrand<unsigned>(0, oth.cols.size() - start2);

		if (from1 == 0 && from2 == 0)
			from1 = 1;

		for (int i = 0; i < from1; ++i)
			cols[i] = cols[start1 + i];

		cols.resize(from1 + from2);

		for (int i = 0; i < from2; ++i)
			cols[from1 + i] = oth.cols[start2 + i];
		/*
		std::copy(oth.cols.begin() + start2, oth.cols.begin() + start2 + from2, std::back_inserter(cols));
		if (cols.size() > max_size)
			cols.resize(max_size);
		*/
	}

	template <typename Arr>
	matrix_t matrix(Arr const& mono_gates) const {
		using val_t = std::remove_cvref_t<decltype(mono_gates[0](0, 0))>;
		static constexpr int sz = helper::infos<matrix_t>::lines;
		Matrix<val_t, sz, sz> ret = ::identity<val_t, sz>();
		for (auto const& c : cols)
			ret = ret * evaluate(c, mono_gates, dispatcher);
		return ret;
	}

	//template <auto n, auto monoq_gates, auto dispatcher>
	friend std::istream& operator>>(std::istream& is, QuantumCircuit<n, monoq_gates, dispatcher, max_size>& qc) {
		CircuitColumn<n> col;
		while (is >> col)
			qc.cols.push_back(std::move(col));
		return is;
	}

	template <typename Array>
	void print(std::ostream& os, Array const& map) const {
		for (auto const& c : cols) {
			c.print(os, map);
			os << "\n";
		}
	}
};

#endif
