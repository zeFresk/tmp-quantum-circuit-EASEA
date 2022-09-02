#ifndef UNITARY_GATES_HPP
#define UNITARY_GATES_HPP

#include "matrix/matrix.hpp"
#include "matrix/initializers.hpp"
#include "matrix/kronecker.hpp"
#include "matrix/product.hpp"
#include "maths/sqrt.hpp"

#include <complex>
#include <array>
#include <string_view>
#include <span>

using std::complex_literals::operator""if;

using Gate = Matrix<std::complex<float>, 2, 2>;
using G2 = decltype(kron(std::declval<Gate>(), std::declval<Gate>()));

static constexpr float inv_root2 = 1.f / sqrt<7>(2.f);

static constexpr Gate ug_i{ 1.f, 0.f, 0.f, 1.f };
static constexpr Gate ug_x{ 0.f, 1.f, 1.f, 0.f };
static constexpr Gate ug_y{ 0.f, -1.if, 1if, 0.f };
static constexpr Gate ug_z{ 1.f, 0.f, 0.f, -1.f };
static constexpr Gate ug_h{ inv_root2 * 1.f, inv_root2 * 1.f, inv_root2 * 1.f, inv_root2 * -1.f };
static constexpr Gate ug_s{ 1.f, 0.f, 0.f, 1.if };
static constexpr Gate ug_t{ 1.f, 0.f, 0.f, std::complex<float>(0.707106781f, 0.707106781f) };
static constexpr Gate ug_tdg{ 1.f, 0.f, 0.f, std::complex<float>(0.707106781f, -0.707106781f) };

static constexpr std::array<Gate, 5> all_gates_mono{ ug_i, ug_x, ug_y, ug_z, ug_h };
static constexpr std::array<std::string_view, all_gates_mono.size()> all_gates_mono_str{ "I", "X", "Z", "Y", "H" };

template <typename T, int n>
bool is_id(Matrix<T, n, n> const& mat) {
	static constexpr auto id = ::identity<T, n>();
	return std::equal(mat.begin(), mat.end(), id.begin(),
		   [](auto const& lhs, auto const& rhs) { return std::norm(lhs - rhs) < 1e-3; });
}

/*static constexpr G2 cx{ { 1.f, 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f, 0.f }, { 0.f, 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f, 0.f } };
static constexpr G2 cz{ { 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, -1.f } };
static constexpr G2 swapg{ { 1.f, 0.f, 0.f, 0.f },
			  { 0.f, 0.f, 1.f, 0.f },
			  { 0.f, 1.f, 0.f, 1.f },
			  { 0.f, 0.f, 0.f, 1.f } };

static constexpr std::array<G2, 3> all_gates_bi{ cx, cz, swapg };*/

/*template <int N>
struct Basis {
	std::array<G2, N> gates;
	std::array<std::array<std::string_view, 2>, N> names;
};

template <auto mono, auto bi>
constexpr std::array<G2, pow(mono, 2) + bi> gen_basis(std::array<Gate, mono> const& monogates, std::array<G2, bi> const& bigates) {
	constexpr int size = pow(mono, 2) + bi;
	std::array<G2, size> ret;
	int k = 0;
	for (int i = 0; i < mono; ++i) {
		for (int j = 0; j < mono; ++j) {
			ret[k++] = kron(monogates[i], monogates[j]);
		}
	}
	for (int i = 0; i < bi; ++i)
		ret[k++] = bigates[i];
	return ret;
}

template <auto mono, auto bi>
constexpr std::array<std::array<std::string_view, 2>, pow(mono, 2) + bi> gen_basis_str(std::array<std::string_view, mono> const& monogates,
										       std::array<std::string_view, bi> const& bigates) {
	constexpr int size = pow(mono, 2) + bi;
	std::array<std::array<std::string_view, 2>, size> ret;
	int k = 0;
	for (int i = 0; i < mono; ++i) {
		for (int j = 0; j < mono; ++j) {
			ret[k++] = { monogates[i], monogates[j] };
		}
	}
	for (int i = 0; i < bi; ++i)
		ret[k++] = { bigates[i], "" };
	return ret;
}*/

#endif
