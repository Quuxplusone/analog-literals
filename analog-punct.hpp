
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

  struct nil {};

  template <int,typename=nil> struct Ls {}; // diagonals (L & &&)
  template <int,typename=nil> struct hs {}; // horizontals (- ~ --)
  template <int,typename=nil> struct vs {}; // verticals (! | ||)
  template <int, typename=nil> struct os {}; // corners (o *)

  template <int,typename=nil> struct vLs {};  // upper diagonal of cuboid: !L|L|L
  template <int,typename=nil> struct Lvvs {}; // lower diagonal of cuboid: L|!L|!L|!
  template <int n> struct hline : line<n> {}; // o---o
  template <int n> struct vline : line<n> {}; // o|!!o

  static os<1> o;
  static hline<0> oo;
  static os<1,hline<0> > ooo;
  static Ls<1> L;
  static Ls<2> LL;
  static Ls<3> LLL;

  template<typename T> os<1,T> operator* (T) { return gen(); }
  template<int n, typename T> os<n+1,T> operator* (os<n,T>) { return gen(); }
  os<1,hline<0> > operator* (os<1>, os<1>) { return gen(); }
  template<int n, typename T> os<n+2,T> operator* (os<1>, os<n,T>) { return gen(); }
  template<typename T> os<2,T> operator* (os<1>, T) { return gen(); }

  template<typename T> Ls<1,T> operator& (T) { return gen(); }
  template<int n, typename T> Ls<n+1,T> operator& (Ls<n,T>) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+1,T> operator& (Ls<p>, Ls<q,T>) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+2,T> operator&& (Ls<p>, Ls<q,T>) { return gen(); }

  template<typename T> hs<1,T> operator~ (T) { return gen(); }
  template<typename T> hs<1,T> operator- (T) { return gen(); }
  template<typename T> hs<2,T> operator-- (T) { return gen(); }
  template<int n, typename T> hs<n+1,T> operator~ (hs<n,T>) { return gen(); }
  template<int n, typename T> hs<n+1,T> operator- (hs<n,T>) { return gen(); }
  template<int n, typename T> hs<n+2,T> operator-- (hs<n,T>) { return gen(); }

  template<typename T> vs<1,T> operator! (T) { return gen(); }
  template<int n, typename T> vs<n+1,T> operator! (vs<n,T>) { return gen(); }

  // o--
  template<int n> os<n,hs<2> > operator-- (os<n>, int) { return gen(); }
  template<int n, int t> os<n,hs<t+2> > operator-- (os<n,hs<t> >, int) { return gen(); }

  // o-----o
  hline<0> operator* (os<1>) { return gen(); }
  template<int x> hline<x> operator* (hs<x,os<1> >) { return gen(); }
  hline<1> operator- (os<1>, os<1>) { return gen(); }
  template<int q> hline<q+1> operator- (os<1>, hs<q,os<1> >) { return gen(); }
  template<int p, int q> hline<p+q+1> operator- (os<1,hs<p> >, hs<q,os<1> >) { return gen(); }
  template<int p> hline<p+1> operator- (os<1,hs<p> >, os<1>) { return gen(); }

  template<int n, int q> os<1,hline<q> > operator* (os<1>, hs<q,os<1> >) { return gen(); }
  template<int p, int q> hs<p,os<1,hline<q> > > operator* (hs<p,os<1> >, hs<q,os<1> >) { return gen(); }
  template<int p, int q> vs<p,os<1,hline<q> > > operator* (vs<p,os<1> >, hs<q,os<1> >) { return gen(); }

  template<int y> vs<y,hline<1> > operator- (vs<y,os<1> >, os<1>) { return gen(); }
  template<int y, int p> vs<y,hline<p+1> > operator- (vs<y,os<1,hs<p> > >, os<1>) { return gen(); }
  template<int y, int p, int q> vs<y,hline<p+q+1> > operator- (vs<y,os<1,hs<p> > >, hs<q,os<1> >) { return gen(); }
  template<int y, int q> vs<y,hline<q+1> > operator- (vs<y,os<1> >, hs<q,os<1> >) { return gen(); }

  // o|!!!o
  template<int y> vline<y> operator* (vs<y,os<1> >) { return gen(); }
  vline<1> operator| (os<1>, os<1>) { return gen(); }
  template<int y> vline<y+1> operator| (os<1>, vs<y,os<1> >) { return gen(); }

  template<int x, int y> os<1,hs<x,vline<y> > > operator* (os<1,hs<x> >, vs<y,os<1> >) { return gen(); }

