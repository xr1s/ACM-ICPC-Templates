#pragma once

#include "Util.hh"

template <typename T, size_t Row = 0, size_t Col = 0> class Matrix;

// Declare but do not define.
template <typename T, size_t Row> class Matrix<T, Row, 0>;
template <typename T, size_t Col> class Matrix<T, 0, Col>;

#include "DynamicMatrix.hh"
#include "StaticMatrix.hh"
#include "SquareMatrix.hh"
#include "Operations.hh"
