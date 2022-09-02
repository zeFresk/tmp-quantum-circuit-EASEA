#ifndef QC_KRONALL_HPP
#define QC_KRONALL_HPP

#include "kron_pow.hpp"

template <int left, int n, typename GatesArray, typename IdxArray>
struct KronAll {
	/*using mono_t = decltype(std::declval<GatesArray>()[0]);
	using val_t = decltype(std::declval<mono_t>(0, 0));*/
	static constexpr decltype(kron_pow<left>(std::declval<typename GatesArray::value_type>())) loop(GatesArray const& all_gates, IdxArray const& tab) {
		return kron(KronAll<left - 1, n, GatesArray, IdxArray>::loop(all_gates, tab), all_gates[tab[left - 1]]);
	}
};

template <int n, typename GatesArray, typename IdxArray>
struct KronAll<1, n, GatesArray, IdxArray> {
	static constexpr decltype(kron_pow<1>(std::declval<typename GatesArray::value_type>())) loop(GatesArray const& all_gates, IdxArray const& tab) {
return all_gates[tab[0]];
	}
};

#endif
