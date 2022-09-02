#ifndef QC_DISPATCHER_HPP
#define QC_DISPATCHER_HPP

#include <cassert>


template <auto f, typename... Args>
decltype(f(0, 1)) dispatch(int gate, int control, int target) {
	if constexpr (sizeof...(Args) >= 1) {
		return (gate == 0) ? f(control, target) : dispatch<Args...>(gate-1, control, target);
	} else {
		assert(gate == 0 && "bad gate id");
		return f(control, target);
	}
}

template <auto f, typename... Args>
struct Dispatcher {
	inline decltype(f(0, 1)) operator()(int gate, int control, int target) const {
		assert(gate >= 0 && gate < size() && "bad gate id");
		return dispatch<f, Args...>(gate, control, target);
	}
	static constexpr auto size() {
		return sizeof...(Args) + 1;
	}
};

#endif
