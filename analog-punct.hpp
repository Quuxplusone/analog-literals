
#ifndef ANALOGLITERALS_HPP
#define ANALOGLITERALS_HPP

namespace analog_literals {

  typedef unsigned int uint;
#ifdef USE_STATIC_ASSERT
  #define analog_literals_assert(x) static_assert(x, "")
#else
  #define analog_literals_assert(x)
#endif

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
  dashes<1, osym> operator~ (osym) { return gen(); }
  dashes<1, osym> operator- (osym) { return gen(); }
  dashes<2, osym> operator-- (osym) { return gen(); }
  template <typename T, int n> dashes<n+1, T> operator~ (dashes<n,T>) { return gen(); }
  template <typename T, int n> dashes<n+1, T> operator- (dashes<n,T>) { return gen(); }
  template <typename T, int n> dashes<n+2, T> operator-- (dashes<n,T>) { return gen(); }

  template <int> struct odashes {}; // o followed by n hyphens
  odashes<2> operator-- (osym, int) { return gen(); }
  template <int n> odashes<n+2> operator-- (odashes<n>, int) { return gen(); }

  // represents *----o
  line<0> operator* (osym) { return gen(); }
  line<1> operator- (osym, osym) { return gen(); }
  template <int n> line<n> operator* (dashes<n, osym>) { return gen(); }
  template <int n> line<n+1> operator- (osym, dashes<n, osym>) { return gen(); }
  template <int n> line<n+1> operator- (odashes<n>, osym) { return gen(); }
  template <int p, int q> line<p+q+1> operator- (odashes<p>, dashes<q, osym>) { return gen(); }

  template <int, typename> struct bangs {}; // T preceded by n bangs
  template <int n> bangs<1, line<n> > operator! (line<n>) { return gen(); }
  template <int n> bangs<1, odashes<n> > operator! (odashes<n>) { return gen(); }
  template <typename T, int n> bangs<n+1, T> operator! (bangs<n,T>) { return gen(); }

  template <int s2, int b> bangs<s2, line<b+1> > operator- (bangs<s2, odashes<b> >, osym) { return gen(); }
  template <int s2, int p, int q> bangs<s2, line<p+q+1> > operator- (bangs<s2, odashes<p> >, dashes<q, osym>) { return gen(); }

  // represents *!!!!*--o
  template <int s, int b> struct osidesobottomo {};
  template <int b> osidesobottomo<b,0> operator * (line<b>) { return gen(); }
  template <int s2, int b> osidesobottomo<s2/2, b> operator* (bangs<s2, line<b> >)
  { analog_literals_assert(s2 % 2 == 0); return gen(); }

  template <int s, int b> dashes<1, osidesobottomo<s,b> > operator~ (osidesobottomo<s,b>) { return gen(); }
  template <int s, int b> dashes<1, osidesobottomo<s,b> > operator- (osidesobottomo<s,b>) { return gen(); }
  template <int s, int b> dashes<2, osidesobottomo<s,b> > operator-- (osidesobottomo<s,b>) { return gen(); }

  // represents *!!!!o--
  template <int s, int b> struct osidesobottom {};
  template <int b> osidesobottom<0,b> operator * (odashes<b>) { return gen(); }
  template <int s2, int b> osidesobottom<s2/2, b> operator* (bangs<s2, odashes<b> >)
  { analog_literals_assert(s2 % 2 == 0); return gen(); }
  template <int s2, int b> osidesobottom<(s2+1)/2, b> operator| (osym, bangs<s2, odashes<b> >)
  { analog_literals_assert(s2 % 2 == 1); return gen(); }
  template <int t, int s2, int b> dashes<t, osidesobottom<(s2+1)/2, b> > operator| (dashes<t, osym>, bangs<s2, odashes<b> >)
  { analog_literals_assert(s2 % 2 == 1); return gen(); }

