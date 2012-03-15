#include <stack>
#include <iostream>
#include <iomanip>
#include <assert.h>


template<unsigned int Base>
class DigitStream {
private:
  std::stack<int8_t> digits;
  DigitStream *next;
  int64_t value;
  void fill_buffer() {
    if (value < 0) {
      uint64_t absVal = -value;
      while(absVal) {
        digits.push(-(absVal % Base));
        absVal /= Base;
      }
    } else {
      uint64_t absVal = value;
      while(absVal) {
        digits.push(absVal % Base);
        absVal /= Base;
      }
    }
  }
  void refill_buffer() {
    if (!next) {
      next = new DigitStream(value);
    }
    value -= next -> pop();
    fill_buffer();
  }
public:
  DigitStream(int64_t start) : value(start), next(NULL) {
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

template<unsigned int Base>
void runForInner(int64_t value) {
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
  runForInner<2>(start);
  runForInner<3>(start);
  runForInner<4>(start);
  runForInner<5>(start);
  runForInner<6>(start);
  runForInner<7>(start);
  runForInner<8>(start);
  runForInner<9>(start);
  runForInner<10>(start);
  runForInner<11>(start);
  runForInner<12>(start);
  runForInner<13>(start);
  runForInner<14>(start);
  runForInner<15>(start);
  runForInner<16>(start);
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
    runFor(i);
  }
  return 0;
}

template<unsigned int Base>
int one_number(int64_t start, uint64_t maxiter) {
  int64_t value = start;
  DigitStream<Base> str(value);
  uint64_t cnt = 0;
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
    std::cout << start << " in base " << Base << " needs more than " << maxiter << " iterations" << std::endl;
  } else {
    std::cout << start << " in base " << Base << " reaches 0 in step " << cnt << std::endl;
  }

  return 0;
}


int main_one_number(int argc, char const *argv[]) {
  if (argc != 4 && argc != 3) {
    std::cerr << "Usage: " << argv[0] << " start base [maxiter]" << std::endl;
    exit(1);
  }

  int64_t start = atoll(argv[1]);
  unsigned int base = atoi(argv[2]);
  uint64_t maxiter = std::numeric_limits<uint64_t>::max();
  if (argc > 3) {
    maxiter = atoll(argv[3]);
  }

  switch(base) {
  case 2: return one_number<2>(start, maxiter);
  case 3: return one_number<3>(start, maxiter);
  case 4: return one_number<4>(start, maxiter);
  case 5: return one_number<5>(start, maxiter);
  case 6: return one_number<6>(start, maxiter);
  case 7: return one_number<7>(start, maxiter);
  case 8: return one_number<8>(start, maxiter);
  case 9: return one_number<9>(start, maxiter);
  case 10: return one_number<10>(start, maxiter);
  case 11: return one_number<11>(start, maxiter);
  case 12: return one_number<12>(start, maxiter);
  case 13: return one_number<13>(start, maxiter);
  case 14: return one_number<14>(start, maxiter);
  case 15: return one_number<15>(start, maxiter);
  case 16: return one_number<16>(start, maxiter);
  default:
    std::cerr << "for performance reasons, base must be in the range 2..16" << std::endl;
    exit(1);
  }
}


int main(int argc, char const *argv[]) {
  return main_one_number(argc, argv);
  // return main_table(argc, argv);
}
