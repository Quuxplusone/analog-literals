
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
  bangs<1, osym > operator! (osym) { return gen(); }
  template <int n> bangs<1, line<n> > operator! (line<n>) { return gen(); }
  template <int n> bangs<1, odashes<n> > operator! (odashes<n>) { return gen(); }
  template <typename T, int n> bangs<n+1, T> operator! (bangs<n,T>) { return gen(); }

  template <int s, int b> bangs<s, line<b+1> > operator- (bangs<s, odashes<b> >, osym) { return gen(); }
  template <int s, int p, int q> bangs<s, line<p+q+1> > operator- (bangs<s, odashes<p> >, dashes<q, osym>) { return gen(); }
  template <int s> bangs<s, line<1> > operator- (bangs<s, osym>, osym) { return gen(); }
  template <int s, int q> bangs<s, line<q+1> > operator- (bangs<s, osym>, dashes<q, osym>) { return gen(); }

  // osidesobottomo represents *!!!!*--o
  template <int s, int b> struct osidesobottomo {};
  template <int b> osidesobottomo<0,b> operator* (line<b>) { return gen(); }
  template <int b> osidesobottomo<0,b> operator* (osym, dashes<b, osym>) { return gen(); }
  template <int s, int b> osidesobottomo<s,b> operator* (bangs<s, line<b> >)
  { analog_literals_assert(s % 2 == 0); return gen(); }

  template <int s, int b> dashes<1, osidesobottomo<s,b> > operator~ (osidesobottomo<s,b>) { return gen(); }
  template <int s, int b> dashes<1, osidesobottomo<s,b> > operator- (osidesobottomo<s,b>) { return gen(); }
  template <int s, int b> dashes<2, osidesobottomo<s,b> > operator-- (osidesobottomo<s,b>) { return gen(); }

  template <int t, int b> dashes<t, osidesobottomo<0,b> > operator* (dashes<t,osym>, dashes<b,osym>) { return gen(); }

  // osidesobottom represents *!!!!o--
  template <int s, int b> struct osidesobottom {};
  template <int b> osidesobottom<0,b> operator* (odashes<b>) { return gen(); }
  template <int s, int b> osidesobottom<s,b> operator* (bangs<s, odashes<b> >)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int s> osidesobottom<s, 0> operator* (bangs<s, osym >)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int s, int b> osidesobottom<s+1, b> operator| (osym, bangs<s, odashes<b> >)
  { analog_literals_assert(s % 2 == 1); return gen(); }
  template <int t, int s, int b> dashes<t, osidesobottom<s+1, b> > operator| (dashes<t, osym>, bangs<s, odashes<b> >)
  { analog_literals_assert(s % 2 == 1); return gen(); }

  dashes<1, osidesobottom<0,0> > operator~ (line<0>) { return gen(); }
  dashes<1, osidesobottom<0,0> > operator- (line<0>) { return gen(); }
  dashes<2, osidesobottom<0,0> > operator-- (line<0>) { return gen(); }

  template <int s, int b> dashes<1, osidesobottom<s,b> > operator~ (osidesobottom<s,b>) { return gen(); }
  template <int s, int b> dashes<1, osidesobottom<s,b> > operator- (osidesobottom<s,b>) { return gen(); }
  template <int s, int b> dashes<2, osidesobottom<s,b> > operator-- (osidesobottom<s,b>) { return gen(); }

  // otoposidesobottom represents *---*!!!!o--
  template <int t, int s, int b> struct otoposidesobottom {};
  template <int t> otoposidesobottom<t,0,0> operator* (odashes<t>, osym) { return gen(); }
  template <int t, int b> otoposidesobottom<t,0,b> operator* (odashes<t>, odashes<b>) { return gen(); }
  template <int t, int s, int b> otoposidesobottom<t, s, b> operator* (odashes<t>, bangs<s, odashes<b> >)
  { analog_literals_assert(s % 2 == 0); analog_literals_assert(t > b); return gen(); }
  template <int t, int s, int b> otoposidesobottom<t,s,b> operator* (dashes<t, osidesobottom<s,b> >)
  { analog_literals_assert(t > b); return gen(); }
  template <int t, int s> otoposidesobottom<t, s, 0> operator* (odashes<t>, bangs<s, osym>)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int n, int s, int b> otoposidesobottom<n+1,s,b> operator- (odashes<n>, osidesobottom<s,b>)
  { analog_literals_assert(n+1 > b); return gen(); }
  template <int p, int q, int s, int b> otoposidesobottom<p+q+1,s,b> operator- (odashes<p>, dashes<q, osidesobottom<s,b> >)
  { analog_literals_assert(p+q+1 > b); return gen(); }
  template <int q, int s, int b> otoposidesobottom<q+1,s,b> operator- (osym, dashes<q, osidesobottom<s,b> >)
  { analog_literals_assert(q+1 > b); return gen(); }
  template <int s, int b> otoposidesobottom<1,s,b> operator- (osym, osidesobottom<s,b>)
  { analog_literals_assert(b <= 1); return gen(); }
  template <int t> otoposidesobottom<t+1,0,0> operator- (odashes<t>, line<0>) { return gen(); }
  otoposidesobottom<1,0,0> operator- (osym, line<0>) { return gen(); }

  // Constructing a complete rectangle: * (--*!!!!*--o)
  template <int s> rectangle<0,s/2> operator* (osidesobottomo<s,0>)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int t, int s, int b> rectangle<t,s/2> operator* (dashes<t, osidesobottomo<s,b> >)
  { analog_literals_assert(t == b); analog_literals_assert(s % 2 == 0); return gen(); }

  // Constructing a complete rectangle: (o--) - (--*!!!!*-----o)
  template <int s> rectangle<1,s/2> operator- (osym, osidesobottomo<s,1>)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int t, int s, int b> rectangle<b,s/2> operator- (osym, dashes<t, osidesobottomo<s,b> >)
  { analog_literals_assert(t+1 == b); analog_literals_assert(s % 2 == 0); return gen(); }
  template <int t, int s, int b> rectangle<b,s/2> operator- (odashes<t>, osidesobottomo<s,b>)
  { analog_literals_assert(t+1 == b); analog_literals_assert(s % 2 == 0); return gen(); }
  template <int p, int q, int s, int b> rectangle<b,s/2> operator- (odashes<p>, dashes<q, osidesobottomo<s,b> >)
  { analog_literals_assert(p+q+1 == b); analog_literals_assert(s % 2 == 0); return gen(); }

  // Constructing a complete rectangle: (*-----*!!!!o--) - (--o)
  template <int t, int s, int b> rectangle<t,s/2> operator- (otoposidesobottom<t,s,b>, osym)
  { analog_literals_assert(t == b+1); analog_literals_assert(s % 2 == 0); return gen(); }
  template <int t, int s, int b, int n> rectangle<t,s/2> operator- (otoposidesobottom<t,s,b>, dashes<n, osym>)
  { analog_literals_assert(t == b+n+1); analog_literals_assert(s % 2 == 0); return gen(); }

  // Constructing a complete rectangle: (o---o) | (!!!o---o)
  template <int t, int s, int b> rectangle<t, (s+1)/2> operator| (line<t>, bangs<s, line<b> >)
  { analog_literals_assert(s % 2 == 1); return gen(); }

  // Constructing a complete rectangle: (*---o) * (---o)
  template <int t, int b> rectangle<t, 0> operator* (line<t>, dashes<b, osym>)
  { analog_literals_assert(t == b); return gen(); }
  rectangle<0,0> operator* (line<0>, osym) { return gen(); }

  // Constructing a complete rectangle: (o--) * (!!!!*--o)
  rectangle<0,0> operator* (osym, line<0>) { return gen(); }
  template <int t, int s, int b> rectangle<t,s/2> operator* (odashes<t>, bangs<s, line<b> >)
  { analog_literals_assert(t == b); analog_literals_assert(s % 2 == 0); return gen(); }
  template <int s> rectangle<0,s/2> operator* (osym, bangs<s, line<0> >)
  { analog_literals_assert(s % 2 == 0); return gen(); }
  template <int t, int b> rectangle<t,0> operator* (odashes<t>, line<b>)
  { analog_literals_assert(t == b); return gen(); }

  // Constructing a complete rectangle: (*o) || (!!!!*o)
  rectangle<0,1> operator|| (line<0>, line<0>) { return gen(); }
  template <int s> rectangle<0, s/2 + 1> operator|| (line<0>, bangs<s, line<0> >) { return gen(); }

