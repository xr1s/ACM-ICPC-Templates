#include <stdexcept>

// Gregorian calender, saved in julian day number.
class Date {
 public:
  enum Weekday {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Firday, Saturday,
  };
  enum Month {
    January = 1, February, March, April, May, June,
    July, August, September, October, November, December,
  };
  Date(int, Month = January, int = 1);
  Date(const Date &);

  int julian() const;
  int year() const;
  Month month() const;
  int day() const;
  Weekday weekday() const;

  friend Date &operator++(Date &);
  friend Date &operator--(Date &);
  friend Date &operator+=(Date &, int);
  friend Date &operator-=(Date &, int);

  static bool isLeapYear(int);
  static int dayInMonth(int, Month);

 private:
  unsigned int jdn;  // Julian Day Number
  const static int monthLen_[];
  const static int y, j, m, n, r, p, v, u, s, w, B, C;
};
const int Date::monthLen_[] = {
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
};
// Constant variables for converting Julian day number to Gregorian
// calendar, please refer to https://en.wikipedia.org/wiki/Julian_day
const int Date::y = 4716, Date::j = 1401, Date::m = 2, Date::n = 12,
          Date::r = 4, Date::p = 1461, Date::v = 3, Date::u = 5,
          Date::s = 153, Date::w = 2, Date::B = 274277, Date::C = -38;

Date::Date(const Date &that)
    : jdn(that.jdn) {
}

Date::Date(int year, Month month, int day) {
  if (year == 0)
    throw std::invalid_argument("Date::Date: year should not equal to 0.");
  if (month < 1 || 12 < month)
    throw std::invalid_argument("Date::Date: month not in 1~12.");
  if (day < 1 || dayInMonth(year, month) < day)
    throw std::invalid_argument("Date::Date: day not in range.");
  if (year < 0) ++year;
  const int a = (14 - month) / 12;
  const int y = year + 4800 - a;
  const int m = month + 12 * a - 3;
  const int d = day;
  jdn = d + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
}

int Date::julian() const {
  return this->jdn;
}

int Date::year() const {
  const int J = this->jdn;
  const int f = J + j + (4 * J + B) / 146097 * 3 / 4 + C;
  const int e = r * f + v;
  const int M = this->month();
  const int Y = e / p - y + (n + m - M) / n;
  return Y > 0 ? Y : Y - 1;
}

Date::Month Date::month() const {
  const int J = this->jdn;
  const int f = J + j + (4 * J + B) / 146097 * 3 / 4 + C;
  const int e = r * f + v, g = e % p / r, h = u * g + w;
  return static_cast<Month>((h / s + m) % n + 1);
}

int Date::day() const {
  const int J = this->jdn;
  const int f = J + j + (4 * J + B) / 146097 * 3 / 4 + C;
  const int e = r * f + v, g = e % p / r, h = u * g + w;
  return h % s / u + 1;
}

Date::Weekday Date::weekday() const {
  return static_cast<Weekday>((this->jdn + 1) % 7);
}

Date &operator++(Date &self) {
  ++self.jdn; return self;
}

Date operator++(Date &self, int) {
  Date result = self; ++self; return result;
}

Date &operator--(Date &self) {
  --self.jdn; return self;
}

Date operator--(Date &self, int) {
  Date result = self; --self; return result;
}

Date &operator+=(Date &lhs, int rhs) {
  lhs.jdn += rhs; return lhs;
}

Date operator+(const Date &lhs, int rhs) {
  Date result = lhs; result += rhs; return result;
}

Date operator+(int &lhs, const Date &rhs) {
  Date result = rhs; result += lhs; return result;
}

Date operator-(Date &lhs, int rhs) {
  Date result = lhs; result += rhs; return result;
}

int operator-(const Date &lhs, const Date &rhs) {
  return lhs.julian() - rhs.julian();
}

bool operator==(const Date &lhs, const Date &rhs) {
  return lhs.julian() == rhs.julian();
}

bool operator!=(const Date &lhs, const Date &rhs) {
  return !(lhs == rhs);
}

bool operator<(const Date &lhs, const Date &rhs) {
  return lhs.julian() < rhs.julian();
}

bool operator>=(const Date &lhs, const Date &rhs) {
  return !(lhs < rhs);
}

bool operator>(const Date &lhs, const Date &rhs) {
  return rhs < lhs;
}

bool operator<=(const Date &lhs, const Date &rhs) {
  return !(rhs < lhs);
}

bool Date::isLeapYear(int year) {
  if (year == 0)
    throw std::invalid_argument("Date::isLeapYear: year should not be 0.");
  if (year < 0) ++year;
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int Date::dayInMonth(int year, Date::Month month) {
  return monthLen_[month] + (isLeapYear(year) && month == February);
}
