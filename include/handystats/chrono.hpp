// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_HPP_
#define HANDYSTATS_CHRONO_HPP_

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <stdexcept>

namespace handystats { namespace chrono {

enum class time_unit {
	CYCLE,
	NSEC,
	USEC,
	MSEC,
	SEC,
	MIN,
	HOUR,
	DAY
};

inline
const char* time_unit_as_string(const time_unit& unit) {
	switch (unit) {
	case time_unit::CYCLE:
		return "c";
	case time_unit::NSEC:
		return "ns";
	case time_unit::USEC:
		return "us";
	case time_unit::MSEC:
		return "ms";
	case time_unit::SEC:
		return "s";
	case time_unit::MIN:
		return "m";
	case time_unit::HOUR:
		return "h";
	case time_unit::DAY:
		return "d";
	}

	return "";
}

inline
time_unit time_unit_from_string(const std::string& unit_str) {
	if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::CYCLE)) == 0) {
		return time_unit::CYCLE;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::NSEC)) == 0) {
		return time_unit::NSEC;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::USEC)) == 0) {
		return time_unit::USEC;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::MSEC)) == 0) {
		return time_unit::MSEC;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::SEC)) == 0) {
		return time_unit::SEC;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::MIN)) == 0) {
		return time_unit::MIN;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::HOUR)) == 0) {
		return time_unit::HOUR;
	}
	else if (strcmp(unit_str.c_str(), time_unit_as_string(time_unit::DAY)) == 0) {
		return time_unit::DAY;
	}
	else {
		throw std::logic_error("time_unit_from_string: unknown time_unit");
	}
}

enum class clock_type {
	INTERNAL, // epoch - unspecified point in time (usually boot-time), machine-specific
	SYSTEM // epoch - 1970 00:00:00 UT
};

}} // namespace handystats::chrono


namespace std {

template <>
struct less<handystats::chrono::time_unit> {
	bool operator() (const handystats::chrono::time_unit& x, const handystats::chrono::time_unit& y) {
		return precision_class(x) < precision_class(y);
	}

private:
	static unsigned precision_class(const handystats::chrono::time_unit& unit) {
		switch (unit) {
		case handystats::chrono::time_unit::CYCLE:
			return 1 << 0;
		case handystats::chrono::time_unit::NSEC:
			return 1 << 1;
		case handystats::chrono::time_unit::USEC:
			return 1 << 2;
		case handystats::chrono::time_unit::MSEC:
			return 1 << 3;
		case handystats::chrono::time_unit::SEC:
			return 1 << 4;
		case handystats::chrono::time_unit::MIN:
			return 1 << 5;
		case handystats::chrono::time_unit::HOUR:
			return 1 << 6;
		case handystats::chrono::time_unit::DAY:
			return 1 << 7;
		}

		return 0;
	}
};

} // namespace std


namespace handystats { namespace chrono {

struct duration;
struct time_point;

struct internal_clock {
	// will return time_point with INTERNAL clock type and CYCLE time unit
	static time_point now();
};

struct system_clock {
	// will return time_point with SYSTEM clock type and NSEC time unit
	static time_point now();
};

struct duration {
	static duration convert_to(const time_unit&, const duration&);

	duration();
	duration(const int64_t& d, const time_unit& u);
	duration(const duration&) = default;

	/* Assign operator */
	duration& operator= (const duration&) = default;

	/* Access ticks count and time unit */
	int64_t count() const;
	time_unit unit() const;

	/* Unary operators */
	duration operator+() const;
	duration operator-() const;

	/* Increment/decrement operators */
	duration& operator++();
	duration operator++(int);
	duration& operator--();
	duration operator--(int);

	/* Compound assignments */
	duration& operator+=(const duration& d);
	duration& operator-=(const duration& d);
	duration& operator%=(const duration& d);

	duration& operator+=(const int64_t& d);
	duration& operator-=(const int64_t& d);
	duration& operator*=(const int64_t& d);
	duration& operator/=(const int64_t& d);
	duration& operator%=(const int64_t& d);

	/* Arithmetic operations with duration */
	duration operator+(const duration& d) const;
	duration operator-(const duration& d) const;

	duration operator+(const int64_t& d) const;
	duration operator-(const int64_t& d) const;
	duration operator*(const int64_t& d) const;
	duration operator/(const int64_t& d) const;
	duration operator%(const int64_t& d) const;

	/* Comparison operators */
	bool operator==(const duration& d) const;
	bool operator!=(const duration& d) const;
	bool operator<(const duration& d) const;
	bool operator<=(const duration& d) const;
	bool operator>(const duration& d) const;
	bool operator>=(const duration& d) const;

	friend struct time_point;

private:
	int64_t m_rep;
	time_unit m_unit;
};

struct time_point {
	static time_point convert_to(const clock_type&, const time_point&);

	time_point();
	time_point(const duration& d, const clock_type& clock);

	duration time_since_epoch() const;

	/* Compound assignments */
	time_point& operator+=(const duration& d);
	time_point& operator-=(const duration& d);

	/* Arithmetic operations with duration */
	time_point operator+(const duration& d) const;
	time_point operator-(const duration& d) const;

	/* Arithmetic operations with time_point */
	duration operator-(const time_point& t) const;

	/* Comparison operators */
	bool operator==(const time_point& t) const;
	bool operator!=(const time_point& t) const;
	bool operator<(const time_point& t) const;
	bool operator<=(const time_point& t) const;
	bool operator>(const time_point& t) const;
	bool operator>=(const time_point& t) const;

private:
	duration m_since_epoch;
	clock_type m_clock;
};

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_HPP_
