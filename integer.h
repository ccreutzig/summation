#include <vector>
#include <ostream>

template <int Base>
class integer {
public:
  integer(int64_t);
  integer(const integer&);
  void subtract(int8_t d, bool &zero_pass); // requires: abs(d) < Base
  bool isZero() const;
  template <typename OutIter>
  void writeDigits(OutIter it) const;
  std::ostream &write(std::ostream &out);
private:
  std::vector<int8_t> digits;
  bool neg;
};

template <int Base>
integer<Base>::integer(int64_t value) {
  neg = (value < 0);
  uint64_t absVal = neg ? -value : value;
  while(absVal) {
    digits.push_back(absVal % Base);
    absVal /= Base;
  }
}

template <int Base>
integer<Base>::integer(const integer<Base> & other) :
  neg(other.neg), digits(other.digits.begin(), other.digits.end())
{}

template <int Base>
void integer<Base>::subtract(int8_t d, bool &zero_pass) { // requires: abs(d) < Base
  zero_pass = false;
  if (!neg) {
    d = -d;
  }
  if (-d >= digits[0] && digits.size() == 1) {
    zero_pass = true;
    digits[0] = - d - digits[0];
    if (0 == digits[0]) {
      digits.clear();
    }
    neg = !neg;
    return;
  }
  for (size_t i = 0; d && i < digits.size(); ++i) {
    digits[i] += d;
    d = digits[i] / Base;
    if (digits[i] < 0) {
      --d;
      digits[i] += Base;
    } else {
      digits[i] %= Base;
    }
  }
  if (d) {
    digits.push_back(d);
  } else {
    while (!digits.empty() && 0 == digits.back()) {
      digits.pop_back();
    }
  }
}

template <int Base>
bool integer<Base>::isZero() const {
  return digits.empty();
}

template<int Base>
template<class OutIter>
void integer<Base>::writeDigits(OutIter it) const {
  int8_t sign = neg ? -1 : 1;
  for (std::vector<int8_t>::const_iterator i = digits.begin();
    i != digits.end(); ++i) {
    *it = sign*(*i);
    ++it;
  }
}

template <int Base>
std::ostream& integer<Base>::write(std::ostream &out) {
  if (neg) {
    out << '-';
  }
  for (std::vector<int8_t>::const_reverse_iterator i = digits.rbegin();
    i != digits.rend(); ++i) {
    out << static_cast<char>('0' + (*i));
  }

  return out;
}

template <int Base>
std::ostream &operator << (std::ostream &out, integer<Base> i) {
  return i.write(out);
}