  template <int s, int b> dashes<1, osidesobottom<s,b> > operator~ (osidesobottom<s,b>) { return gen(); }
  template <int s, int b> dashes<1, osidesobottom<s,b> > operator- (osidesobottom<s,b>) { return gen(); }
  template <int s, int b> dashes<2, osidesobottom<s,b> > operator-- (osidesobottom<s,b>) { return gen(); }

  // represents *---*!!!!o--
  template <int t, int s, int b> struct otoposidesobottom {};
  template <int t, int s, int b> otoposidesobottom<t,s,b> operator* (dashes<t, osidesobottom<s,b> >)
  { analog_literals_assert(t > b); return gen(); }
  template <int n, int s, int b> otoposidesobottom<n+1,s,b> operator- (odashes<n>, osidesobottom<s,b>)
  { analog_literals_assert(n+1 > b); return gen(); }
  template <int p, int q, int s, int b> otoposidesobottom<p+q+1,s,b> operator- (odashes<p>, dashes<q, osidesobottom<s,b> >)
  { analog_literals_assert(p+q+1 > b); return gen(); }

  // Constructing a complete rectangle: * (--*!!!!*--o)
  template <int s> rectangle<0,s> operator* (osidesobottomo<s,0>) { return gen(); }
  template <int t, int s, int b> rectangle<t,s> operator* (dashes<t, osidesobottomo<s,b> >)
  { analog_literals_assert(t == b); return gen(); }

  // Constructing a complete rectangle: (o--) - (--*!!!!*-----o)
  template <int t, int s, int b> rectangle<b,s> operator- (osym, dashes<t, osidesobottomo<s,b> >)
  { analog_literals_assert(t+1 == b); return gen(); }
  template <int t, int s, int b> rectangle<b,s> operator- (odashes<t>, osidesobottomo<s,b>)
  { analog_literals_assert(t+1 == b); return gen(); }
  template <int p, int q, int s, int b> rectangle<b,s> operator- (odashes<p>, dashes<q, osidesobottomo<s,b> >)
  { analog_literals_assert(p+q+1 == b); return gen(); }

  // Constructing a complete rectangle: (*-----*!!!!o--) - (--o)
  template <int t, int s, int b> rectangle<t,s> operator- (otoposidesobottom<t,s,b>, osym)
  { analog_literals_assert(t == b+1); return gen(); }
  template <int t, int s, int b, int n> rectangle<t,s> operator- (otoposidesobottom<t,s,b>, dashes<n, osym>)
  { analog_literals_assert(t == b+n+1); return gen(); }

  // Constructing a complete rectangle: (o---o) | (!!!o---o)
  template <int t, int s2, int b> rectangle<t, (s2+1)/2> operator| (line<t>, bangs<s2, line<b> >)
  { analog_literals_assert(s2 % 2 == 1); return gen(); }

} // analog_literals

#endif // header guard

#ifdef ANALOGLITERALS_TEST

int main ()
{
  using namespace analog_literals::symbols;
  using namespace analog_literals::shapes;

  line<7>(*-------o);

  line<7>(o-------o);
  line<7>(o---~~--o);
  line<1>(o-o);
  line<2>(o-~o);

  rectangle<5, 3>(*-----*
                  !     !
                  !     !
                  !     !
                  *-----o);

  rectangle<5, 3>(*-----*
                  !     !
                  !     !
                  !     !
                  o-----o);

  rectangle<5, 3>(*-----o
                  |     !
                  !     !
                  !     !
                  *-----o);

  rectangle<5, 3>(*-----o
                  |     !
                  !     !
                  !     !
                  o-----o);

  rectangle<5, 3>(o-----*
                  !     !
                  !     !
                  !     !
                  *-----o);

  rectangle<5, 3>(o-----*
                  !     !
                  !     !
                  !     !
                  o-----o);

  rectangle<5, 3>(o-----o
                  |     !
                  !     !
                  !     !
                  *-----o);

  rectangle<5, 3>(o-----o
                  |     !
                  !     !
                  !     !
                  o-----o);
}

#endif // testing