// Misc 2-D constructions
  template<typename T> os<1,hs<1,T> > operator- (os<1>, T) { return gen(); }
  template<int q, typename T> os<1,hs<q+1,T> > operator- (os<1>, hs<q,T>) { return gen(); }
  template<int p, typename T> os<1,hs<p+1,T> > operator- (os<1,hs<p> >, T) { return gen(); }
  template<int p, int q, typename T> os<1,hs<p+q+1,T> > operator- (os<1,hs<p> >, hs<q,T>) { return gen(); }
  template<int x> os<1,hline<x> > operator* (os<1>, hs<x,os<1> >) { return gen(); }
  template<int x, int q, typename T> os<1,hs<x,os<q+1,T> > > operator* (os<1,hs<x> >, os<q,T>) { return gen(); }

  // Constructing a rectangle: o---o|!!!o---o
  rectangle<0,0> operator* (os<1,hline<0> >) { return gen(); }
  template<int x> rectangle<x,0> operator* (hs<x,os<1,hline<x> > >) { return gen(); }
  template<int y> rectangle<0,y/2> operator* (os<1,vs<y,hline<0> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (hs<x,os<1,vs<y,hline<x> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<1,0> operator- (os<1>, os<1,hline<1> >) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1>, os<1,vs<y,hline<1> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x> rectangle<x,0> operator- (os<1>, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int q, int y> rectangle<q+1,y/2> operator- (os<1>, hs<q,os<1,vs<y,hline<q+1> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p, int q> rectangle<p+q+1,0> operator- (os<1,hs<p> >, hs<q,os<1,hline<p+q+1> > >) { return gen(); }
  template<int p, int q, int y> rectangle<p+q+1,y/2> operator- (os<1,hs<p> >, hs<q,os<1,vs<y,hline<p+q+1> > > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p> rectangle<p+1,0> operator- (os<1,hs<p> >, os<1,hline<p+1> >) { return gen(); }
  template<int p, int y> rectangle<p+1,y/2> operator- (os<1,hs<p> >, os<1,vs<y,hline<p+1> > >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<0,0> operator* (os<1>, hline<0>) { return gen(); }
  template<int x> rectangle<x,0> operator* (os<1,hs<x> >, hline<x>) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (os<1,hs<x> >, vs<y,hline<x> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int y> rectangle<0,y/2> operator* (os<1>, vs<y,hline<0> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  template<int x, int y> rectangle<x,y/2+1> operator| (hline<x>, vs<y,hline<x> >)
  { analog_literals_assert(y % 2 == 1); return gen(); }
  rectangle<0,1> operator|| (hline<0>, hline<0>) { return gen(); }
  template<int y> rectangle<0,y/2+1> operator|| (hline<0>, vs<y,hline<0> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<0,0> operator* (hline<0>, os<1>) { return gen(); }
  template<int x> rectangle<x,0> operator* (hline<x>, hs<x,os<1> >) { return gen(); }

  rectangle<1,0> operator- (os<1,hs<1,hline<0> > >, os<1>) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1,hs<1,vline<y> > >, os<1>)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p, int q> rectangle<p+q+1,0> operator- (os<1,hs<p+q+1,os<2,hs<p> > > >, hs<q,os<1> >) { return gen(); }
  template<int p, int q, int y> rectangle<p+q+1,y/2> operator- (os<1,hs<p+q+1,os<1,vs<y,os<1,hs<p> > > > > >, hs<q,os<1> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int q> rectangle<q+1,0> operator- (os<1,hs<q+1,hline<0> > >, hs<q,os<1> >) { return gen(); }
  template<int q, int y> rectangle<q+1,y/2> operator- (os<1,hs<q+1,vline<y> > >, hs<q,os<1> >)
  { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int p> rectangle<p+1,0> operator- (os<1,hs<p+1,os<2,hs<p> > > >, os<1>)
  { return gen(); }
  template<int p, int y> rectangle<p+1,y/2> operator- (os<1,hs<p+1,os<1,vs<y,os<1,hs<p> > > > > >, os<1>)
  { return gen(); }

// Constructions for 3-D literals begin here.

  // Constructions for vLs
  template<typename T> vLs<1,T> operator! (Ls<1,T>) { return gen(); }
  template<int n, typename T> vLs<n+1,T> operator! (Ls<1,vLs<n,T> >) { return gen(); }

  // Constructions for Lvvs
  template<typename T> vs<2,Ls<1,T> > operator! (vLs<1,T>) { return gen(); }
  template<typename T> Lvvs<1,T> operator& (vs<2,T>) { return gen(); }
  template<int n, typename T> Lvvs<n+1,T> operator& (vs<2,Lvvs<n,T> >) { return gen(); }
  template<typename T> Lvvs<1,Ls<1,T> > operator| (Ls<1>, vLs<1,T>) { return gen(); }
  template<int p, typename T> Lvvs<p+1,Ls<1,T> > operator| (Lvvs<p,Ls<1> >, vLs<1,T>) { return gen(); }
  template<typename T> Lvvs<1,T> operator| (Ls<1>, vs<1,T>) { return gen(); }
  template<int p, typename T> Lvvs<p+1,T> operator| (Lvvs<p,Ls<1> >, vs<1,T>) { return gen(); }
  template<int q, typename T> Lvvs<q+1,T> operator| (Ls<1>, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int p, int q, typename T> Lvvs<p+q+1,T> operator| (Lvvs<p,Ls<1> >, vs<1,Lvvs<q,T> >) { return gen(); }

// Misc 3-D constructions

  // Rear-top-left corner

  // Rear-top-right corner
  os<1,rectangle<0,0> > operator* (os<1>, os<1,hline<0> >) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,T> > > operator* (os<1,hs<x> >, T) { return gen(); }
  template<int x> os<1,hs<x,hline<0> > > operator* (os<1,hs<x> >, os<1>) { return gen(); }

  template<typename T> os<2,vLs<1,T> > operator| (hline<0>, Ls<1,T>) { return gen(); }
  template<int q, typename T> os<2,vLs<q+1,T> > operator| (hline<0>, Ls<1,vLs<q,T> >) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vLs<1,T> > > > operator| (hline<x>, Ls<1,T>) { return gen(); }
  template<int x, int q, typename T> os<1,hs<x,os<1,vLs<q+1,T> > > > operator| (hline<x>, Ls<1,vLs<q,T> >) { return gen(); }
  template<int p, int q, typename T> os<2,vLs<p+q+1,T> > operator| (os<2,vLs<p> >, Ls<1,vLs<q,T> >) { return gen(); }
  template<int x, int p, int q, typename T> os<1,hs<x,os<1,vLs<p+q+1,T> > > > operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,vLs<q,T> >) { return gen(); }
  template<int p, typename T> os<2,vLs<p+1,T> > operator| (os<2,vLs<p> >, Ls<1,T>) { return gen(); }
  template<int x, int p, typename T> os<1,hs<x,os<1,vLs<p+1,T> > > > operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,T>) { return gen(); }

  // Rear-bottom-left corner
  template<int n> hs<n,os<1,hline<0> > > operator* (hs<n,os<1> >, os<1>) { return gen(); }
  Ls<1,hline<0> > operator* (Ls<1>, os<1>) { return gen(); }
  os<1,rectangle<0,0> > operator* (hline<0>, hline<0>) { return gen(); }
  template<typename T> Ls<1,os<2,T> > operator* (Ls<1>, os<1,T>) { return gen(); }
  template<int n> Ls<1,vLs<n,hline<0> > > operator* (Ls<1,vLs<n> >, os<1>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<2,T> > > operator* (Ls<1,vLs<n> >, os<1,T>) { return gen(); }
  template<int x> Ls<1,os<1,hline<x> > > operator* (Ls<1>, hline<x>) { return gen(); }
  template<typename T> Ls<1,os<3,T> > operator* (Ls<1>, os<2,T>) { return gen(); }
  template<int n, int z> os<n,vLs<z,hline<0> > > operator* (os<n,vLs<z> >, os<1>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,os<2,T> > > operator* (os<2,vLs<z> >, os<1,T>) { return gen(); }
  template<int x> os<1,hs<x,os<1,hline<0> > > > operator* (hline<x>, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,T> > > operator* (hline<x>, os<1,T>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,hline<0> > > > > operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,T> > > > > operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1,T>) { return gen(); }
  template<int z> os<2,vLs<z,os<1,hline<0> > > > operator* (os<2,vLs<z> >, hline<0>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator* (os<1,hs<x,os<1,vLs<z> > > >, hline<x>) { return gen(); }
  template<int x> os<2,hline<x> > operator* (os<1>, hline<x>) { return gen(); }
  template<int n, int x> hs<n,os<2,hline<x> > > operator* (hs<n,os<1> >, hline<x>) { return gen(); }
  template<int n, int x> Ls<1,vLs<n,hline<x> > > operator* (Ls<1,vLs<n> >, hs<x,os<1> >) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<1,T> > > operator* (Ls<1,vLs<n> >, T) { return gen(); }
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,os<1> >, rectangle<x,0>) { return gen(); }
  template<int n, int z> hs<n,os<1,vLs<z,hline<0> > > > operator* (hs<n,os<1,vLs<z> > >, os<1>) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<2,T> > > > operator* (hs<n,os<1,vLs<z> > >, os<1,T>) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<1,T> > > > operator* (hs<n,os<1,vLs<z> > >, T) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<2,T> > > operator* (hline<x>, T) { return gen(); }
  template<int n, typename T> hs<n,os<3,T> > operator* (hs<n,os<1> >, os<1,T>) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<1,T> > > operator* (os<n,vLs<z> >, T) { return gen(); }
  template<int n, int z, int q, typename T> os<n,vLs<z,os<q+1,T> > > operator* (os<n,vLs<z> >, os<q,T>) { return gen(); }
  template<int n, typename T> vLs<n,os<3,T> > operator* (vLs<n>, os<2,T>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,T> > > operator* (Ls<1,vLs<n> >, os<2,T>) { return gen(); }

  // Near-top-left corner
  os<1,rectangle<0,0> > operator* (os<1,hline<0> >, os<1>) { return gen(); }
  template<int n> os<2,vLs<n,hline<0> > > operator* (os<2,vLs<n> >, os<1>) { return gen(); }
  template<int n, int x> Ls<n,os<1,hline<x> > > operator* (Ls<n,os<1> >, hs<x,os<1> >) { return gen(); }
  template<int n, typename T> Ls<n,os<2,T> > operator* (Ls<n,os<1> >, T) { return gen(); }
  template<int n> Ls<1,vLs<n,os<1,hline<0> > > > operator* (Ls<1,vLs<n,os<1> > >, os<1>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,T> > > operator* (Ls<1,vLs<n,os<1> > >, os<1,T>) { return gen(); }
  template<int n, int x> Ls<1,vLs<n,os<1,hline<x> > > > operator* (Ls<1,vLs<n,os<1> > >, hs<x,os<1> >) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<2,T> > > operator* (Ls<1,vLs<n,os<1> > >, T) { return gen(); }
  template<int x> os<1,hs<x,os<2,hline<x> > > > operator* (os<1,hs<x,hline<0> > >, hs<x,os<1> >) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,T> > > operator* (os<1,hs<x,hline<0> > >, T) { return gen(); }
  template<int x, int z> os<1,vLs<z,os<1,hline<x> > > > operator* (os<1,vLs<z,os<1> > >, hs<x,os<1> >) { return gen(); }
  template<int n, int x> hs<n,os<2,hline<x> > > operator* (hs<n,hline<0> >, hs<x,os<1> >) { return gen(); }
  template<int n, int m, int x> hs<n,os<1,vLs<m,os<1,hline<x> > > > > operator* (hs<n,os<1,vLs<m,os<1> > > >, hs<x,os<1> >) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, hs<x,os<1> >) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,T> > > > > operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, T) { return gen(); }
  template<typename T> os<3,T> operator* (hline<0>, T) { return gen(); }
  template<int x> os<2,hline<x> > operator* (hline<0>, hs<x,os<1> >) { return gen(); }
  template<int x> os<2,rectangle<x,0> > operator* (hline<0>, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<2,T> > > > operator* (hs<n,os<1,vLs<z,os<1> > > >, T) { return gen(); }
  Ls<1,os<1,hline<0> > > operator* (Ls<1,os<1> >, os<1>) { return gen(); }
  template<typename T> Ls<1,os<3,T> > operator* (Ls<1,os<1> >, os<1,T>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, hs<x,os<1> >) { return gen(); }
  template<int n, int z, int p, typename T> os<n,vLs<z,os<p+1,T> > > operator* (os<n,vLs<z,os<p> > >, T) { return gen(); }
  template<int n, int x> hs<n,os<3,hs<x,hline<0> > > > operator* (hs<n,hline<0> >, hs<x,hline<0> >) { return gen(); }
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,hline<0> >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int n, typename T> hs<n,os<3,T> > operator* (hs<n,hline<0> >, T) { return gen(); }
  template<int n> vLs<n,os<1,hline<0> > > operator* (vLs<n,os<1> >, os<1>) { return gen(); }
  template<int n, typename T> vLs<n,os<3,T> > operator* (vLs<n,os<1> >, os<1,T>) { return gen(); }
  template<int n, int z> os<n,vLs<z,os<1,hline<0> > > > operator* (os<n,vLs<z,os<1> > >, os<1>) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<3,T> > > operator* (os<n,vLs<z,os<1> > >, os<1,T>) { return gen(); }

  Ls<1,os<1,hline<1> > > operator- (Ls<1,hline<0> >, os<1>) { return gen(); }
  template<int n> vLs<n,os<1,hline<1> > > operator- (vLs<n,hline<0> >, os<1>) { return gen(); }
  template<int n> Ls<1,vLs<n,os<1,hline<1> > > > operator- (Ls<1,vLs<n,hline<0> > >, os<1>) { return gen(); }
  os<2,hs<2> > operator-- (hline<0>, int) { return gen(); }
  template<typename T> os<1,hs<1,os<3,hs<1,T> > > > operator- (os<1,hs<1,os<1,hline<0> > > >, T) { return gen(); }
  template<int z> os<1,hs<1,os<1,vLs<z,os<1,hline<1> > > > > > operator- (os<1,hs<1,os<1,vLs<z,hline<0> > > > >, os<1>) { return gen(); }
  template<int z, typename T> os<1,hs<1,os<1,vLs<z,os<2,hs<1,os<1,T> > > > > > > operator- (os<1,hs<1,os<1,vLs<z,hline<0> > > > >, os<1,T>) { return gen(); }
  os<1,hs<1,os<1,vLs<1,os<2,hs<1,os<1,Ls<1> > > > > > > > operator- (os<1,hs<1,os<1,vLs<1,hline<0> > > > >, os<1,Ls<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<2,hline<x> > > > operator- (os<1,hs<x,os<1,hline<0> > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,hs<x,T> > > > operator- (os<1,hs<x,os<1,hline<0> > > >, hs<x-1,T>) { return gen(); }
  template<int q> Ls<1,os<1,hline<q+1> > > operator- (Ls<1,hline<0> >, hs<q,os<1> >) { return gen(); }
  template<int p> Ls<1,os<1,hline<p+1> > > operator- (Ls<1,os<2,hs<p> > >, os<1>) { return gen(); }
  template<int p, int q> Ls<1,os<1,hline<p+q+1> > > operator- (Ls<1,os<2,hs<p> > >, hs<q,os<1> >) { return gen(); }
  template<typename T> Ls<1,os<2,hs<1,T> > > operator- (Ls<1,hline<0> >, T) { return gen(); }
  template<int q, typename T> Ls<1,os<2,hs<q+1,T> > > operator- (Ls<1,hline<0> >, hs<q,T>) { return gen(); }
  template<int p, typename T> Ls<1,os<2,hs<p+1,T> > > operator- (Ls<1,os<2,hs<p> > >, T) { return gen(); }
  template<int p, int q, typename T> Ls<1,os<2,hs<p+q+1,T> > > operator- (Ls<1,os<2,hs<p> > >, hs<q,T>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<2,hs<1,T> > > > operator- (Ls<1,vLs<n,hline<0> > >, T) { return gen(); }
  template<int n, int p, typename T> Ls<1,vLs<n,os<2,hs<p+1,T> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, T) { return gen(); }
  template<int n, int q, typename T> Ls<1,vLs<n,os<2,hs<q+1,T> > > > operator- (Ls<1,vLs<n,hline<0> > >, hs<q,T>) { return gen(); }
  template<int n, int p, int q, typename T> Ls<1,vLs<n,os<2,hs<p+q+1,T> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, hs<q,T>) { return gen(); }
  template<int x, int p> os<1,hs<x,os<2,hline<x> > > > operator- (os<1,hs<x,os<3,hs<p> > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<2,hline<x> > > > operator- (os<1,hs<x,os<3,hs<x-1> > > >, os<1>) { return gen(); }
  template<int x, int p, typename T> os<1,hs<x,os<3,hs<x,T> > > > operator- (os<1,hs<x,os<3,hs<p> > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,hs<x,T> > > > operator- (os<1,hs<x,os<3,hs<x-1> > > >, T) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,hline<0> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,hline<0> > > > >, hs<x-1,T>) { return gen(); }
  template<int x, int z, int p, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, T) { return gen(); }
  template<int n, int q> vLs<n,os<1,hline<q+1> > > operator- (vLs<n,hline<0> >, hs<q,os<1> >) { return gen(); }
  template<int n, int p, int q> vLs<n,os<1,hline<p+q+1> > > operator- (vLs<n,os<2,hs<p> > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p> vLs<n,os<1,hline<p+1> > > operator- (vLs<n,os<2,hs<p> > >, os<1>) { return gen(); }
  template<int n, int q> Ls<1,vLs<n,os<1,hline<q+1> > > > operator- (Ls<1,vLs<n,hline<0> > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p, int q> Ls<1,vLs<n,os<1,hline<p+q+1> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p> Ls<1,vLs<n,os<1,hline<p+1> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, os<1>) { return gen(); }

  // Near-top-right corner
  template<typename T> Ls<1,os<3,T> > operator* (Ls<1,hline<0> >, T) { return gen(); }
  template<int x, typename T> Ls<1,os<2,hs<x,os<1,T> > > > operator* (Ls<1,os<2,hs<x> > >, T) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,os<2,hs<x,os<1,T> > > > > operator* (Ls<1,vLs<n,os<2,hs<x> > > >, T) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,T> > > operator* (Ls<1,vLs<n,hline<0> > >, T) { return gen(); }
  template<int x> os<1,hs<x,os<3,hs<x,hline<0> > > > > operator* (os<1,hs<x,os<3,hs<x> > > >, os<1>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,os<3,T> > > operator* (os<2,vLs<z,hline<0> > >, T) { return gen(); }
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,os<3,hs<x> > >, hline<x>) { return gen(); }
  template<int n, int x, typename T> hs<n,os<3,hs<x,os<1,T> > > > operator* (hs<n,os<3,hs<x> > >, T) { return gen(); }
  template<int n, int x, int z, typename T> hs<n,os<1,vLs<z,os<2,hs<x,os<1,T> > > > > > operator* (hs<n,os<1,vLs<z,os<2,hs<x> > > > >, T) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,T> > > > > > > operator* (os<1,hs<x,os<1,vLs<z,os<2,hs<x> > > > > >, T) { return gen(); }
  template<int n, int x, typename T> hs<n,os<2,hline<x> > > operator* (hs<n,hline<0> >, hs<x,os<1> >) { return gen(); }
  template<int n, int x> hs<n,os<3,hs<x,hline<0> > > > operator* (hs<n,os<3,hs<x> > >, os<1>) { return gen(); }

  template<typename T> Ls<1,os<3,Lvvs<1,T> > > operator& (Ls<1,os<1,hline<0> > >, vs<2,T>) { return gen(); }
  template<int q, typename T> Ls<1,os<3,Lvvs<q+1,T> > > operator& (Ls<1,os<1,hline<0> > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,Lvvs<1,T> > > >  operator& (Ls<1,vLs<n,os<1,hline<0> > > >, vs<2,T>) { return gen(); }
  template<int n, int q, typename T> Ls<1,vLs<n,os<3,Lvvs<q+1,T> > > >  operator& (Ls<1,vLs<n,os<1,hline<0> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, int q, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<q+1,T> > > > > > > > operator& (os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<1,T> > > > > > > > operator& (os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > >, vs<2,T>) { return gen(); }
  template<int x, typename T> Ls<1,os<2,hs<x,os<1,Lvvs<1,T> > > > > operator& (Ls<1,os<1,hline<x> > >, vs<2,T>) { return gen(); }
  template<int x, int q, typename T> Ls<1,os<2,hs<x,os<1,Lvvs<q+1,T> > > > > operator& (Ls<1,os<1,hline<x> > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<1,T> > > > > > operator& (Ls<1,vLs<n,os<1,hline<x> > > >, vs<2,T>) { return gen(); }
  template<int n, int x, int q, typename T> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<q+1,T> > > > > > operator& (Ls<1,vLs<n,os<1,hline<x> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int q, typename T> Ls<1,os<3,hs<x,os<1,Lvvs<q+1,T> > > > > operator& (Ls<1,os<2,hline<x> > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, typename T> Ls<1,os<3,hs<x,os<1,Lvvs<1,T> > > > > operator& (Ls<1,os<2,hline<x> > >, vs<2,T>) { return gen(); }
  template<int n, int q, int z, typename T> os<n,vLs<z,os<3,Lvvs<q+1,T> > > > operator& (os<n,vLs<z,os<1,hline<0> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<3,Lvvs<1,T> > > > operator& (os<n,vLs<z,os<1,hline<0> > > >, vs<2,T>) { return gen(); }
  template<int n, int p, int q, int z, typename T> os<n,vLs<z,os<3,Lvvs<p+q+1,T> > > > operator& (os<n,vLs<z,os<3,Lvvs<p> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int p, int z, typename T> os<n,vLs<z,os<3,Lvvs<p+1,T> > > > operator& (os<n,vLs<z,os<3,Lvvs<p> > > >, vs<2,T>) { return gen(); }

  template<int z, typename T> os<2,vLs<z,os<3,Lvvs<1,Ls<1,T> > > > > operator| (os<2,vLs<z,os<3,Ls<1> > > >, vLs<1,T>) { return gen(); }
  template<int x, int n, typename T> os<1,hs<x,os<1,vLs<n,os<2,hs<x,os<1,Lvvs<1,Ls<1,T> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<n,os<2,hs<x,os<1,Ls<1> > > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p+1,Ls<1> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vLs<1>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,os<3,Lvvs<1,T> > > > operator| (os<2,vLs<z,os<3,Ls<1> > > >, vs<1,T>) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,os<3,Lvvs<q+1,T> > > > operator| (os<2,vLs<z,os<3,Ls<1> > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int p, typename T> os<2,vLs<z,os<3,Lvvs<p+1,T> > > > operator| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,T>) { return gen(); }
  template<int z, int p, int q, typename T> os<2,vLs<z,os<3,Lvvs<p+q+1,T> > > > operator| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,T>) { return gen(); }
  template<int x, int z, int q, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<q+1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, int p, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p+1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vs<1,T>) { return gen(); }
  template<int x, int z, int p, int q, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p+q+1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int n, int z, int p, typename T> os<n,vLs<z,os<3,Lvvs<p+1,T> > > > operator| (os<n,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,T>) { return gen(); }
  template<int n, int z, int p, typename T> os<n,vLs<z,os<3,Lvvs<p+1,Ls<1,T> > > > > operator| (os<n,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vLs<1,T>) { return gen(); }

  template<int z, typename T> os<2,vLs<z,os<3,Lvvs<1,T> > > > operator|| (os<2,vLs<z,os<3,Ls<1> > > >, T) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,os<3,Lvvs<q+1,T> > > > operator|| (os<2,vLs<z,os<3,Ls<1> > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int p, typename T> os<2,vLs<z,os<3,Lvvs<p+1,T> > > > operator|| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, T) { return gen(); }
  template<int z, int p, int q, typename T> os<2,vLs<z,os<3,Lvvs<p+q+1,T> > > > operator|| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, Lvvs<q,T>) { return gen(); }

  // Near-bottom-left corner
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,os<2,hline<x> > >, hs<x,os<1> >) { return gen(); }
  template<int n, int x> os<n,rectangle<x,0> > operator* (os<n,hline<x> >, hs<x,os<1> >) { return gen(); }

  template<int n, int m> vs<n,Lvvs<m,hline<1> > > operator- (vs<n,Lvvs<m,os<1> > >, os<1>) { return gen(); }
  template<int n, int m, int q> vs<n,Lvvs<m,hline<q+1> > > operator- (vs<n,Lvvs<m,os<1> > >, hs<q,os<1> >) { return gen(); }
  template<int n, int m, int p, int q> vs<n,Lvvs<m,hline<p+q+1> > > operator- (vs<n,Lvvs<m,os<1,hs<p> > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int m, int p> vs<n,Lvvs<m,hline<p+1> > > operator- (vs<n,Lvvs<m,os<1,hs<p> > > >, os<1>) { return gen(); }
  template<int n, int z> os<n,hs<1,os<1,Lvvs<z,hline<1> > > > > operator- (os<2,hs<1,os<1,Lvvs<z,os<1> > > > >, os<1>) { return gen(); }
  template<int n, int z> vLs<n,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > operator- (vLs<n,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > >, os<1>) { return gen(); }
  template<int z> Ls<1,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > operator- (Ls<1,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > >, os<1>) { return gen(); }
  template<int n, int z> Ls<1,vLs<n,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z> os<n,hs<x,os<1,Lvvs<z,hline<x> > > > > operator- (os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int p> os<n,hs<x,os<1,Lvvs<z,hline<x> > > > > operator- (os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z> os<n,hs<x,os<1,Lvvs<z,hline<x> > > > > operator- (os<2,hs<x,os<1,Lvvs<z,os<1> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int z> vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int p> vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z> vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<int x, int z, int p> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int p> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > > >, hs<x-1,os<1> >) { return gen(); }


// Constructing a cuboid with H == D: o---o|&!&|&*o---o&!!&!!&!!o---o
  cuboid<0,0,0> operator* (os<2,rectangle<0,0> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (hs<x,os<2,rectangle<x,0> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<1,vLs<z,os<3,Lvvs<z,hline<0> > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > >) { return gen(); }

  cuboid<1,0,0> operator- (os<1>, os<2,rectangle<1,0> >) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1>, os<1,vLs<z,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1>, hs<x-1,os<2,rectangle<x,0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1>, hs<x-1,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > >) { return gen(); }
  template<int x, int p> cuboid<x,0,0> operator- (os<1,hs<p> >, hs<x-p-1,os<2,rectangle<x,0> > >) { return gen(); }
  template<int x, int p, int z> cuboid<x,z,z> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x-1> >, os<2,rectangle<x,0> >) { return gen(); }
  template<int p, int z> cuboid<p+1,z,z> operator- (os<1,hs<p> >, os<1,vLs<z,os<2,hs<p+1,os<1,Lvvs<z,hline<p+1> > > > > > >) { return gen(); }

  // Rear-top-right corner
  cuboid<0,0,0> operator* (os<1>, os<1,rectangle<0,0> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<1>, vLs<z,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x> >, os<1,rectangle<x,0> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x> >, vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > >) { return gen(); }

  cuboid<0,1,1> operator| (hline<0>, Ls<1,os<3,Lvvs<1,hline<0> > > >) { return gen(); }
  template<int x> cuboid<x,1,1> operator| (hline<x>, Ls<1,os<2,hs<x,os<1,Lvvs<1,hline<x> > > > > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator| (hline<0>, Ls<1,vLs<z-1,os<3,Lvvs<z,hline<0> > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (hline<x>, Ls<1,vLs<z-1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator| (os<2,vLs<p> >, Ls<1,vLs<z-p-1,os<3,Lvvs<z,hline<0> > > > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,vLs<z-p-1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator| (os<2,vLs<z-1> >, Ls<1,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z-1> > > >, Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > >) { return gen(); }

  template<int z> cuboid<0,z,z> operator& (os<2,vs<1> >, vLs<z-1,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vs<1> > > >, vLs<z-1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator& (os<2,vLs<p,vs<1> > >, vLs<z-p-1,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<p,vs<1> > > > >, vLs<z-p-1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator& (os<2,vLs<z-1,vs<1> > >, os<3,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z-1,vs<1> > > > >, os<2,hs<x,os<1,Lvvs<z,hline<x> > > > >) { return gen(); }

  // Rear-bottom-left corner
  cuboid<0,0,0> operator* (hline<0>, rectangle<0,0>) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z> >, os<2,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (hline<x>, rectangle<x,0>) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1,hs<x,os<1,Lvvs<z,hline<x> > > > >) { return gen(); }

  // Near-top-left corner
  cuboid<0,0,0> operator* (os<1,hline<0> >, os<1,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,os<1> > >, os<1,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,hline<0> > >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, hs<x,os<1,Lvvs<z,hline<x> > > >) { return gen(); }

  cuboid<1,0,0> operator- (os<1,hs<1,os<1,hline<0> > > >, os<1,hline<1> >) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1,hs<1,os<1,vLs<z,hline<0> > > > >, os<1,Lvvs<z,hline<1> > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<1,hline<0> > > >, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,hline<0> > > > >, hs<x-1,os<1,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int x, int p> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<p> > > >, hs<x-p-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,os<1,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x-1> > > >, os<1,hline<x> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, os<1,Lvvs<z,hline<x> > >) { return gen(); }

  // Near-top-right corner
  cuboid<0,0,0> operator* (rectangle<0,0>, hline<0>) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,hline<0> > >, Lvvs<z,hline<0> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<3,hs<x> > > >, hline<x>) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z,os<2,hs<x> > > > > >, Lvvs<z,hline<x> >) { return gen(); }

  cuboid<0,1,1> operator& (os<2,vLs<1,os<1,hline<0> > > >, vs<2,hline<0> >) { return gen(); }
  template<int x> cuboid<x,1,1> operator& (os<1,hs<x,os<1,vLs<1,os<1,hline<x> > > > > >, vs<2,hline<x> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator& (os<2,vLs<z,os<1,hline<0> > > >, vs<2,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > >, vs<2,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator& (os<2,vLs<z,os<3,Lvvs<p> > > >, vs<2,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p> > > > > > > >, vs<2,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator& (os<2,vLs<z,os<3,Lvvs<z-1> > > >, vs<2,hline<0> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z-1> > > > > > > >, vs<2,hline<x> >) { return gen(); }

  cuboid<0,1,1> operator| (os<2,vLs<1,os<3,Ls<1> > > >, vs<1,hline<0> >) { return gen(); }
  template<int x> cuboid<x,1,1> operator| (os<1,hs<x,os<1,vLs<1,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Ls<1> > > >, vs<1,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vs<1,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Lvvs<z-1,Ls<1> > > > >, vs<1,hline<0> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z-1,Ls<1> > > > > > > > >, vs<1,hline<x> >) { return gen(); }

  cuboid<0,1,1> operator|| (os<2,vLs<1,os<3,Ls<1> > > >, hline<0>) { return gen(); }
  template<int z> cuboid<0,z,z> operator|| (os<2,vLs<z,os<3,Ls<1> > > >, Lvvs<z-1,hline<0> >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator|| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, Lvvs<z-p-1,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator|| (os<2,vLs<z,os<3,Lvvs<z-1,Ls<1> > > > >, hline<0>) { return gen(); }

  // Near-bottom-left corner
  cuboid<0,0,0> operator* (os<1,rectangle<0,0> >, os<1>) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,os<3,Lvvs<z> > > >, os<1>) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<2,hline<x> > > >, hs<x,os<1> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z> > > > > > > >, hs<x,os<1> >) { return gen(); }

  cuboid<1,0,0> operator- (os<1,hs<1,os<3,hs<1,hline<0> > > > >, os<1>) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1,hs<1,os<1,vLs<z,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > > > > >, os<1>) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,hline<0> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int p> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,os<2,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,os<2,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > > > > >, os<1>) { return gen(); }

// Constructing a cuboid with H < D: o---o|&o&&&&o---o&!!o---o
// Constructing a cuboid with H > D: o---o|&!&!o---o|!!!!!o|!&!!&!!o---o

#if 0
  o---o
  |L   \
  | L   \
  o  o---o
   L |   |
    L|   |
     o---o
#endif


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