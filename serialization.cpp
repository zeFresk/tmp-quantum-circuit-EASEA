#include "serialization.hpp"

std::ostream& operator<<(std::ostream& os, Biqubit const& b) {
	os << b.gate_id << " " << b.control << " " << b.target;
	return os;
}

std::istream& operator>>(std::istream& is, Biqubit& b) {
	is >> b.gate_id >> b.control >> b.target;
	return is;
}
