
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

// Constructions (i.e., the messy part)

  template <int,typename> struct Ls {}; // diagonals (L & &&)
  template <int,typename> struct hs {}; // horizontals (- ~ --)
  template <int,typename> struct vs {}; // verticals (! | ||)
  template <int n, typename T> struct os {}; // corners (o *)

  template <int,typename> struct Lvs {};  // L|L|L|
  template <int,typename> struct Lvvs {}; // L|!L|!L|!
  template <int n> struct hline : line<n> {}; // o---o
  template <int n> struct vline : line<n> {}; // o|!!o

  struct nil {};

  static os<1,nil> o;
  static hline<0> oo;
  static os<1,hline<0> > ooo;
  static Ls<1,nil> L;
  static Ls<2,nil> LL;
  static Ls<3,nil> LLL;

  template<typename T> os<1,T> operator* (T) { return gen(); }
  template<int n, typename T> os<n+1,T> operator* (os<n,T>) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+1,T> operator* (os<p,nil>, os<q,T>) { return gen(); }

  template<typename T> Ls<1,T> operator& (T) { return gen(); }
  template<int n, typename T> Ls<n+1,T> operator& (Ls<n,T>) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+1,T> operator& (Ls<p,nil>, Ls<q,T>) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+2,T> operator&& (Ls<p,nil>, Ls<q,T>) { return gen(); }

  template<typename T> hs<1,T> operator~ (T) { return gen(); }
  template<typename T> hs<1,T> operator- (T) { return gen(); }
  template<typename T> hs<2,T> operator-- (T) { return gen(); }
  template<int n, typename T> hs<n+1,T> operator~ (hs<n,T>) { return gen(); }
  template<int n, typename T> hs<n+1,T> operator- (hs<n,T>) { return gen(); }
  template<int n, typename T> hs<n+2,T> operator-- (hs<n,T>) { return gen(); }

  template<typename T> vs<1,T> operator! (T) { return gen(); }
  template<int n, typename T> vs<n+1,T> operator! (vs<n,T>) { return gen(); }

  // o--
  template<int n> os<n,hs<2,nil> > operator-- (os<n,nil>, int) { return gen(); }
  template<int n, int t> os<n,hs<t+2,nil> > operator-- (os<n,hs<t,nil> >, int) { return gen(); }

  // o-----o
  hline<0> operator* (os<1,nil>) { return gen(); }
  template<int x> hline<x> operator* (hs<x,os<1,nil> >) { return gen(); }
  hline<1> operator- (os<1,nil>, os<1,nil>) { return gen(); }
  template<int q> hline<q+1> operator- (os<1,nil>, hs<q,os<1,nil> >) { return gen(); }
  template<int p, int q> hline<p+q+1> operator- (os<1,hs<p,nil> >, hs<q,os<1,nil> >) { return gen(); }
  template<int p> hline<p+1> operator- (os<1,hs<p,nil> >, os<1,nil>) { return gen(); }

  template<int n, int q> os<n,hline<q> > operator* (os<n,nil>, hs<q,os<1,nil> >) { return gen(); }
  template<int p, int n, int q> hs<p,os<n,hline<q> > > operator* (hs<p,os<n,nil> >, hs<q,os<1,nil> >) { return gen(); }
  template<int p, int n, int q> vs<p,os<n,hline<q> > > operator* (vs<p,os<n,nil> >, hs<q,os<1,nil> >) { return gen(); }

  template<int y> vs<y,hline<1> > operator- (vs<y,os<1,nil> >, os<1,nil>) { return gen(); }
  template<int y, int p> vs<y,hline<p+1> > operator- (vs<y,os<1,hs<p,nil> > >, os<1,nil>) { return gen(); }
  template<int y, int p, int q> vs<y,hline<p+q+1> > operator- (vs<y,os<1,hs<p,nil> > >, hs<q,os<1,nil> >) { return gen(); }
  template<int y, int q> vs<y,hline<q+1> > operator- (vs<y,os<1,nil> >, hs<q,os<1,nil> >) { return gen(); }

  // o|!!!o
  template<int y> vline<y> operator* (vs<y,os<1,nil> >) { return gen(); }
  vline<1> operator| (os<1,nil>, os<1,nil>) { return gen(); }
  template<int y> vline<y+1> operator| (os<1,nil>, vs<y,os<1,nil> >) { return gen(); }

  template<int x, int y> os<1,hs<x,vline<y> > > operator* (os<1,hs<x,nil> >, vs<y,os<1,nil> >) { return gen(); }

  // Misc constructions
  template<int x> os<1,hs<x,hline<0> > > operator* (os<1,hs<x,nil> >, os<1,nil>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<2,T> > > operator* (os<1,hs<x,nil> >, os<1,T>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,T> > > operator* (os<1,hs<x,nil> >, T) { return gen(); }
  template<int x, typename T> os<1,hs<x+1,T> > operator- (os<1,nil>, hs<x,T>) { return gen(); }
  template<typename T> os<1,hs<1,T> > operator- (os<1,nil>, T) { return gen(); }
  template<int x, typename T> os<1,hs<x+1,T> > operator- (os<1,hs<x,nil> >, T) { return gen(); }
  template<int p, int q, typename T> os<1,hs<p+q+1,T> > operator- (os<1,hs<p,nil> >, hs<q,T>) { return gen(); }

  // Constructing a rectangle: o---o|!!!o---o
  rectangle<0,0> operator* (os<1,hline<0> >) { return gen(); }
  template<int x> rectangle<x,0> operator* (hs<x,os<1,hline<x> > >) { return gen(); }
  template<int y> rectangle<0,y/2> operator* (os<1,vs<y,hline<0> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (hs<x,os<1,vs<y,hline<x> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<1,0> operator- (os<1,nil>, os<1,hline<1> >) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1,nil>, os<1,vs<y,hline<1> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x> rectangle<x,0> operator- (os<1,nil>, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int q, int y> rectangle<q+1,y/2> operator- (os<1,nil>, hs<q,os<1,vs<y,hline<q+1> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p, int q> rectangle<p+q+1,0> operator- (os<1,hs<p,nil> >, hs<q,os<1,hline<p+q+1> > >) { return gen(); }
  template<int p, int q, int y> rectangle<p+q+1,y/2> operator- (os<1,hs<p,nil> >, hs<q,os<1,vs<y,hline<p+q+1> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p> rectangle<p+1,0> operator- (os<1,hs<p,nil> >, os<1,hline<p+1> >) { return gen(); }
  template<int p, int y> rectangle<p+1,y/2> operator- (os<1,hs<p,nil> >, os<1,vs<y,hline<p+1> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<0,0> operator* (os<1,nil>, hline<0>) { return gen(); }
  template<int x> rectangle<x,0> operator* (os<1,hs<x,nil> >, hline<x>) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (os<1,hs<x,nil> >, vs<y,hline<x> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int y> rectangle<0,y/2> operator* (os<1,nil>, vs<y,hline<0> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  template<int x, int y> rectangle<x,y/2+1> operator| (hline<x>, vs<y,hline<x> >)
  { analog_literals_assert(y % 2 == 1); return gen(); }
  rectangle<0,1> operator|| (hline<0>, hline<0>) { return gen(); }
  template<int y> rectangle<0,y/2+1> operator|| (hline<0>, vs<y,hline<0> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<0,0> operator* (hline<0>, os<1,nil>) { return gen(); }
  template<int x> rectangle<x,0> operator* (hline<x>, hs<x,os<1,nil> >) { return gen(); }

  rectangle<1,0> operator- (os<1,hs<1,hline<0> > >, os<1,nil>) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1,hs<1,vline<y> > >, os<1,nil>)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p, int q> rectangle<p+q+1,0> operator- (os<1,hs<p+q+1,os<2,hs<p,nil> > > >, hs<q,os<1,nil> >) { return gen(); }
  template<int p, int q, int y> rectangle<p+q+1,y/2> operator- (os<1,hs<p+q+1,os<1,vs<y,os<1,hs<p,nil> > > > > >, hs<q,os<1,nil> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int q> rectangle<q+1,0> operator- (os<1,hs<q+1,hline<0> > >, hs<q,os<1,nil> >) { return gen(); }
  template<int q, int y> rectangle<q+1,y/2> operator- (os<1,hs<q+1,vline<y> > >, hs<q,os<1,nil> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p> rectangle<p+1,0> operator- (os<1,hs<p+1,os<2,hs<p,nil> > > >, os<1,nil>)
  { return gen(); }
  template<int p, int y> rectangle<p+1,y/2> operator- (os<1,hs<p+1,os<1,vs<y,os<1,hs<p,nil> > > > > >, os<1,nil>)
  { return gen(); }


// Convenience namespaces that can be "using namespace"'d:

  namespace symbols
  {
    using analog_literals::o;
    using analog_literals::oo;
    using analog_literals::ooo;
    using analog_literals::L;
    using analog_literals::LL;
    using analog_literals::LLL;
  }

  namespace shapes
  {
    using analog_literals::line;
    using analog_literals::rectangle;
    using analog_literals::cuboid;
  }

} // analog_literals

#endif // header guard