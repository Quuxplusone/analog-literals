
#ifndef ANALOGLITERALS_HPP
#define ANALOGLITERALS_HPP

namespace analog_literals {

typedef unsigned int uint;

// Result types

  struct gen { template <typename T> operator T () const { return T(); } };

  template <int x> struct line { static const int length; };
  template <int x, int y> struct rectangle { static const int width, height, area; };
  template <int x, int y, int z> struct cuboid { static const int width, height, depth, volume; };

// Static members

  template <int len> int const line<len>::length = len;

  template <int x, int y> int const rectangle<x, y>::width = x;
  template <int x, int y> int const rectangle<x, y>::height = y;
  template <int x, int y> int const rectangle<x, y>::area = x * y;

  template <int x, int y, int z> int const cuboid<x, y, z>::width = x;
  template <int x, int y, int z> int const cuboid<x, y, z>::height = y;
  template <int x, int y, int z> int const cuboid<x, y, z>::depth = z;
  template <int x, int y, int z> int const cuboid<x, y, z>::volume = x * y * z;

  template <int x, int y, int z> rectangle<x, y> front (cuboid<x, y, z>) { return gen(); }
  template <int x, int y, int z> rectangle<z, y> side (cuboid<x, y, z>) { return gen(); }
  template <int x, int y, int z> rectangle<x, z> top (cuboid<x, y, z>) { return gen(); }

// Equality

  template <int ax, int bx> bool operator== (line<ax>, line<bx>) { return ax == bx; }

  template <int ax, int ay, int bx, int by> bool operator== (rectangle<ax, ay>, rectangle<bx, by>)
  { return ax == bx && ay == by; }

  template <int ax, int ay, int az, int bx, int by, int bz>
  bool operator== (cuboid<ax, ay, az>, cuboid<bx, by, bz>) { return ax == bx && ay == by && az == bz; }

// Symbols

  enum osym { o };
  enum Lsym { L };

// Convenience namespaces that can be "using namespace"'d:

  namespace symbols
  {
    using analog_literals::o;
    using analog_literals::L;
  }

  namespace shapes
  {
    using analog_literals::line;
    using analog_literals::rectangle;
    using analog_literals::cuboid;
  }

// Constructions (i.e., the messy part)

  template <int, typename> struct dashes {}; // T preceded by n hyphens
  dashes<1, osym> operator- (osym) { return gen(); }
  dashes<2, osym> operator-- (osym) { return gen(); }
  template <typename T, int n> dashes<n+1, T> operator- (dashes<n,T>) { return gen(); }
  template <typename T, int n> dashes<n+2, T> operator-- (dashes<n,T>) { return gen(); }

  // represents *----o
  line<0> operator* (osym) { return gen(); }
  template <int n> line<n> operator* (dashes<n, osym>) { return gen(); }

  template <int, typename> struct bangs {}; // T preceded by n bangs
  template <int n> bangs<1, line<n> > operator! (line<n>) { return gen(); }
  template <typename T, int n> bangs<n+1, T> operator! (bangs<n,T>) { return gen(); }

  // represents *!!!!*--o
  template <int x, int y> struct lower_rectangle {};
  template <int x> lower_rectangle<x,0> operator * (line<x>) { return gen(); }
  template <int b, int d> lower_rectangle<d, b/2> operator * (bangs<b, line<d> >)
  {
#ifdef USE_STATIC_ASSERT
    static_assert(b % 2 == 0, "");
#endif /* USE_STATIC_ASSERT */
    return gen();
  }

  template <int x, int y> dashes<1, lower_rectangle<x,y> > operator- (lower_rectangle<x,y>) { return gen(); }
  template <int x, int y> dashes<2, lower_rectangle<x,y> > operator-- (lower_rectangle<x,y>) { return gen(); }

  // represents *--*!!!!*--o
  template <int y> rectangle<0,y> operator * (lower_rectangle<0,y>) { return gen(); }
  template <int x, int y> rectangle<x,y> operator * (dashes<x, lower_rectangle<x,y> >) { return gen(); }
} // analog_literals

#endif // header guard

#ifdef ANALOGLITERALS_TEST

int main ()
{
  using namespace analog_literals::symbols;
  using namespace analog_literals::shapes;

  line<7>(*-------o);

  rectangle<5, 3>(*-----*
                  !     !
                  !     !
                  !     !
                  *-----o);
}

#endif // testing
