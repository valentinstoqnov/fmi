//
// Created by valio_stoyanov on 5/31/18.
//

#ifndef POLYNOMIAL_POLYNOMIAL_H
#define POLYNOMIAL_POLYNOMIAL_H

#include <limits>
#include "ds/array.h"

/*
  TODO: Extract some of the operators code in properly named functions.
  TODO: Implement polynom simplification and call it.
  TODO: Think of algorithm for polynomial division.
  TODO: Add the rest features and check functionality.
*/

template <typename T>
class Polynomial {
  static const T kZero = T(0);
  static const T kOne = T(1);
  Array<T> coefficients_;

  class Monomial {
    T coefficient_;
    unsigned degree_{};

    T Pow(const T& val) const {
      if (degree_ == 0 || val == kOne) return kOne;
      if (degree_ == 1) return val;

      T result = val;
      for (int i = 1; i < degree_; ++i) result *= val;

      return result;
    }

   public:
    Monomial(const T& coeffiecient, const unsigned degree)
        : coefficient_(coeffiecient), degree_(degree) {}

    Monomial()
        : coefficient_(kZero), degree_(0) {}

    void Set(const T& coefficient, const unsigned degree) {
      SetCoefficient(coefficient);
      SetDegree(degree);
    }

    void SetCoefficient(const T& coefficient) {
      coefficient_ = coefficient;
    }

    void SetDegree(const unsigned degree) {
      degree_ = degree;
    }

    const T& Coefficient() const {
      return coefficient_;
    }

    unsigned Degree() const {
      return degree_;
    }

    T operator()(const T& val) const {
      return coefficient_ * (Pow(val));
    }

    Monomial& Derivative() {
      if (degree_ == 0) coefficient_ = 0;
      else if (degree_ == 1) degree_ = 0;
      else coefficient_ *= degree_--;

      return *this;
    }

    Monomial& Antiderivative() {
      coefficient_ /= ++degree_;
      return *this;
    }

    Monomial Derivative() const {
      Monomial monomial(coefficient_, degree_);
      return monomial.Derivative();
    }

    Monomial Antiderivative() const {
      Monomial monomial(coefficient_, degree_);
      return monomial.Antiderivative();
    }

    T operator()(const T& left_bound, const T& right_bound) const {
      const Monomial antiderivative = Antiderivative();
      return antiderivative(right_bound) - antiderivative(left_bound);
    }

    friend std::ostream& operator<<(std::ostream& out, const Monomial& monomial) {
      if (monomial.coefficient_ == kZero)
        return out << kZero;

      if (monomial.coefficient_ < kZero)
        out << '(' << monomial.coefficient_ << ')';
      else
        out << monomial.coefficient_;

      if (monomial.degree_ > 0)
        if (monomial.degree_ == 1) out << 'x';
        else out << "x^" << monomial.degree_;

      return out;
    }

    Monomial& operator/=(const Monomial& other) {
      coefficient_ /= other.coefficient_;
      degree_ -= other.degree_;
    }
  };

  explicit Polynomial(const Monomial& monomial)
      : coefficients_(Array<T>(monomial.Degree() + 1)){
    coefficients_.Fill(kZero);
    coefficients_[monomial.Degree()] = monomial.Coefficient();
    Simplify();
  }

  unsigned GetTrailingZerosCount() {
    unsigned trailing_zeros = 0;
    for (long i = coefficients_.Size() - 1; i >= 0 && coefficients_[i] == kZero; --i)
      ++trailing_zeros;

    return trailing_zeros;
  }

  void Simplify() {
    const unsigned trailing_zeros = GetTrailingZerosCount();
    for (unsigned i = 0; i < trailing_zeros; ++i)
      coefficients_.PopBack();
  }
  /*
  void Divide(Polynomial denominator, const bool is_remainder = false) {
    if (denominator.coefficients_.Empty())
      throw std::invalid_argument("Polynomial: Division by zero polynomial.");
    if (coefficients_.Size() < denominator.coefficients_.Size())
      throw std::invalid_argument("Polynomial: Division by polynomial from higher degree.");

    Monomial num_monomial(coefficients_.PopBack(),
                      static_cast<const unsigned>(coefficients_.Size() - 1));
    Monomial denom_monomial(denominator.coefficients_.PopBack(),
                   static_cast<const unsigned>(denominator.coefficients_.Size() - 1));
    num_monomial /= denom_monomial;
    Polynomial remainder(num_monomial);

    while (!denominator.coefficients_.Empty()) {

    }
  }*/
 public:
  explicit Polynomial(const Array<T>& coefficients)
      : coefficients_(coefficients) { Simplify(); }

  friend std::ostream& operator<<(std::ostream& out, const Polynomial& polynomial){
    const Array<T>& coefficients = polynomial.coefficients_;
    if (coefficients.Empty())
      return out << kZero;

    Monomial monomial;
    for (unsigned i = 0; i < coefficients.Size() - 1; ++i) {
      monomial.Set(coefficients[i], i);
      out << monomial << " + ";
    }
    monomial.Set(coefficients.Back(),
                 static_cast<const unsigned int>(coefficients.Size() - 1));
    out << monomial;

    return out;
  }

  Polynomial& operator=(const Polynomial& other) {
    if (this != &other) coefficients_ = other.coefficients_;
    return *this;
  }

