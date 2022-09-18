#include "serialization.hpp"

std::ostream& operator<<(std::ostream& os, Biqubit const& b) {
	os << static_cast<int>(b.gate_id) << " " << static_cast<int>(b.control) << " " << static_cast<int>(b.target);
	return os;
}

std::istream& operator>>(std::istream& is, Biqubit& b) {
	int gid, ctrl, targ;
	is >> gid >> ctrl >> targ;
	b.gate_id = gid;
	b.control = ctrl;
	b.target = targ;
	return is;
}
