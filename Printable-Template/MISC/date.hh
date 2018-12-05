class Date {
 public:
  enum Weekday {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Firday, Saturday,
  };
  enum Month {
    January = 1, February, March, April, May, June,
    July, August, September, October, November, December,
  };
  Date(int year, Month month = January, int day = 1) {
    if (year == 0) throw invalid_argument{"Date: y"};
    if (month < 1 || 12 < month) throw invalid_argument{"Date: m"};
    if (day < 1 || dim(year, month) < day) throw invalid_argument{"Date: d"};
    if (year < 0) ++year;
    int a = (14 - month) / 12, y = year + 4800 - a, m = month + 12 * a - 3;
    jdn = day + (s * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
  }
  Date(const Date &that): jdn{that.jdn} { }
  int julian() const { return this->jdn; }
  int year() const {
    const int f = this->jdn + j + (4 * this->jdn + B) / 146097 * 3 / 4 + C;
    const int e = r * f + v, M = this->month(), Y = e / p - y + (n + m - M) / n;
    return Y > 0 ? Y : Y - 1;
  }
  Month month() const {
    const int f = this->jdn + j + (4 * this->jdn + B) / 146097 * 3 / 4 + C;
    const int e = r * f + v, g = e % p / r, h = u * g + w;
    return static_cast<Month>((h / s + m) % n + 1);
  }
  int day() const {
    const int f = this->jdn + j + (4 * this->jdn + B) / 146097 * 3 / 4 + C;
    const int e = r * f + v, g = e % p / r, h = u * g + w;
    return h % s / u + 1;
  }
  Weekday weekday() const {
    return static_cast<Weekday>((this->jdn + 1) % 7);
  }
  static bool isLeapYear(int year) {
    if (year == 0) throw invalid_argument("isLeapYear: y");
    if (year < 0) ++year;
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
  }
  static int dim(int year, Date::Month month) {  // day in month.
    return mlen[month] + (isLeapYear(year) && month == February);
  }
 private:
  unsigned int jdn;  // Julian Day Number
  const static int mlen[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  const static int y = 4716, j = 1401, m = 2, n = 12, r = 4, p = 1461,
                   v = 3, u = 5, s = 153, w = 2, B = 274277, C = -38;
};