// Constructions required for 3-D literals (cuboids) begin here

  bangs<1,Lsym> operator! (Lsym) { return gen(); }

  osidesobottomo<0,0> operator* (osym, osym) { return gen(); }
  osidesobottomo<0,0> operator* (line<0>) { return gen(); }

  // odiag represents *!L|L|L
  template <int l> struct odiag {};
  odiag<1> operator* (bangs<1,Lsym>) { return gen(); }

  template <int l> dashes<1, odiag<l> > operator~ (odiag<l>) { return gen(); }
  template <int l> dashes<1, odiag<l> > operator- (odiag<l>) { return gen(); }
  template <int l> dashes<2, odiag<l> > operator-- (odiag<l>) { return gen(); }

  // base represents L|!L|!L
  template <int l> struct base {};
  base<2> operator| (Lsym, bangs<1,Lsym>) { return gen(); }
  template <int l> base<l+1> operator| (base<l>, bangs<1,Lsym>) { return gen(); }

  // baseline represents L|!L|!*~~o
  template <int l, int b> struct baseline {};
  template <int b> baseline<1,b> operator| (Lsym, bangs<1, line<b> >) { return gen(); }
  template <int l, int b> baseline<l+1,b> operator| (Lsym, bangs<1, baseline<l,b> >) { return gen(); }
  template <int l> baseline<l,0> operator| (base<l>, bangs<1, line<0> >) { return gen(); }

  // otopodiag represents *~~*!L|L|L
  template <int t, int l> struct otopodiag {};
  otopodiag<0,1> operator* (odiag<1>) { return gen(); }
  otopodiag<0,1> operator* (osym, bangs<1,Lsym>) { return gen(); }
  template <int t> otopodiag<t,1> operator* (odashes<t>, bangs<1,Lsym>) { return gen(); }
  template <int t> otopodiag<t,1> operator* (dashes<t, odiag<1> >) { return gen(); }
  otopodiag<1,1> operator- (osym, odiag<1>) { return gen(); }
  template <int t> otopodiag<t+1,1> operator- (osym, dashes<t, odiag<1> >) { return gen(); }
  template <int t> otopodiag<t+1,1> operator- (odashes<t>, odiag<1>) { return gen(); }
  template <int p, int q> otopodiag<p+q+1,1> operator- (odashes<p>, dashes<q, odiag<1> >) { return gen(); }
  template <int t> otopodiag<t,1> operator| (line<t>, Lsym) { return gen(); }
  template <int t, int l> otopodiag<t,l+1> operator| (otopodiag<t,l>, Lsym) { return gen(); }

  // otopodiagotopo represents *~~*!L|L|L|*--o
  template <int t, int d> struct otopodiagotopo {};
  template <int t, int d> otopodiagotopo<t,d> operator| (otopodiag<t,d>, line<t>) { return gen(); }

  // obase represents *!!L|!L|!L
  template <int l> struct obase {};
  obase<1> operator* (bangs<2,Lsym>) { return gen(); }
  template <int l> obase<l> operator|| (osym, base<l>) { return gen(); }

  bangs<1, obase<1> > operator! (obase<1>) { return gen(); }

  // olongobase represents *!!!!!!*!!L|!L|!L
  template <int l, int b> struct olongobase {};
  olongobase<0,1> operator* (osym, bangs<2,Lsym>) { return gen(); }
  template <int b> olongobase<0,b> operator* (obase<b>) { return gen(); }
  template <int l, int b> olongobase<l,b> operator* (bangs<l, obase<b> >)
  { analog_literals_assert(l % 3 == 0); return gen(); }

  template <int l, int b> dashes<1, olongobase<l,b> > operator~ (olongobase<l,b>) { return gen(); }
  template <int l, int b> dashes<1, olongobase<l,b> > operator- (olongobase<l,b>) { return gen(); }
  template <int l, int b> dashes<2, olongobase<l,b> > operator-- (olongobase<l,b>) { return gen(); }

  template <int t> dashes<t, olongobase<0,1> > operator* (dashes<t,osym>, bangs<2,Lsym>) { return gen(); }

  // otopolongobase represents *~~*!!!!!!*!!L|!L|!L
  template <int t, int l, int b> struct otopolongobase {};
  otopolongobase<0,0,1> operator* (osym, obase<1>) { return gen(); }
  template <int l> otopolongobase<0,l,1> operator* (osym, bangs<l, obase<1> >) { return gen(); }
  template <int l> otopolongobase<0,l,0> operator* (osidesobottom<l,0>) { return gen(); }
  template <int l, int b> otopolongobase<0,l,b> operator* (olongobase<l,b>) { return gen(); }
  template <int t> otopolongobase<t,0,1> operator* (line<t>, bangs<2,Lsym>) { return gen(); }
  template <int t> otopolongobase<t,0,1> operator* (odashes<t>, obase<1>) { return gen(); }
  template <int l> otopolongobase<0,l,0> operator* (osym, bangs<l,osym>) { return gen(); }
  template <int t, int l> otopolongobase<t,l,1> operator* (odashes<t>, bangs<l, obase<1> >) { return gen(); }
  template <int t, int l, int b> otopolongobase<t,l,b> operator* (dashes<t, olongobase<l,b> >) { return gen(); }
  template <int l> otopolongobase<1,l,1> operator- (osym, olongobase<l,1>) { return gen(); }
  template <int t, int l> otopolongobase<t+1,l,1> operator- (odashes<t>, olongobase<l,1>) { return gen(); }
  template <int t, int l> otopolongobase<t+1,l,1> operator- (osym, dashes<t, olongobase<l,1> >) { return gen(); }
  template <int p, int q, int l> otopolongobase<p+q+1,l,1> operator- (odashes<p>, dashes<q, olongobase<l,1> >) { return gen(); }


  // otopodiagotopolongobase represents o--o!L|L|L|o--o|!!o!!L|!L|!L
  template <int t, int d, int l, int b> struct otopodiagotopolongobase {};
  otopodiagotopolongobase<0,1,0,0> operator| (otopodiag<0,1>, osidesobottomo<0,0>) { return gen(); }
  template <int d> otopodiagotopolongobase<0,d,0,0> operator| (otopodiag<0,d>, osidesobottomo<0,0>) { return gen(); }
  template <int t, int d, int l> otopodiagotopolongobase<t,d,l,0> operator| (otopodiag<t,d>, otoposidesobottom<t,l,0>) { return gen(); }
  template <int t, int d, int l> otopodiagotopolongobase<t,d,l+1,0> operator| (otopodiagotopo<t,d>, bangs<l, osym>) { return gen(); }
  template <int t, int d, int l> otopodiagotopolongobase<t,d,l+1,1> operator| (otopodiagotopo<t,d>, bangs<l, obase<1> >) { return gen(); }
  template <int t, int d, int l, int b> otopodiagotopolongobase<t,d,l,b> operator| (otopodiag<t,d>, otopolongobase<t,l,b>) { return gen(); }
  template <int t, int d, int l, int b> otopodiagotopolongobase<t,d,l,b+1> operator| (otopodiagotopolongobase<t,d,l,b>, bangs<1,Lsym>) { return gen(); }
  template <int t, int d, int l, int b> otopodiagotopolongobase<t,d,l,b+1> operator|| (otopodiagotopolongobase<t,d,l,b>, Lsym) { return gen(); }
  template <int t, int d, int l, int p, int q> otopodiagotopolongobase<t,d,l,p+q> operator|| (otopodiagotopolongobase<t,d,l,p>, base<q>) { return gen(); }

  // Constructing a cuboid: (o-~o|L|o-~o|!L) | (!o-~o)
  template <int t, int l, int d> cuboid<t,d+(l/3)+1,d> operator| (otopodiagotopolongobase<t,d,l,d>, bangs<1,line<t> >)
  { analog_literals_assert(l % 3 == 0); return gen(); }

  // Constructing a cuboid: (oo|L|oo|!L) || (oo)
  template <int d, int l> cuboid<0,d+(l/3)+1,d> operator|| (otopodiagotopolongobase<0,d,l,d>, line<0>)
  { analog_literals_assert(l % 3 == 0); return gen(); }

  // Constructing a cuboid: (oo|L|oo) || (L|!oo)
  template <int p, int q, int l> cuboid<0,p+q+(l/3)+1,p+q> operator|| (otopodiagotopolongobase<0,p+q,l,p>, baseline<q,0>)
  { analog_literals_assert(l % 3 == 0); return gen(); }

} // analog_literals

