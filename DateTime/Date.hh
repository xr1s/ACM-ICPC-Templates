#include <stdexcept>

class Date {
 public:
  enum Weekday {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };
  enum Month {
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12
  };
  Date(int, Month = JANUARY, int = 1);
  Date(int, int = JANUARY, int = 1);
  friend Date &operator++(Date &);
  friend Date &operator--(Date &);
  friend int operator-(const Date &, const Date &);

  int year() const;
  Month month() const;
  int day() const;
  Weekday weekday() const;
  int dayOfYear() const;

  static bool isLeap(int);
  static int dayInMonth(int, Month);
 private:
  int year_;
  Month month_;
  int day_;
  const static int dayInMonth_[];
  // Invoker should guarantee that lhs <= rhs.
  static int yearDiff_(int, int);
};
const int Date::dayInMonth_[] = {
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
};

Date::Month &operator++(Date::Month &self) {
  switch (self) { 
   case Date::JANUARY:
    return self = Date::FEBRUARY;
   case Date::FEBRUARY:
    return self = Date::MARCH;
   case Date::MARCH:
    return self = Date::APRIL;
   case Date::APRIL:
    return self = Date::MAY;
   case Date::MAY:
    return self = Date::JUNE;
   case Date::JUNE:
    return self = Date::JULY;
   case Date::JULY:
    return self = Date::AUGUST;
   case Date::AUGUST:
    return self = Date::SEPTEMBER;
   case Date::SEPTEMBER:
    return self = Date::OCTOBER;
   case Date::OCTOBER:
    return self = Date::NOVEMBER;
   case Date::NOVEMBER:
    return self = Date::DECEMBER;
   case Date::DECEMBER:
    return self = Date::JANUARY;
  }
  return self;
}

Date::Month operator++(Date::Month &self, int) {
  Date::Month result = self; ++self; return result;
}

Date::Month &operator--(Date::Month &self) {
  switch (self) { 
   case Date::JANUARY:
    return self = Date::DECEMBER;
   case Date::FEBRUARY:
    return self = Date::JANUARY;
   case Date::MARCH:
    return self = Date::FEBRUARY;
   case Date::APRIL:
    return self = Date::MARCH;
   case Date::MAY:
    return self = Date::APRIL;
   case Date::JUNE:
    return self = Date::MAY;
   case Date::JULY:
    return self = Date::JUNE;
   case Date::AUGUST:
    return self = Date::JULY;
   case Date::SEPTEMBER:
    return self = Date::AUGUST;
   case Date::OCTOBER:
    return self = Date::SEPTEMBER;
   case Date::NOVEMBER:
    return self = Date::OCTOBER;
   case Date::DECEMBER:
    return self = Date::NOVEMBER;
  }
  return self;
}

Date::Month operator--(Date::Month &self, int) {
  Date::Month result = self; --self; return result;
}

Date::Weekday &operator++(Date::Weekday &self) {
  switch (self) {
   case Date::MONDAY:
    return self = Date::TUESDAY;
   case Date::TUESDAY:
    return self = Date::WEDNESDAY;
   case Date::WEDNESDAY:
    return self = Date::THURSDAY;
   case Date::THURSDAY:
    return self = Date::FRIDAY;
   case Date::FRIDAY:
    return self = Date::SATURDAY;
   case Date::SATURDAY:
    return self = Date::SUNDAY;
   case Date::SUNDAY:
    return self = Date::MONDAY;
  }
  return self;
}

Date::Weekday operator++(Date::Weekday &self, int) {
  Date::Weekday result = self; ++self; return result;
}

bool Date::isLeap(int y) {
  bool result = false;
  result |= y % 4 == 0 && y % 100 != 0;
  result |= y % 400 == 0 && y % 3200 != 0;
  return result;
}

int Date::dayInMonth(int y, Month m) {
  return dayInMonth_[m] + (isLeap(y) && m == FEBRUARY);
}

Date::Date(int y, Month m, int d)
    : year_(y), month_(m), day_(d) {
  if (m < 1 || 12 < m)
    throw std::invalid_argument("Date::Date(int, Date::Month, int): month");
  if (d < 1 || dayInMonth(y, static_cast<Date::Month>(m)) < d)
    throw std::invalid_argument("Date::Date(int, Date::Month, int): day");
}

Date::Date(int y, int m, int d)
    : year_(y), month_(static_cast<Date::Month>(m)), day_(d) {
  if (m < 1 || 12 < m)
    throw std::invalid_argument("Date::Date(int, int, int) month");
  if (d < 1 || dayInMonth(y, static_cast<Date::Month>(m)) < d)
    throw std::invalid_argument("Date::Date(int, int, int) day");
}

bool operator==(const Date &lhs, const Date &rhs) {
  return lhs.year() == rhs.year()
      && lhs.month() == rhs.month()
      && lhs.day() == rhs.day();
}

bool operator!=(const Date &lhs, const Date &rhs) {
  return !(lhs == rhs);
}

bool operator<(const Date &lhs, const Date &rhs) {
  if (lhs.year() != rhs.year()) return lhs.year() < rhs.year();
  if (lhs.month() != rhs.month()) return lhs.month() < rhs.month();
  return lhs.day() < rhs.day();
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

Date &operator++(Date &self) {
  const int dim = Date::dayInMonth(self.year_, self.month_);
  if (self.day_++ == dim) {
    if (self.month_++ == Date::DECEMBER) ++self.year_;
    self.day_ = 1;
  }
  return self;
}

Date operator++(Date &self, int) {
  Date result = self; ++self; return result;
}

Date &operator--(Date &self) {
  if (self.day_-- == 1) {
    if (self.month_-- == Date::JANUARY) --self.year_;
    self.day_ = Date::dayInMonth(self.year_, self.month_);
  }
  return self;
}

Date operator--(Date &self, int) {
  Date result = self; --self; return result;
}

int operator-(const Date &lhs, const Date &rhs) {
  bool negative = lhs < rhs;
  Date l = lhs, r = rhs;
  if (negative) std::swap(l, r);
  int result = Date::yearDiff_(r.year_, l.year_);
  result = result + l.dayOfYear() - r.dayOfYear();
  return negative ? -result : result;
}

int Date::year() const {
  return this->year_;
}

Date::Month Date::month() const {
  return this->month_;
}

int Date::day() const {
  return this->day_;
}

Date::Weekday Date::weekday() const {
  int m = static_cast<int>(this->month_);
  int year = this->year_;
  if (m <= 2) m += 12, --year;
  const int c = year / 100;
  const int y = year % 100;
  const int d = this->day_;
  int w = (y + y / 4 + c / 4 - c * 2 + (m + 1) * 26 / 10 + d - 1) % 7;
  return static_cast<Date::Weekday>(w < 0 ? w + 7 : w);
}

int Date::dayOfYear() const {
  int result = 0;
  for (int i = 1; i != this->month_; ++i)
    result += Date::dayInMonth(this->year_, static_cast<Month>(i));
  return result + this->day_ - 1;
}

int Date::yearDiff_(int lhs, int rhs) {
  int result = (rhs - lhs) * 365;
  lhs = (lhs + 3) / 4;
  rhs = (rhs + 3) / 4;
  result += rhs - lhs;
  lhs = (lhs + 24) / 25;
  rhs = (rhs + 24) / 25;
  result -= rhs - lhs;
  lhs = (lhs + 3) / 4;
  rhs = (rhs + 3) / 4;
  result += rhs - lhs;
  lhs = (lhs + 7) / 8;
  rhs = (rhs + 7) / 8;
  result -= rhs - lhs;
  return result;
}
