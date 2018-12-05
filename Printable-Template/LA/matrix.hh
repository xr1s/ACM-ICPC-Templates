template <typename T> struct Matrix {  // copy,move are omitted
  Matrix(const Matrix &that): row_{that.row_}, col_{that.col_} {
    this->value_ = new T[this->row_ * this->col_];
    copy(that.value_, that.value_ + that.row_ * that.col_, this->value_);
  }
  Matrix &operator=(const Matrix &that) {
    if (this == &that) return *this; else delete[] this->value_;
    this->row_ = that.row_; this->col_ = that.col_;
    this->value_ = new T[this->row_ * this->col_];
    copy(that.value_, that.value_ + that.row_ * that.col_, this->value_);
  }
  Matrix(size_t row, size_t col)
      : row_{row}, col_{col}, value_{new T[row * col]} {
    std::fill(this->value_, this->value_ + row * col, 0);
  }
  template <typename Head, typename ...Tail, size_t Col>
  Matrix(const Head (&head)[Col], const Tail (&...tail)[Col])
      : row_{1 + sizeof...(tail)}, col_{Col} {
    const Head *matrix[] = {head, tail...};
    this->value_ = new T[this->row_ * this->col_];
    for (size_t r = 0; r != this->row_; ++r)
      copy(matrix[r], matrix[r] + Col, this->value_ + r * Col);
  }
  ~Matrix() { delete[] this->value_; this->value_ = nullptr; }
  const T *operator[](size_t row) const {
    return this->value_ + this->col_ * row;
  }
  T *operator[](size_t row) { return this->value_ + this->col_ * row; }
  size_t row() const { return this->row_; }
  size_t col() const { return this->col_; }
  friend Matrix &operator+=(Matrix &lhs, const Matrix &rhs) {
    if (lhs.row_ != rhs.row_ || lhs.col_ != rhs.col_) throw "Matrix::operator+=";
    transform(lhs.value_, lhs.value_ + lhs.row_ * lhs.col_,
              rhs.value_, lhs.value_, plus<>{});  // operator-= -> minus<>{}
    return lhs;
  }
  friend Matrix &operator*=(Matrix &lhs, const Matrix &rhs) {
    if (lhs.col_ != rhs.row_) throw "Matrix::operator*=";
    size_t row = lhs.row_, col = rhs.col_;
    T *value = new T[row * col]{};
    for (size_t i = 0; i != lhs.row_; ++i)
      for (size_t j = 0; j != lhs.col_; ++j)
        for (size_t k = 0; k != rhs.col_; ++k)
          value[i * col + k] += lhs[i][j] * rhs[j][k];
    delete[] lhs.value_; lhs.row_ = row; lhs.col_ = col; lhs.value_ = value;
    return lhs;
  }
  friend bool operator==(const Matrix &lhs, const Matrix &rhs) {
    if (lhs.row_ != rhs.row_ || lhs.col_ != rhs.col_) return false;
    return equal(lhs.value_, lhs.value_ + lhs.row_ * lhs.col_, rhs.value_);
  }
  void triangularize() { this->triangularizeInternal_(); }
  T determinant() const {
    if (this->row_ != this->col_) throw "Matrix::determinant";
    Matrix matrix = *this;
    T result = matrix.triangularizeInternal_() ? -1 : 1;
    for (size_t k = 0; k != matrix.row_; ++k)
      result = result * matrix.value_[k * matrix.col_ + k];
    return result;
  }
  void eliminate() {
    const size_t row = this->row_, col = this->col_;
    this->triangularizeInternal_();
    for (size_t r = row - 1, c; ~r; --r) {
      for (c = 0; c != col && isZero(this->value_[r * col + c]); ++c);
      if (c == col) continue;
      for (size_t i = c + 1; i != col; ++i)
        this->value_[r * col + i] /= this->value_[r * col + c];
      this->value_[r * col + c] = 1;
      for (size_t i = 0; i != r; ++i) {
        for (size_t j = c + 1; j != col; ++j)
          this->value_[i * col + j] -= \
              this->value_[r * col + j] * this->value_[i * col + c];
        this->value_[i * col + c] = 0;
      }
    }
  }
  void invert() {
    if (this->row_ != this->col_) throw "Matrix::invert";
    Matrix apd{this->row_, this->col_ * 2};
    const size_t row = this->row_, col = this->col_;
    for (size_t r = 0; r != row; ++r) {
      move(this->value_ + r * col, this->value_ + (r + 1) * col, app[r]);
      apd[r][col + r] = 1;
    }
    apd.eliminate();
    for (size_t r = 0; r != row; ++r)
      move(apd[r] + col, apd[r + 1], this->value_ + r * col);
  }
 private:
  size_t row_, col_; T *value_;
  bool triangularizeInternal_() {
    bool swapped = false;
    T *matrix[this->row_], value = new T[row * col];
    const size_t row = this->row_, col = this->col_;
    for (size_t r = 0; r != row; ++r) matrix[r] = this->value_ + r * col;
    for (size_t r = 0, c = 0; r != row && c != col; ++c) {
      size_t pivot = ~static_cast<size_t>(0);
      for (size_t k = r; !~pivot && k != row; ++k)
        if (!isZero(matrix[k][c])) pivot = k;
      if (pivot == ~static_cast<size_t>(0)) continue;
      if (pivot != r++)
        swap(matrix[pivot], matrix[r]); swapped = !swapped;
      for (size_t i = r; i != row; ++i) {
        T f = matrix[i][c] / matrix[r - 1][c];
        for (size_t j = c + 1; j != col; ++j)
          matrix[i][j] -= matrix[r - 1][j] * f;
        matrix[i][c] = 0;
      }
    }
    for (size_t r = 0; r != row; ++r)
      move(matrix[r], matrix[r] + col, value + r * col);
    delete[] this->value_; this->value_ = value;
    return swapped;
  }
};