#endif // header guard

#ifdef ANALOGLITERALS_TEST

int main ()
{
  using namespace analog_literals::symbols;
  using namespace analog_literals::shapes;

  (line<0>)(*o);

  (line<1>)(o-o); (line<1>)(*-o); (line<1>)(*~o);

  (line<2>)(o-~o); (line<2>)(*--o);
  (line<2>)(*-~o); (line<2>)(*~-o);
  (line<2>)(*~~o);

  (line<3>)(o---o); (line<3>)(o-~-o);
  (line<3>)(o-~~o); (line<3>)(*---o);
  (line<3>)(*--~o); (line<3>)(*-~-o);
  (line<3>)(*-~~o); (line<3>)(*~--o);
  (line<3>)(*~-~o); (line<3>)(*~~~o);

  (line<7>)(o-------o);

  (rectangle<0,0>)(***o);
  (rectangle<0,0>)(*o*o);
  (rectangle<0,0>)(o**o);

  (rectangle<0,1>)(**!!*o);             (rectangle<0,2>)(**!!!!*o);
  (rectangle<0,1>)(*o|!*o);             (rectangle<0,2>)(*o|!!!*o);
    (rectangle<0,1>)(*o||*o);             (rectangle<0,2>)(*o||!!*o);
  (rectangle<0,1>)(o*!!*o);             (rectangle<0,2>)(o*!!!!*o);

  (rectangle<1,0>)(*-**-o);
  (rectangle<1,0>)(*-*o-o);
  (rectangle<1,0>)(*-o*-o);
  (rectangle<1,0>)(o-**-o);
  (rectangle<1,0>)(o-*o-o);

  (rectangle<1,1>)(*-* !! *-o);         (rectangle<1,2>)(*-* !!!! *-o);
  (rectangle<1,1>)(*-* !! o-o);         (rectangle<1,2>)(*-* !!!! o-o);
  (rectangle<1,1>)(*-o |! *-o);         (rectangle<1,2>)(*-o |!!! *-o);
  (rectangle<1,1>)(*-o |! o-o);         (rectangle<1,2>)(*-o |!!! o-o);
  (rectangle<1,1>)(o-* !! *-o);         (rectangle<1,2>)(o-* !!!! *-o);
  (rectangle<1,1>)(o-* !! o-o);         (rectangle<1,2>)(o-* !!!! o-o);
  (rectangle<1,1>)(o-o |! *-o);         (rectangle<1,2>)(o-o |!!! *-o);
  (rectangle<1,1>)(o-o |! o-o);         (rectangle<1,2>)(o-o |!!! o-o);

  (rectangle<2,0>)(*--**--o);
  (rectangle<2,0>)(*--*o-~o);
  (rectangle<2,0>)(*--o*--o);
  (rectangle<2,0>)(o--**--o);
    (rectangle<2,0>)(o-~**--o);
  (rectangle<2,0>)(o--*o-~o);
    (rectangle<2,0>)(o-~*o-~o);

  (rectangle<2,1>)(*--* !! *--o);       (rectangle<2,2>)(*--* !!!! *--o);
  (rectangle<2,1>)(*--* !! o-~o);       (rectangle<2,2>)(*--* !!!! o-~o);
  (rectangle<2,1>)(*--o |! *--o);       (rectangle<2,2>)(*--o |!!! *--o);
  (rectangle<2,1>)(*--o |! o-~o);       (rectangle<2,2>)(*--o |!!! o-~o);
  (rectangle<2,1>)(o--* !! *--o);       (rectangle<2,2>)(o--* !!!! *--o);
    (rectangle<2,1>)(o-~* !! *--o);       (rectangle<2,2>)(o-~* !!!! *--o);
  (rectangle<2,1>)(o--* !! o-~o);       (rectangle<2,2>)(o--* !!!! o-~o);
  (rectangle<2,1>)(o-~o |! *--o);       (rectangle<2,2>)(o-~o |!!! *--o);
  (rectangle<2,1>)(o-~o |! o-~o);       (rectangle<2,2>)(o-~o |!!! o-~o);

  (rectangle<3,0>)(*---**---o);
  (rectangle<3,0>)(*---*o---o);
    (rectangle<3,0>)(*---*o-~~o);
  (rectangle<3,0>)(*---o*---o);
  (rectangle<3,0>)(o---**---o);
    (rectangle<3,0>)(o-~~**---o);
  (rectangle<3,0>)(o---*o---o);
    (rectangle<3,0>)(o---*o-~~o);
    (rectangle<3,0>)(o-~~*o---o);
    (rectangle<3,0>)(o-~~*o-~~o);
  (rectangle<3,0>)(o---o*---o);
    (rectangle<3,0>)(o-~~o*---o);

  (rectangle<3,1>)(*---* !! *---o);     (rectangle<3,2>)(*---* !!!! *---o);
  (rectangle<3,1>)(*---* !! o---o);     (rectangle<3,2>)(*---* !!!! o---o);
    (rectangle<3,1>)(*---* !! o-~-o);     (rectangle<3,2>)(*---* !!!! o-~-o);
  (rectangle<3,1>)(*---o |! *---o);     (rectangle<3,2>)(*---o |!!! *---o);
  (rectangle<3,1>)(*---o |! o---o);     (rectangle<3,2>)(*---o |!!! o---o);
    (rectangle<3,1>)(*---o |! o-~-o);     (rectangle<3,2>)(*---o |!!! o-~-o);
  (rectangle<3,1>)(o---* !! *---o);     (rectangle<3,2>)(o---* !!!! *---o);
    (rectangle<3,1>)(o-~-* !! *---o);     (rectangle<3,2>)(o-~-* !!!! *---o);
  (rectangle<3,1>)(o---* !! o---o);     (rectangle<3,2>)(o---* !!!! o---o);
    (rectangle<3,1>)(o---* !! o-~-o);     (rectangle<3,2>)(o---* !!!! o-~-o);
    (rectangle<3,1>)(o-~-* !! o---o);     (rectangle<3,2>)(o-~-* !!!! o---o);
    (rectangle<3,1>)(o-~-* !! o-~-o);     (rectangle<3,2>)(o-~-* !!!! o-~-o);
  (rectangle<3,1>)(o---o |! *---o);     (rectangle<3,2>)(o---o |!!! *---o);
    (rectangle<3,1>)(o---o |! *-~-o);     (rectangle<3,2>)(o---o |!!! *-~-o);
    (rectangle<3,1>)(o-~-o |! *---o);     (rectangle<3,2>)(o-~-o |!!! *---o);
    (rectangle<3,1>)(o-~-o |! *-~-o);     (rectangle<3,2>)(o-~-o |!!! *-~-o);
  (rectangle<3,1>)(o---o |! o---o);     (rectangle<3,2>)(o---o |!!! o---o);
    (rectangle<3,1>)(o---o |! o-~-o);     (rectangle<3,2>)(o---o |!!! o-~-o);
    (rectangle<3,1>)(o-~-o |! o---o);     (rectangle<3,2>)(o-~-o |!!! o---o);
    (rectangle<3,1>)(o-~-o |! o-~-o);     (rectangle<3,2>)(o-~-o |!!! o-~-o);

  (rectangle<4,0>)(*----**----o);
  (rectangle<4,0>)(*----*o---~o);
      (rectangle<4,0>)(*----*o-~~~o);
  (rectangle<4,0>)(*----o*----o);
  (rectangle<4,0>)(o----**----o);
      (rectangle<4,0>)(o---~**----o);
      (rectangle<4,0>)(o-~~~**----o);
  (rectangle<4,0>)(o----*o---~o);
      (rectangle<4,0>)(o----*o-~~~o);
      (rectangle<4,0>)(o---~*o---~o);
      (rectangle<4,0>)(o-~~~*o-~~~o);
  (rectangle<4,0>)(o---~o*----o);
      (rectangle<4,0>)(o-~~~o*----o);

  (rectangle<4,1>)(*----* !! *----o);   (rectangle<4,2>)(*----* !!!! *----o);
  (rectangle<4,1>)(*----* !! o---~o);   (rectangle<4,2>)(*----* !!!! o---~o);
    (rectangle<4,1>)(*----* !! o-~--o);   (rectangle<4,2>)(*----* !!!! o-~--o);
  (rectangle<4,1>)(*----o |! *----o);   (rectangle<4,2>)(*----o |!!! *----o);
  (rectangle<4,1>)(*----o |! o---~o);   (rectangle<4,2>)(*----o |!!! o---~o);
    (rectangle<4,1>)(*----o |! o-~--o);   (rectangle<4,2>)(*----o |!!! o-~--o);
  (rectangle<4,1>)(o----* !! *----o);   (rectangle<4,2>)(o----* !!!! *----o);
    (rectangle<4,1>)(o---~* !! *----o);   (rectangle<4,2>)(o---~* !!!! *----o);
    (rectangle<4,1>)(o-~--* !! *----o);   (rectangle<4,2>)(o-~--* !!!! *----o);
  (rectangle<4,1>)(o----* !! o---~o);   (rectangle<4,2>)(o----* !!!! o---~o);
    (rectangle<4,1>)(o----* !! o-~--o);   (rectangle<4,2>)(o----* !!!! o-~--o);
    (rectangle<4,1>)(o---~* !! o---~o);   (rectangle<4,2>)(o---~* !!!! o---~o);
    (rectangle<4,1>)(o---~* !! o-~--o);   (rectangle<4,2>)(o---~* !!!! o-~--o);
    (rectangle<4,1>)(o-~--* !! o---~o);   (rectangle<4,2>)(o-~--* !!!! o---~o);
    (rectangle<4,1>)(o-~--* !! o-~--o);   (rectangle<4,2>)(o-~--* !!!! o-~--o);
  (rectangle<4,1>)(o---~o |! *----o);   (rectangle<4,2>)(o---~o |!!! *----o);
    (rectangle<4,1>)(o-~--o |! *----o);   (rectangle<4,2>)(o-~--o |!!! *----o);
  (rectangle<4,1>)(o---~o |! o---~o);   (rectangle<4,2>)(o---~o |!!! o---~o);
    (rectangle<4,1>)(o---~o |! o-~~~o);   (rectangle<4,2>)(o---~o |!!! o-~~~o);
    (rectangle<4,1>)(o-~~~o |! o---~o);   (rectangle<4,2>)(o-~~~o |!!! o---~o);
    (rectangle<4,1>)(o-~~~o |! o-~~~o);   (rectangle<4,2>)(o-~~~o |!!! o-~~~o);

  (rectangle<5,0>)(*-----**-----o);
  (rectangle<5,0>)(*-----*o-----o);
    (rectangle<5,0>)(*-----*o---~~o);
    (rectangle<5,0>)(*-----*o-~~~~o);
  (rectangle<5,0>)(*-----o*-----o);
  (rectangle<5,0>)(o-----**-----o);
    (rectangle<5,0>)(o---~~**-----o);
    (rectangle<5,0>)(o-~~~~**-----o);
  (rectangle<5,0>)(o-----*o-----o);
    (rectangle<5,0>)(o-----*o---~~o);
    (rectangle<5,0>)(o-----*o-~~~~o);
    (rectangle<5,0>)(o---~~*o-----o);
    (rectangle<5,0>)(o---~~*o---~~o);
    (rectangle<5,0>)(o---~~*o-~~~~o);
    (rectangle<5,0>)(o-~~~~*o-----o);
    (rectangle<5,0>)(o-~~~~*o---~~o);
    (rectangle<5,0>)(o-~~~~*o-~~~~o);
  (rectangle<5,0>)(o-----o*-----o);
    (rectangle<5,0>)(o---~~o*-----o);
    (rectangle<5,0>)(o-~~~~o*-----o);

  (rectangle<5,1>)(*-----* !! *-----o);     (rectangle<5,2>)(*-----* !!!! *-----o);
  (rectangle<5,1>)(*-----* !! o-----o);     (rectangle<5,2>)(*-----* !!!! o-----o);
    (rectangle<5,1>)(*-----* !! o---~~o);     (rectangle<5,2>)(*-----* !!!! o---~~o);
    (rectangle<5,1>)(*-----* !! o-~~~~o);     (rectangle<5,2>)(*-----* !!!! o-~~~~o);
  (rectangle<5,1>)(*-----o |! *-----o);     (rectangle<5,2>)(*-----o |!!! *-----o);
  (rectangle<5,1>)(*-----o |! o-----o);     (rectangle<5,2>)(*-----o |!!! o-----o);
    (rectangle<5,1>)(*-----o |! o---~~o);     (rectangle<5,2>)(*-----o |!!! o---~~o);
    (rectangle<5,1>)(*-----o |! o-~~~~o);     (rectangle<5,2>)(*-----o |!!! o-~~~~o);
  (rectangle<5,1>)(o-----* !! *-----o);     (rectangle<5,2>)(o-----* !!!! *-----o);
    (rectangle<5,1>)(o---~~* !! *-----o);     (rectangle<5,2>)(o---~~* !!!! *-----o);
    (rectangle<5,1>)(o-~~~~* !! *-----o);     (rectangle<5,2>)(o-~~~~* !!!! *-----o);
  (rectangle<5,1>)(o-----* !! o-----o);     (rectangle<5,2>)(o-----* !!!! o-----o);
    (rectangle<5,1>)(o-----* !! o---~~o);     (rectangle<5,2>)(o-----* !!!! o---~~o);
    (rectangle<5,1>)(o-----* !! o-~~~~o);     (rectangle<5,2>)(o-----* !!!! o-~~~~o);
    (rectangle<5,1>)(o---~~* !! o-----o);     (rectangle<5,2>)(o---~~* !!!! o-----o);
    (rectangle<5,1>)(o---~~* !! o---~~o);     (rectangle<5,2>)(o---~~* !!!! o---~~o);
    (rectangle<5,1>)(o---~~* !! o-~~~~o);     (rectangle<5,2>)(o---~~* !!!! o-~~~~o);
    (rectangle<5,1>)(o-~~~~* !! o-----o);     (rectangle<5,2>)(o-~~~~* !!!! o-----o);
    (rectangle<5,1>)(o-~~~~* !! o---~~o);     (rectangle<5,2>)(o-~~~~* !!!! o---~~o);
    (rectangle<5,1>)(o-~~~~* !! o-~~~~o);     (rectangle<5,2>)(o-~~~~* !!!! o-~~~~o);
  (rectangle<5,1>)(o-----o |! *-----o);     (rectangle<5,2>)(o-----o |!!! *-----o);
    (rectangle<5,1>)(o---~~o |! *-----o);     (rectangle<5,2>)(o---~~o |!!! *-----o);
    (rectangle<5,1>)(o-~~~~o |! *-----o);     (rectangle<5,2>)(o-~~~~o |!!! *-----o);
  (rectangle<5,1>)(o-----o |! o-----o);     (rectangle<5,2>)(o-----o |!!! o-----o);
    (rectangle<5,1>)(o-----o |! o---~~o);     (rectangle<5,2>)(o-----o |!!! o---~~o);
    (rectangle<5,1>)(o-----o |! o-~~~~o);     (rectangle<5,2>)(o-----o |!!! o-~~~~o);
    (rectangle<5,1>)(o---~~o |! o-----o);     (rectangle<5,2>)(o---~~o |!!! o-----o);
    (rectangle<5,1>)(o---~~o |! o---~~o);     (rectangle<5,2>)(o---~~o |!!! o---~~o);
    (rectangle<5,1>)(o---~~o |! o-~~~~o);     (rectangle<5,2>)(o---~~o |!!! o-~~~~o);
    (rectangle<5,1>)(o-~~~~o |! o-----o);     (rectangle<5,2>)(o-~~~~o |!!! o-----o);
    (rectangle<5,1>)(o-~~~~o |! o---~~o);     (rectangle<5,2>)(o-~~~~o |!!! o---~~o);
    (rectangle<5,1>)(o-~~~~o |! o-~~~~o);     (rectangle<5,2>)(o-~~~~o |!!! o-~~~~o);

  (cuboid<0,2,1>)(**!L|***!!L|!*o);
  (cuboid<0,2,1>)(**!L|**o|!L|!*o);
  (cuboid<0,2,1>)(**!L|*o*!!L|!*o);
  (cuboid<0,2,1>)(**!L|o**!!L|!*o);
  (cuboid<0,2,1>)(**!L|o*o|!L|!*o);       
  (cuboid<0,2,1>)(*o|L|***!!L|!*o);
  (cuboid<0,2,1>)(*o|L|**o|!L|!*o);
  (cuboid<0,2,1>)(*o|L|*o*!!L|!*o);
  (cuboid<0,2,1>)(*o|L|o**!!L|!*o);
  (cuboid<0,2,1>)(*o|L|o*o|!L|!*o);
  (cuboid<0,2,1>)(o*!L|***!!L|!*o);
  (cuboid<0,2,1>)(o*!L|**o|!L|!*o);
  (cuboid<0,2,1>)(o*!L|*o*!!L|!*o);
  (cuboid<0,2,1>)(o*!L|o**!!L|!*o);
  (cuboid<0,2,1>)(o*!L|o*o|!L|!*o);
  (cuboid<0,2,1>)(**!L|***!!L||*o);
  (cuboid<0,2,1>)(**!L|**o||L|!*o);
  (cuboid<0,2,1>)(**!L|**o||L||*o);

  (cuboid<1,2,1>)(*~*!L|*~**!!L|!*~o);
  (cuboid<1,2,1>)(*~*!L|*~**!!L|!o-o);
  (cuboid<1,2,1>)(*~*!L|*~*o|!L|!*~o);
  (cuboid<1,2,1>)(*~*!L|*~o*!!L|!*~o);
  (cuboid<1,2,1>)(*~*!L|o-**!!L|!*~o);
  (cuboid<1,2,1>)(*~*!L|o-*o|!L|!*~o);
  (cuboid<1,2,1>)(*~*!L|o-o*!!L|!*~o);
  (cuboid<1,2,1>)(*~o|L|*~**!!L|!*~o);
  (cuboid<1,2,1>)(o-o|L|o-o*!!L|!o-o);

  (cuboid<5,2,1>)(*~~~~~*!L|*~~~~~**!!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|*~~~~~**!!L|!o-~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|*~~~~~**!!L|!o---~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|*~~~~~*o|!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o-~~~~*o|!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o---~~*o|!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|*~~~~~o*!!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o-~~~~o*!!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o---~~o*!!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o-~~~~**!!L|!*~~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o---~~**!!L|!o-~~~~o);
  (cuboid<5,2,1>)(*~~~~~*!L|o-----**!!L|!o---~~o);

}

#endif // testing
