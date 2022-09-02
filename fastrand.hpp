#ifndef FASTRAND_HPP
#define FASTRAND_HPP

#include <random>
#include <limits>

inline std::minstd_rand& global_engine() {
	static std::minstd_rand gen(std::random_device{}());
	return gen;
};

template <typename T, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()>
T fastrand() {
	static std::uniform_int_distribution<T> dis(min, max);
	return dis(global_engine());
}

template <typename T>
T fastrand(int min, int max) {
	std::uniform_int_distribution<T> dis(min, max);
	return dis(global_engine());
}

#endif
