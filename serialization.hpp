#ifndef QC_SERIALIZATION_HPPP
#define QC_SERIALIZATION_HPPP


#include "circuit_column.hpp"
#include "quantum_circuit.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, Biqubit const& b);
std::istream& operator>>(std::istream& is, Biqubit& b);

template <auto n>
std::ostream& operator<<(std::ostream& os, CircuitColumn<n> const& c) {
	os << c.is_monoqubit << " ";
	if (c.is_monoqubit) {
		for (auto const& g : c.gates)
			os << static_cast<unsigned>(g) << " ";
	} else {
		os << c.biqubit << " ";
	}
	return os;
}

template <auto n>
std::istream& operator>>(std::istream& is, CircuitColumn<n>& c) {
	bool tmp;
	is >> tmp;
	c.is_monoqubit = tmp;
	if (c.is_monoqubit) {
		for (auto& g : c.gates)
			is >> g;
	} else {
		is >> c.biqubit;
	}
	return is;
}


template <auto n, auto monoq_gates, auto dispatcher, auto max_size>
std::ostream& operator<<(std::ostream& os, QuantumCircuit<n, monoq_gates, dispatcher, max_size> const& qc) {
	for (auto const& c : qc)
		os << c << " ";
	return os;
}

template <auto n, auto monoq_gates, auto dispatcher, auto max_size>
std::istream& operator>>(std::istream& is, QuantumCircuit<n, monoq_gates, dispatcher, max_size>& qc) {
	CircuitColumn<n> col;
	while (is >> col)
		qc.cols.push_back(std::move(col));
	return is;
}

#endif
