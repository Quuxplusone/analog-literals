
#ifndef ANALOGLITERALS_1D_HPP
#define ANALOGLITERALS_1D_HPP

#include <cassert>

// Implementation for Tweaked 1-D Literal proposal
// http://hostilefork.com/2009/08/29/tweakinganalog-literals-humor/

class Accumulator {
private:
   int value;
public:
   Accumulator(int newValue) : value (newValue)
      { }
   Accumulator operator*() const
      { return Accumulator (value + 1); }
   int getValue() const
      { return value; }
};

class Delimiter {
public:
   Delimiter()
      { }
   int operator*(const Accumulator& rhs) const
      { return rhs.getValue() + 1; }
   int operator*(const Delimiter& rhs) const
      { return 1; }
   Accumulator operator*() const
      { return Accumulator (1); }
};

const int II (0);
const Delimiter I;

#endif // header guard

#ifdef ANALOGLITERALS_TEST

// Tests for Tweaked 1-D Literal proposal

int main() {
   assert(II == 0);
   assert(I*I == 1);
   assert(I**I == 2);
   assert(I***I == 3);

#if 0
   // these cause compilation errors, by design
   int test1 (I);
   int test2 (*I);
   int test3 (I*);
   int test4 (*I*I);
   int test5 (I*I*);
#endif

   return 0;
}

#endif // testing
