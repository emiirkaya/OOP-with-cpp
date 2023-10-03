#include "Date.h"
#include <exception>
#include <tuple>
#include <random>
class bad_date : public std::exception {
public:
	const char* what()const override {
		return "bad date!";
	}
};
namespace project {
	constexpr bool Date::isleap(int y)
	{
		return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
	}
	constexpr bool Date::isvalid(int d, int m, int y)
	{
		return y >= yearbase && m > 0 && m <= 12 && d > 0 && d <= daytabs[static_cast<int>(isleap(y))][m];
	}
	int Date::totaldays()const
	{
		return (y_ - 1900) * 365 + (y_ - 1901) / 4 + get_year_day();
	}
	Date& Date::set_month_day(int day)
	{
		d_ = day;
		return *this;
	}
	Date& Date::set_month(int month)
	{
		m_ = month;
		return *this;
	}
	Date& Date::set_year(int year)
	{
		y_ = year;
		return *this;
	}
	Date& Date::set(int d, int m, int y)
	{
		if (!isvalid(d, m, y))
			throw bad_date{};
		d_ = d, m_ = m, y_ = y;
		return *this;
	}
	Date::Date(int d, int m, int y)
	{
		set(d, m, y);
	}
	Date::Date(const char* p)
	{
		set(atoi(p), atoi(p + 3), atoi(p + 6));
	}
	int Date::get_month_day() const
	{
		return d_;
	}
	int Date::get_month() const
	{
		return m_;
	}
	int Date::get_year() const
	{
		return y_;
	}
	int Date::get_year_day() const
	{
		int sum = d_;
		for (int i = 1; i < m_; ++i)
			sum += daytabs[static_cast<int>(isleap(y_))][i];
		return sum;
	}
	bool operator<(const Date& left, const Date& right)
	{
		return std::tie(left.y_,left.m_,left.d_) < std::tie(right.y_, right.m_, right.d_);
	}
	bool operator==(const Date& left, const Date& right)
	{
		return std::tie(left.y_, left.m_, left.d_) == std::tie(right.y_, right.m_, right.d_);
	}
	Date& Date::operator++()
	{
		if (d_ < 28)
			++d_;

		else if (d_ == 31) {
			d_ = 1;
			m_ = m_ == 12 ? 1 : m_ + 1;
			if (m_ == 1)
				++y_;
		}
		else if (d_ == 30 && (m_ == 4 || m_ == 6 || m_ == 9 || m_ == 11)) {
			d_ = 1;
			++m_;
		}
		else if (d_ == 29) {
			if (m_ == 2) {
				d_ = 1, m_ = 3;
			}
			else
				++d_;
		}
		else {
			d_ = m_ == 2 ? (isleap(y_) ? 29 : 1) : d_ + 1;
			if (d_ == 1)
				++m_;
		}

		return *this;
	}
	Date Date::operator++(int)
	{
		Date temp{ *this };
		++* this;
		return temp;
	}
	Date& Date::operator--()
	{
		int d = d_, m = m_, y = y_;

		if (d != 1)
			--d;
		else if (m != 1) {
			--m;
			d = daytabs[static_cast<int>(isleap(y))][m];
		}
		else {
			if (y == yearbase)
				throw bad_date{};
			m = 12;
			d = 31;
			--y;
		}
		d_ = d;
		m_ = m;
		y_ = y;

		return *this;
	}
	Date Date::operator--(int)
	{
		Date temp{ *this };
		--*this;
		return temp;
	}
	Date Date::random_date()
	{
		std::mt19937 eng{ std::random_device{}() };
		std::uniform_int_distribution<int> m_dist{ random_min_year, random_max_year };
		return Date(1,1, m_dist(eng));
	}
	bool operator>(const Date& left, const Date& right)
	{
		return right < left;
	}
	bool operator>=(const Date& left, const Date& right)
	{
		return !(left < right);
	}
	bool operator<=(const Date& left, const Date& right)
	{
		return !(right < left);
	}
	bool operator!=(const Date& left, const Date& right)
	{
		return !(left == right);
	}
}