  bool operator==(const Polynomial& other) {
    return coefficients_ == other.coefficients_;
  }

  bool operator!=(const Polynomial& other) {
    return !(*this == other);
  }

  bool operator<(const Polynomial& other) {
    return coefficients_.Size() < other.coefficients_.Size();
  }

  bool operator>(const Polynomial& other) {
    return coefficients_.Size() > other.coefficients_.Size();
  }

  bool operator<=(const Polynomial& other) {
    return !(*this > other);
  }

  bool operator>=(const Polynomial& other) {
    return !(*this < other);
  }

  Polynomial& operator-=(const Polynomial& other) {
    const size_t min_size = std::min(coefficients_.Size(), other.coefficients_.Size());
    for (size_t i = 0; i < min_size; ++i)
      coefficients_[i] -= other.coefficients_[i];

    if (coefficients_.Size() < other.coefficients_.Size())
      for (size_t i = min_size; i < other.coefficients_.Size(); ++i)
        coefficients_.PushBack(-other.coefficients_[i]);

    Simplify();

    return *this;
  }

  Polynomial& operator+=(const Polynomial& other) {
    const size_t min_size = std::min(coefficients_.Size(), other.coefficients_.Size());
    for (size_t i = 0; i < min_size; ++i)
      coefficients_[i] += other.coefficients_[i];

    if (coefficients_.Size() < other.coefficients_.Size())
      for (size_t i = min_size; i < other.coefficients_.Size(); ++i)
        coefficients_.PushBack(other.coefficients_[i]);

    Simplify();

    return *this;
  }

  Polynomial& operator*=(const Polynomial& other) {
    const size_t result_deg =
        coefficients_.Size() + other.coefficients_.Size() - 1;
    Array<T> result(result_deg);
    result.Fill(kZero);

    for (size_t i = 0; i < coefficients_.Size(); ++i)
      for (size_t j = 0; j < other.coefficients_.Size(); ++j)
        result[i + j] += coefficients_[i] * other.coefficients_[j];

    coefficients_ = result;
    Simplify();

    return *this;
  }

  Polynomial& operator/=(const Polynomial& other) {


  }

  Polynomial& operator%=(const Polynomial&) {
    //TODO
  }

  Polynomial& operator*=(const T& val) {
    for (size_t i = 0; i < coefficients_.Size(); ++i)
      coefficients_[i] *= val;

    Simplify();

    return *this;
  }

  Polynomial& operator/=(const T& val) {
    if (val == kZero)
      throw std::invalid_argument("Polynomial: Division by zero.");

    for (size_t i = 0; i < coefficients_.Size(); ++i)
      coefficients_[i] /= val;

    Simplify();

    return *this;
  }

  T operator[](const unsigned degree) const {
    return (coefficients_.Empty() || degree >= coefficients_.Size()) ?
           kZero : coefficients_[degree];
  }

  T operator()(const T& val) {
    if (coefficients_.Empty()) return kZero;

    T result = kZero;
    Monomial monomial;
    for (unsigned i = 0; i < coefficients_.Size(); ++i) {
      monomial.Set(coefficients_[i], i);
      result += monomial(val);
    }

    return result;
  }

  T operator()(const T& left_bound, const T& right_bound) {
    if (coefficients_.Empty()) return kZero;

    T result = kZero;
    Monomial monomial;
    for (unsigned i = 0; i < coefficients_.Size(); ++i) {
      monomial.Set(coefficients_[i], i);
      result += monomial(left_bound, right_bound);
    }

    return result;
  }

  Polynomial& operator++() {
    if (coefficients_.Empty()) return *this;

    Array<T> result(coefficients_.Size() + 1);
    Monomial monomial;
    result.PushBack(kZero);
    for (unsigned i = 0; i < coefficients_.Size(); ++i) {
      monomial.Set(coefficients_[i], i);
      monomial.Antiderivative();
      result.PushBack(monomial.Coefficient());
    }
    coefficients_ = result;
    Simplify();

    return *this;
  }

  Polynomial operator++(int) {
    Polynomial result(*this);
    ++(*this);
    return result;
  }

  Polynomial& operator--() {
    if (coefficients_.Empty()) return *this;

    Array<T> result(coefficients_.Size() - 1);
    Monomial monomial;
    for (unsigned i = 1; i < coefficients_.Size(); ++i) {
      monomial.Set(coefficients_[i], i);
      monomial.Derivative();
      result.PushBack(monomial.Coefficient());
    }
    coefficients_ = result;
    Simplify();

    return *this;
  }

  Polynomial operator--(int) {
    Polynomial result(*this);
    --(*this);
    return result;
  }

  explicit operator int() const {
    return coefficients_.Empty() ? std::numeric_limits<int>::max()
                                 : static_cast<int>(coefficients_.Size() - 1);
  }

  explicit operator bool() const {
    return coefficients_.Empty();
  }

  bool operator!() const {
    return !coefficients_.Empty();
  }
};

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> result = lhs;
  result += rhs;
  return result;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> result = lhs;
  result -= rhs;
  return result;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> result = lhs;
  result *= rhs;
  return result;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> result = lhs;
  result /= rhs;
  return result;
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> result = lhs;
  result %= rhs;
  return result;
}

#endif //POLYNOMIAL_POLYNOMIAL_H
