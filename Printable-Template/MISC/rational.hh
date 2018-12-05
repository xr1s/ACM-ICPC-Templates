template <typename T> struct Rational {
  Rational(): numerator_{0}, denominator_{1} { }
  Rational(const T &numerator, const T &denominator = 1) {
    if (denominator == 0) throw invalid_argument("Rational::Rational");
    if (denominator < 0) {
      numerator_ = -numerator_;
      denominator_ = -denominator_;
    }
    T divisor = gcd(numerator, denominator);
    this->numerator_ = numerator / divisor;
    this->denominator_ = denominator / divisor;
  }
  template <typename Float> Rational(Float f) {
    int exponent = 0;
    for (; floor(f) != f; ++exponent) f *= 2;
    this->denominator_ = static_cast<T>(1) << exponent;
    this->numerator_ = static_cast<T>(floor(f));
  }
  T numerator() const { return this->numerator_; }
  T denominator() const { return this->denominator_; }
  friend Rational operator-(const Rational &self) {
    return {-self.numerator_, self.denominator_};
  }
  friend Rational &operator+=(Rational &lhs, const Rational &rhs) {
    T divisor = gcd(lhs.denominator_, rhs.denominator_);
    lhs.denominator_ /= divisor;
    lhs.numerator_ *= rhs.denominator_ / divisor;
    lhs.numerator_ += lhs.denominator_ * rhs.numerator_;
    lhs.denominator_ *= rhs.denominator_;
    divisor = gcd(lhs.numerator_, lhs.denominator_);
    lhs.numerator_ /= divisor;
    lhs.denominator_ /= divisor;
  }
  friend Rational &operator-=(Rational &lhs, const Rational &rhs) {
    T divisor = gcd(lhs.denominator_, rhs.denominator_);
    lhs.denominator_ /= divisor;
    lhs.numerator_ *= rhs.denominator_ / divisor;
    lhs.numerator_ -= lhs.denominator_ * rhs.numerator_;
    lhs.denominator_ *= rhs.denominator_;
    divisor = gcd(lhs.numerator_, lhs.denominator_);
    lhs.numerator_ /= divisor;
    lhs.denominator_ /= divisor;
  }
  friend Rational &operator*=(Rational &lhs, const Rational &rhs) {
    T divisor = 1;
    divisor *= gcd(lhs.numerator_, rhs.denominator_);
    divisor *= gcd(lhs.denominator_, rhs.numerator_);
    lhs.numerator_ *= rhs.numerator_;
    lhs.numerator_ /= divisor;
    lhs.denominator_ *= rhs.denominator_;
    lhs.denominator_ /= divisor;
    return lhs;
  }
  friend Rational &operator/=(Rational &lhs, const Rational &rhs) {
    T divisor = 1;
    divisor *= gcd(lhs.numerator_, rhs.numerator_);
    divisor *= gcd(lhs.denominator_, rhs.denominator_);
    lhs.numerator_ *= rhs.denominator_;
    lhs.numerator_ /= divisor;
    lhs.denominator_ *= rhs.numerator_;
    lhs.denominator_ /= divisor;
    if (lhs.denominator_ < 0) {
      lhs.numerator_ = -lhs.numerator_;
      lhs.denominator_ = -lhs.denominator_;
    }
    return lhs;
  }
  friend bool operator==(const Rational &lhs, const Rational &rhs) {
    return lhs.denominator_ == rhs.denominator_
        && lhs.numerator_ == rhs.numerator_;
  }
  friend bool operator<(const Rational &lhs, const Rational &rhs) {
    return lhs.numerator_ * rhs.denominator_ < lhs.denominator_ * rhs.numerator_;
  }
  void limit(T max_denominator) {
    if (max_denominator < 1) throw invalid_argument{"Rational::limit"};
    if (this->denominator_ < max_denominator) return;
    T p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    T n = this->numerator_, d = this->denominator_;
    while (true) {
      const T a = n / d, q2 = q0 + a * q1;
      if (q2 > max_denominator) break;
      p0 = exchange(p1, p0 + a * p1);
      q0 = exchange(q1, q2);
      n = exchange(d, n - a * d);
    }
    this->numerator_ = p1, this->denominator_ = q1;
  }
 private:
  T numerator_, denominator_;
};
