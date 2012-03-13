#include <stack>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include "integer.h"

template <int Base>
class DigitStream {
private:
  std::deque<int8_t> digits;
  DigitStream<Base> *next;
  integer<Base> value;
  void fill_buffer() {
    value.writeDigits(std::back_inserter(digits));
  }
  void refill_buffer() {
    if (!next) {
      next = new DigitStream<Base>(value);
    }
    bool dummy;
    value.subtract(next -> pop(), dummy);
    fill_buffer();
  }
public:
  DigitStream(integer<Base> start) : value(start), next(NULL) {
    assert(!value.isZero());
    fill_buffer();
  }
  DigitStream(int64_t start) : value(start), next(NULL) {
    assert(!value.isZero());
    fill_buffer();
  }
  ~DigitStream() { delete next; }
  int8_t pop() {
    if (digits.empty()) {
      refill_buffer();
    }
    int8_t ret=digits.back();
    digits.pop_back();
    return ret;
  }
};

template <int Base>
void innerLoopFor(int64_t value) {
  DigitStream<Base> str(value);
  uint64_t cnt = 0;
  while(value && cnt <= 1e10) {
    value -= str.pop();
    ++cnt;
  }
  if (value) {
     std::cout << " >10000000000" << std::flush;
  } else {
    std::cout << std::setw(13) << cnt << std::flush;
  }
}

void runFor(int64_t start) {
  std::cout << std::setw(4) << start << ": ";
  innerLoopFor<2>(start);
  innerLoopFor<3>(start);
  innerLoopFor<4>(start);
  innerLoopFor<5>(start);
  innerLoopFor<6>(start);
  innerLoopFor<7>(start);
  innerLoopFor<8>(start);
  innerLoopFor<9>(start);
  innerLoopFor<10>(start);
  innerLoopFor<11>(start);
  innerLoopFor<12>(start);
  innerLoopFor<13>(start);
  innerLoopFor<14>(start);
  innerLoopFor<15>(start);
  innerLoopFor<16>(start);
  for (int base = 2; base < 17; ++base) {
    int64_t value = start;
  }
  std::cout << std::endl;
}

int main_table(int argc, char const *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " from to" << std::endl;
    exit(1);
  }

  std::cout << "      ";
  for (int base = 2; base < 17; ++base) {
    std::cout
      << (base >= 10 ? "      " : "       ")
      << "base " << base;
  }
  std::cout << std::endl;
  int64_t from = atoll(argv[1]); // better: boost::lexical_cast
  int64_t to = atoll(argv[2]); // better: boost::lexical_cast
  for (int64_t i = from; i <= to; ++i) {
//    std::cout << "-----------------------" << std::endl << i << std::endl;
    runFor(i);
  }
  return 0;
}

int main_one_number(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " start" << std::endl;
    exit(1);
  }

  int64_t start = atoll(argv[1]);
  const int base = 10;

  integer<base> value(start);
  DigitStream<base> str(start);
  uint64_t cnt = 0;
  while (!value.isZero()) {
    bool zero_crossing = false;
    int8_t val = str.pop();
    value.subtract(val, zero_crossing);
    ++cnt;
    if (zero_crossing) {
      std::cout << "zero crossing at step " << cnt << ", now: " << value << std::endl;
    }
  }
  std::cout << start << " in base " << base << " reaches 0 in step " << cnt << std::endl;

  return 0;
}

int main(int argc, char const *argv[]) {
  return main_one_number(argc, argv);
}
