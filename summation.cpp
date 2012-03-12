#include <stack>
#include <iostream>
#include <iomanip>
#include <assert.h>


class DigitStream {
private:
  std::stack<int8_t> digits;
  const uint64_t base;
  DigitStream *next;
  int64_t value;
  void fill_buffer() {
    int sign = (value < 0) ? -1 : 1;
    uint64_t absVal = sign*value;
    while(absVal) {
      digits.push(sign*(absVal % base));
      absVal /= base;
    }
  }
  void refill_buffer() {
    if (!next) {
      next = new DigitStream(value, base);
    }
    value -= next -> pop();
    fill_buffer();
  }
public:
  DigitStream(int64_t start, uint64_t base_) : value(start), base(base_), next(NULL) {
    assert(value);
    fill_buffer();
  }
  ~DigitStream() { delete next; }
  int8_t pop() {
    if (digits.empty()) {
      refill_buffer();
    }
    int8_t ret=digits.top();
    digits.pop();
    return ret;
  }
};

void runFor(int64_t start) {
  std::cout << std::setw(4) << start << ": ";
  for (int base = 2; base < 17; ++base) {
    int64_t value = start;
    DigitStream str(value, base);
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
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " start base maxiter" << std::endl;
    exit(1);
  }

  int64_t start = atoll(argv[1]);
  unsigned int base = atoi(argv[2]);
  int64_t maxiter = atoll(argv[3]);

  int64_t value = start;
  DigitStream str(value, base);
  uint64_t cnt = 0;
  std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
  while(value && cnt <= maxiter) {
    int64_t oldval = value;
    value -= str.pop();
    ++cnt;
    if ((value < 0) != (oldval < 0)) {
      if (-10 < value && value < 10) {
        // zero crossing
        std::cout << "zero crossing at step " << cnt << ", now: " << value << std::endl;
      } else {
        std::cerr << "overflow at step " << cnt << std::endl;
        exit(1);
      }
    }
  }
  if (value) {
    std::cout << start << " in base " << base << " needs more than " << maxiter << " iterations" << std::endl;
  } else {
    std::cout << start << " in base " << base << " reaches 0 in step " << cnt << std::endl;
  }

  return 0;
}

int main(int argc, char const *argv[]) {
  return main_one_number(argc, argv);
}
