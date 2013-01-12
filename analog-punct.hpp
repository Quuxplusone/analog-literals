
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
  static os<1,Ls<1> > oL;
  static Ls<1,os<1> > Lo;
  static Ls<1,hline<0> > Loo;

  template<typename T> os<1,T> operator* (T) { return gen(); }
  template<int n, typename T> os<n+1,T> operator* (os<n,T>) { return gen(); }
  os<1,hline<0> > operator* (os<1>, os<1>) { return gen(); }
  template<int n, typename T> os<n+2,T> operator* (os<1>, os<n,T>) { return gen(); }
  template<typename T> os<2,T> operator* (os<1>, T) { return gen(); }

  template<typename T> Ls<1,T> operator& (T) { return gen(); }
  template<int n, typename T> Ls<n+1,T> operator& (Ls<n,T>) { return gen(); }
  template<int p, typename T> Ls<p+1,T> operator& (Ls<p>, T) { return gen(); }
  template<int p, int q, typename T> Ls<p+q+1,T> operator& (Ls<p>, Ls<q,T>) { return gen(); }

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
  template<int n> Ls<1,os<n,hs<2> > > operator-- (Ls<1,os<n> >, int) { return gen(); }
  template<int n, int t> Ls<1,os<n,hs<t+2> > > operator-- (Ls<1,os<n,hs<t> > >, int) { return gen(); }

  // o-----o
  hline<0> operator* (os<1>) { return gen(); }
  template<int x> hline<x> operator* (hs<x,os<1> >) { return gen(); }
  hline<1> operator- (os<1>, os<1>) { return gen(); }
  template<int q> hline<q+1> operator- (os<1>, hs<q,os<1> >) { return gen(); }
  template<int p, int q> hline<p+q+1> operator- (os<1,hs<p> >, hs<q,os<1> >) { return gen(); }
  template<int p> hline<p+1> operator- (os<1,hs<p> >, os<1>) { return gen(); }

  template<int n, int x> hs<n,os<1,hline<x> > > operator* (hs<n,os<1> >, hs<x,os<1> >) { return gen(); }

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
  template<int y> rectangle<0,y/2> operator* (os<1,vs<y,hline<0> > >) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (hs<x,os<1,vs<y,hline<x> > > >) { return gen(); }

  rectangle<1,0> operator- (os<1>, os<1,hline<1> >) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1>, os<1,vs<y,hline<1> > >) { return gen(); }
  template<int x> rectangle<x,0> operator- (os<1>, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator- (os<1>, hs<x-1,os<1,vs<y,hline<x> > > >) { return gen(); }
  template<int x, int p> rectangle<x,0> operator- (os<1,hs<p> >, hs<x-p-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int y, int p> rectangle<x,y/2> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vs<y,hline<x> > > >) { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x> rectangle<x,0> operator- (os<1,hs<x-1> >, os<1,hline<x> >) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator- (os<1,hs<x-1> >, os<1,vs<y,hline<x> > >) { return gen(); }

  rectangle<0,0> operator* (os<1>, hline<0>) { return gen(); }
  template<int x> rectangle<x,0> operator* (os<1,hs<x> >, hline<x>) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator* (os<1,hs<x> >, vs<y,hline<x> >) { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int y> rectangle<0,y/2> operator* (os<1>, vs<y,hline<0> >) { analog_literals_assert(y % 2 == 0); return gen(); }

  template<int y> rectangle<0,y/2+1> operator| (hline<0>, vs<y,hline<0> >) { analog_literals_assert(y % 2 == 1); return gen(); }
  template<int x, int y> rectangle<x,y/2+1> operator| (hline<x>, vs<y,hline<x> >) { analog_literals_assert(y % 2 == 1); return gen(); }
  rectangle<0,1> operator|| (hline<0>, hline<0>) { return gen(); }
  template<int y> rectangle<0,y/2+1> operator|| (hline<0>, vs<y,hline<0> >) { analog_literals_assert(y % 2 == 0); return gen(); }

  rectangle<0,0> operator* (hline<0>, os<1>) { return gen(); }
  template<int x> rectangle<x,0> operator* (hline<x>, hs<x,os<1> >) { return gen(); }

  rectangle<1,0> operator- (os<1,hs<1,hline<0> > >, os<1>) { return gen(); }
  template<int y> rectangle<1,y/2> operator- (os<1,hs<1,vline<y> > >, os<1>) { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x, int p> rectangle<x,0> operator- (os<1,hs<x,os<2,hs<p> > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int p, int y> rectangle<x,y/2> operator- (os<1,hs<x,os<1,vs<y,os<1,hs<p> > > > > >, hs<x-p-1,os<1> >) { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x> rectangle<x,0> operator- (os<1,hs<x,hline<0> > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator- (os<1,hs<x,vline<y> > >, hs<x-1,os<1> >) { analog_literals_assert(y % 2 == 0); return gen(); }
  template<int x> rectangle<x,0> operator- (os<1,hs<x,os<2,hs<x-1> > > >, os<1>) { return gen(); }
  template<int x, int y> rectangle<x,y/2> operator- (os<1,hs<x,os<1,vs<y,os<1,hs<x-1> > > > > >, os<1>) { return gen(); }

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

// Misc 3-D constructions for H==D

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
  os<1,rectangle<0,0> > operator* (hline<0>, hline<0>) { return gen(); }
  template<int n> Ls<1,vLs<n,hline<0> > > operator* (Ls<1,vLs<n> >, os<1>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<2,T> > > operator* (Ls<1,vLs<n> >, os<1,T>) { return gen(); }
  template<typename T> Ls<1,os<3,T> > operator* (Ls<1>, os<2,T>) { return gen(); }
  template<int n, int z> os<n,vLs<z,hline<0> > > operator* (os<n,vLs<z> >, os<1>) { return gen(); }
  template<int x> os<1,hs<x,os<1,hline<0> > > > operator* (hline<x>, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,T> > > operator* (hline<x>, os<1,T>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,hline<0> > > > > operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,T> > > > > operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1,T>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<1,T> > > operator* (Ls<1,vLs<n> >, T) { return gen(); }
  template<int n, int z> hs<n,os<1,vLs<z,hline<0> > > > operator* (hs<n,os<1,vLs<z> > >, os<1>) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<2,T> > > > operator* (hs<n,os<1,vLs<z> > >, os<1,T>) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<1,T> > > > operator* (hs<n,os<1,vLs<z> > >, T) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<2,T> > > operator* (hline<x>, T) { return gen(); }
  template<int n, typename T> hs<n,os<3,T> > operator* (hs<n,os<1> >, os<1,T>) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<1,T> > > operator* (os<n,vLs<z> >, T) { return gen(); }
  template<int n, int z, int q, typename T> os<n,vLs<z,os<q+1,T> > > operator* (os<n,vLs<z> >, os<q,T>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,T> > > operator* (Ls<1,vLs<n> >, os<2,T>) { return gen(); }

  // Near-top-left corner
  os<1,rectangle<0,0> > operator* (os<1,hline<0> >, os<1>) { return gen(); }
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
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,T> > > > > operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, T) { return gen(); }
  template<typename T> os<3,T> operator* (hline<0>, T) { return gen(); }
  template<int x> os<2,hline<x> > operator* (hline<0>, hs<x,os<1> >) { return gen(); }
  template<int x> os<2,rectangle<x,0> > operator* (hline<0>, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int n, int z, typename T> hs<n,os<1,vLs<z,os<2,T> > > > operator* (hs<n,os<1,vLs<z,os<1> > > >, T) { return gen(); }
  Ls<1,os<1,hline<0> > > operator* (Ls<1,os<1> >, os<1>) { return gen(); }
  template<typename T> Ls<1,os<3,T> > operator* (Ls<1,os<1> >, os<1,T>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, hs<x,os<1> >) { return gen(); }
  template<int n, int z, int p, typename T> os<n,vLs<z,os<p+1,T> > > operator* (os<n,vLs<z,os<p> > >, T) { return gen(); }
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,hline<0> >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int n, typename T> hs<n,os<3,T> > operator* (hs<n,hline<0> >, T) { return gen(); }
  template<int n, int z> os<n,vLs<z,os<1,hline<0> > > > operator* (os<n,vLs<z,os<1> > >, os<1>) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<3,T> > > operator* (os<n,vLs<z,os<1> > >, os<1,T>) { return gen(); }

  Ls<1,os<1,hline<1> > > operator- (Ls<1,hline<0> >, os<1>) { return gen(); }
  template<int n> Ls<1,vLs<n,os<1,hline<1> > > > operator- (Ls<1,vLs<n,hline<0> > >, os<1>) { return gen(); }
  os<2,hs<2> > operator-- (hline<0>, int) { return gen(); }
  template<typename T> os<1,hs<1,os<3,hs<1,T> > > > operator- (os<1,hs<1,os<1,hline<0> > > >, T) { return gen(); }
  template<int z> os<1,hs<1,os<1,vLs<z,os<1,hline<1> > > > > > operator- (os<1,hs<1,os<1,vLs<z,hline<0> > > > >, os<1>) { return gen(); }
  template<int z, typename T> os<1,hs<1,os<1,vLs<z,os<2,hs<1,os<1,T> > > > > > > operator- (os<1,hs<1,os<1,vLs<z,hline<0> > > > >, os<1,T>) { return gen(); }
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
  template<int x, int p, typename T> os<1,hs<x,os<3,hs<x,T> > > > operator- (os<1,hs<x,os<3,hs<p> > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,hs<x,T> > > > operator- (os<1,hs<x,os<3,hs<x-1> > > >, T) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,hline<0> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,os<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,hline<0> > > > >, hs<x-1,T>) { return gen(); }
  template<int x, int z, int p, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, T) { return gen(); }
  template<int n, int q> Ls<1,vLs<n,os<1,hline<q+1> > > > operator- (Ls<1,vLs<n,hline<0> > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p, int q> Ls<1,vLs<n,os<1,hline<p+q+1> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p> Ls<1,vLs<n,os<1,hline<p+1> > > > operator- (Ls<1,vLs<n,os<2,hs<p> > > >, os<1>) { return gen(); }

  // Near-top-right corner
  template<int n, typename T> Ls<n,os<3,T> > operator* (Ls<n,hline<0> >, T) { return gen(); }
  template<int x, typename T> Ls<1,os<2,hs<x,os<1,T> > > > operator* (Ls<1,os<2,hs<x> > >, T) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,os<2,hs<x,os<1,T> > > > > operator* (Ls<1,vLs<n,os<2,hs<x> > > >, T) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,os<3,T> > > operator* (Ls<1,vLs<n,hline<0> > >, T) { return gen(); }
  template<int x> os<1,hs<x,os<3,hs<x,hline<0> > > > > operator* (os<1,hs<x,os<3,hs<x> > > >, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<3,hs<x,os<2,T> > > > > operator* (os<1,hs<x,os<3,hs<x> > > >, os<1,T>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,os<3,T> > > operator* (os<2,vLs<z,hline<0> > >, T) { return gen(); }
  template<int n, int x> hs<n,os<2,rectangle<x,0> > > operator* (hs<n,os<3,hs<x> > >, hline<x>) { return gen(); }
  template<int n, int x, typename T> hs<n,os<3,hs<x,os<2,T> > > > operator* (hs<n,os<3,hs<x> > >, os<1,T>) { return gen(); }
  template<int n, int x, int z, typename T> hs<n,os<1,vLs<z,os<2,hs<x,os<1,T> > > > > > operator* (hs<n,os<1,vLs<z,os<2,hs<x> > > > >, T) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,T> > > > > > > operator* (os<1,hs<x,os<1,vLs<z,os<2,hs<x> > > > > >, T) { return gen(); }
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
  template<int n, int q, int z, typename T> os<n,vLs<z,os<3,Lvvs<q+1,T> > > > operator& (os<n,vLs<z,os<1,hline<0> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int z, typename T> os<n,vLs<z,os<3,Lvvs<1,T> > > > operator& (os<n,vLs<z,os<1,hline<0> > > >, vs<2,T>) { return gen(); }

  template<int z, typename T> os<2,vLs<z,os<3,Lvvs<1,Ls<1,T> > > > > operator| (os<2,vLs<z,os<3,Ls<1> > > >, vLs<1,T>) { return gen(); }
  template<int x, int n, typename T> os<1,hs<x,os<1,vLs<n,os<2,hs<x,os<1,Lvvs<1,Ls<1,T> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<n,os<2,hs<x,os<1,Ls<1> > > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p+1,Ls<1> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vLs<1>) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,os<3,Lvvs<q+1,T> > > > operator| (os<2,vLs<z,os<3,Ls<1> > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int p, int q, typename T> os<2,vLs<z,os<3,Lvvs<p+q+1,T> > > > operator| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, int q, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<q+1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, int p, int q, typename T> os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p+q+1,T> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
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
  template<int z> Ls<1,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > operator- (Ls<1,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > >, os<1>) { return gen(); }
  template<int n, int z> Ls<1,vLs<n,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > > >, os<1>) { return gen(); }
  template<int x, int z> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<int x, int z, int p> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> Ls<1,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > operator- (Ls<1,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int p> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > operator- (Ls<1,vLs<n,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > > >, hs<x-1,os<1> >) { return gen(); }

// Constructing a cuboid with H == D: o---o|&!&|&*o---o&!!&!!&!!o---o

  // Rear-top-left corner
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
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<2,hline<x> > > >, hs<x,os<1> >) { return gen(); }

  cuboid<1,0,0> operator- (os<1,hs<1,os<3,hs<1,hline<0> > > > >, os<1>) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1,hs<1,os<1,vLs<z,os<2,hs<1,os<1,Lvvs<z,os<1> > > > > > > > >, os<1>) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,hline<0> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1> > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int p> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,os<2,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1,hs<p> > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x,os<2,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,os<1,hs<x-1> > > > > > > > > >, os<1>) { return gen(); }

// Misc 3-D constructions for H > D

  // Rear-top-left corner
  os<3,vs<2,hline<0> > > operator* (rectangle<0,1>) { return gen(); }
  os<2,vs<1,hline<0> > > operator* (os<1,vs<1,hline<0> > >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator* (hs<x,os<1,vs<1,hline<x> > > >) { return gen(); }

  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1>, hs<x-1,os<1,vs<1,hline<x> > > >) { return gen(); }
  template<int x, int p> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1,hs<p> >, hs<x-p-1,os<1,vs<1,hline<x> > > >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1,hs<x-1> >, os<1,vs<1,hline<x> > >) { return gen(); }

  // Rear-top-right corner
  os<2,vs<1,hline<0> > > operator* (os<1>, vs<1,hline<0> >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator* (os<1,hs<x> >, vs<1,hline<x> >) { return gen(); }

  // Single pipe to the left of the near-top-left corner
  template<int n, typename T> os<2,vLs<n,vs<1,T> > > operator| (os<2,vLs<n> >, T) { return gen(); }

  // Near-top-left corner

  template<int n> Ls<1,vLs<n,vs<1,hline<1> > > > operator- (Ls<1,vLs<n,vs<1,os<1> > > >, os<1>) { return gen(); }
  template<int n, int q> Ls<1,vLs<n,vs<1,hline<q+1> > > > operator- (Ls<1,vLs<n,vs<1,os<1> > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p, int q> Ls<1,vLs<n,vs<1,hline<p+q+1> > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<p> > > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p> Ls<1,vLs<n,vs<1,hline<p+1> > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<p> > > > >, os<1>) { return gen(); }
  os<1,hs<1,os<1,vs<1,hline<1> > > > > operator- (os<1,hs<1,vline<1> > >, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,vline<1> > >, hs<x-1,T>) { return gen(); }
  template<int n, int q, typename T> Ls<1,vLs<n,vs<1,os<1,hs<q+1,T> > > > > operator- (Ls<1,vLs<n,vs<1,os<1> > > >, hs<q,T>) { return gen(); }
  template<int n, int p, int q, typename T> Ls<1,vLs<n,vs<1,os<1,hs<p+q+1,T> > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<p> > > > >, hs<q,T>) { return gen(); }
  template<int n, int p, typename T> Ls<1,vLs<n,vs<1,os<1,hs<p+1,T> > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<p> > > > >, T) { return gen(); }
  template<int z> os<1,hs<1,os<1,vLs<z,vs<1,hline<1> > > > > > operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1> > > > > >, os<1>) { return gen(); }
  template<int z, typename T> os<1,hs<1,os<1,vLs<z,vs<1,os<1,hs<1,T> > > > > > > operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1> > > > > >, T) { return gen(); }
  os<1,hs<1,os<1,vs<1,hline<1> > > > > operator- (os<1>, os<1,vs<1,hline<1> > >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1,hs<x,vline<1> > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int p> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1,hs<x,os<1,vs<1,os<1,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,hline<x> > > > > operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,T> > > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1> > > > > >, hs<x-1,T>) { return gen(); }
  template<int x, int z, int p, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,T> > > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<p> > > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,T> > > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x-1> > > > > > >, T) { return gen(); }
  template<int q, typename T> Ls<1,vs<1,os<1,hs<q+1,T> > > > operator- (Ls<1,vs<1,os<1> > >, hs<q,T>) { return gen(); }
  template<int p, int q, typename T> Ls<1,vs<1,os<1,hs<p+q+1,T> > > > operator- (Ls<1,vs<1,os<1,hs<p> > > >, hs<q,T>) { return gen(); }
  template<int p, typename T> Ls<1,vs<1,os<1,hs<p+1,T> > > > operator- (Ls<1,vs<1,os<1,hs<p> > > >, T) { return gen(); }
  template<typename T> Ls<1,vs<1,os<1,hs<1,T> > > > operator- (Ls<1,vs<1,os<1> > >, T) { return gen(); }
  Ls<1,vs<1,hline<1> > > operator- (Ls<1,vs<1,os<1> > >, os<1>) { return gen(); }
  template<int q> Ls<1,vs<1,hline<q+1> > > operator- (Ls<1,vs<1,os<1> > >, hs<q,os<1> >) { return gen(); }
  template<int p, int q> Ls<1,vs<1,hline<p+q+1> > > operator- (Ls<1,vs<1,os<1,hs<p> > > >, hs<q,os<1> >) { return gen(); }
  template<int p> Ls<1,vs<1,hline<p+1> > > operator- (Ls<1,vs<1,os<1,hs<p> > > >, os<1>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,vs<1,os<1,hs<1,T> > > > > operator- (Ls<1,vLs<n,vs<1,os<1> > > >, T) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1> > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<p> > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > > operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<typename T> os<1,hs<1,os<1,vs<1,os<1,hs<1,T> > > > > > operator- (os<1,hs<1,vline<1> > >, T) { return gen(); }
  template<int x, int p, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vs<1,os<1,hs<p> > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x-1> > > > > >, T) { return gen(); }

  // Near-top-right corner
  template<int h> os<1,vline<h> > operator* (os<1>, vs<h,os<1> >) { return gen(); }
  template<int h> os<2,vs<1,os<1,vline<h> > > > operator* (os<1,vline<1> >, vs<h,os<1> >) { return gen(); }
  template<int x, int h> Ls<1,vs<1,os<1,hs<x,vline<h> > > > > operator* (Ls<1,vs<1,os<1,hs<x> > > >, vs<h,os<1> >) { return gen(); }
  template<int n, int x, int h> hs<n,os<1,vs<1,os<1,hs<x,vline<h> > > > > > operator* (hs<n,os<1,vs<1,os<1,hs<x> > > > >, vs<h,os<1> >) { return gen(); }
  template<int n, int x, int h> Ls<1,vLs<n,vs<1,os<1,hs<x,vline<h> > > > > > operator* (Ls<1,vLs<n,vs<1,os<1,hs<x> > > > >, vs<h,os<1> >) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,T> > > > > > operator* (Ls<1,vLs<n,vs<1,os<1,hs<x> > > > >, T) { return gen(); }
  template<int x, typename T> Ls<1,vs<1,os<1,hs<x,os<1,T> > > > > operator* (Ls<1,vs<1,os<1,hs<x> > > >, T) { return gen(); }
  os<2,vs<1,os<1,hline<0> > > > operator* (os<1,vline<1> >, os<1>) { return gen(); }
  Ls<1,vs<1,os<1,hline<0> > > > operator* (Ls<1,vs<1,os<1> > >, os<1>) { return gen(); }
  template<typename T> Ls<1,vs<1,os<3,T> > > operator* (Ls<1,vs<1,os<1> > >, os<1,T>) { return gen(); }
  template<int x> Ls<1,vs<1,os<1,hs<x,hline<0> > > > > operator* (Ls<1,vs<1,os<1,hs<x> > > >, os<1>) { return gen(); }
  template<int x, typename T> Ls<1,vs<1,os<1,hs<x,os<2,T> > > > > operator* (Ls<1,vs<1,os<1,hs<x> > > >, os<1,T>) { return gen(); }
  template<int n> Ls<1,vLs<n,vs<1,os<1,hline<0> > > > > operator* (Ls<1,vLs<n,vs<1,os<1> > > >, os<1>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,vs<1,os<3,T> > > > operator* (Ls<1,vLs<n,vs<1,os<1> > > >, os<1,T>) { return gen(); }
  template<int n, int x> Ls<1,vLs<n,vs<1,os<1,hs<x,hline<0> > > > > > operator* (Ls<1,vLs<n,vs<1,os<1,hs<x> > > > >, os<1>) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,T> > > > > > operator* (Ls<1,vLs<n,vs<1,os<1,hs<x> > > > >, os<1,T>) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,vs<1,os<2,T> > > > operator* (Ls<1,vLs<n,vs<1,os<1> > > >, T) { return gen(); }
  template<typename T> Ls<1,vs<1,os<2,T> > > operator* (Ls<1,vs<1,os<1> > >, T) { return gen(); }
  template<int h> Ls<1,vs<1,os<1,vline<h> > > > operator* (Ls<1,vs<1,os<1> > >, vs<h,os<1> >) { return gen(); }
  template<int n, int h> Ls<1,vLs<n,vs<1,os<1,vline<h> > > > > operator* (Ls<1,vLs<n,vs<1,os<1> > > >, vs<h,os<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,os<1,hs<x,hline<0> > > > > > > operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,T> > > > > > > operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, T) { return gen(); }
  template<int x, int h> os<1,hs<x,os<1,vs<1,os<1,hs<x,vline<h> > > > > > > operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, vs<h,os<1> >) { return gen(); }
  template<int x, int z, int h> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,vline<h> > > > > > > > operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, vs<h,os<1> >) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,T> > > > > > > > operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, T) { return gen(); }
  template<int n, int x, typename T> hs<n,os<1,vs<1,os<1,hs<x,os<1,T> > > > > > operator* (hs<n,os<1,vs<1,os<1,hs<x> > > > >, T) { return gen(); }
  template<int n, int x, int z, typename T> hs<n,os<1,vLs<z,vs<1,os<1,hs<x,os<1,T> > > > > > > operator* (hs<n,os<1,vLs<z,vs<1,os<1,hs<x> > > > > >, T) { return gen(); }
  template<int z> os<2,vLs<z,vs<1,os<1,hline<0> > > > > operator* (os<2,vLs<z,vs<1,os<1> > > >, os<1>) { return gen(); }
  template<int z, int h> os<2,vLs<z,vs<1,os<1,vline<h> > > > > operator* (os<2,vLs<z,vs<1,os<1> > > >, vs<h,os<1> >) { return gen(); }
  template<int z, typename T> os<2,vLs<z,vs<1,os<3,T> > > > operator* (os<2,vLs<z,vs<1,os<1> > > >, os<1,T>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,vs<1,os<2,T> > > > operator* (os<2,vLs<z,vs<1,os<1> > > >, T) { return gen(); }
  template<int n, int x> hs<n,os<1,vs<1,os<1,hs<x,hline<0> > > > > > operator* (hs<n,os<1,vs<1,os<1,hs<x> > > > >, os<1>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,hline<0> > > > > > > > operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, os<1>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,T> > > > > > > > operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, os<1,T>) { return gen(); }
  template<int n, int x, int z> hs<n,os<1,vLs<z,vs<1,os<1,hs<x,hline<0> > > > > > > operator* (hs<n,os<1,vLs<z,vs<1,os<1,hs<x> > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int h> hs<n,os<1,vLs<z,vs<1,os<1,hs<x,vline<h> > > > > > > operator* (hs<n,os<1,vLs<z,vs<1,os<1,hs<x> > > > > >, vs<h,os<1> >) { return gen(); }
  template<int n, int x, typename T> hs<n,os<1,vs<1,os<1,hs<x,os<2,T> > > > > > operator* (hs<n,os<1,vs<1,os<1,hs<x> > > > >, os<1,T>) { return gen(); }
  template<int n, int x, int z, typename T> hs<n,os<1,vLs<z,vs<1,os<1,hs<x,os<2,T> > > > > > > operator* (hs<n,os<1,vLs<z,vs<1,os<1,hs<x> > > > > >, os<1,T>) { return gen(); }

  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,T> > > > > operator| (os<1,hs<x,os<1,vLs<z> > > >, T) { return gen(); }
  template<int x, int z, int q, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<q+1,T> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > >, vs<q,T>) { return gen(); }
  template<int x, int z, int h> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,vline<h+1> > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > >, vs<h,os<1> >) { return gen(); }
  template<int q> os<2,vs<1,os<1,vline<q+1> > > > operator| (os<2,vs<1,hline<0> > >, vs<q,os<1> >) { return gen(); }
  template<typename T> os<2,vs<1,T> > operator| (hline<0>, T) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,T> > > > operator| (hline<x>, T) { return gen(); }
  template<int x, int q> os<1,hs<x,os<1,vs<1,os<1,hs<x,vline<q+1> > > > > > > operator| (os<1,hs<x,os<1,vs<1,hline<x> > > > >, vs<q,os<1> >) { return gen(); }
  template<int z, int q> os<2,vLs<z,vs<1,os<1,vline<q+1> > > > > operator| (os<2,vLs<z,vs<1,hline<0> > > >, vs<q,os<1> >) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,vs<1,os<2,vs<q+1,T> > > > > operator| (os<2,vLs<z,vs<1,hline<0> > > >, vs<q,T>) { return gen(); }

  // Rear-bottom-left corner
  template<int n> hs<n,os<1,vline<2> > > operator* (hs<n,os<1> >, vs<2,os<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<1,vline<2> > > > operator* (hline<x>, vs<2,os<1> >) { return gen(); }
  template<int n, typename T> hs<n,os<2,T> > operator* (hs<n,os<1> >, T) { return gen(); }
  template<typename T> Ls<1,vs<1,os<3,T> > > operator* (Ls<1,vs<1,hline<0> > >, T) { return gen(); }
  template<int x, typename T> Ls<1,vs<1,os<1,hs<x,os<2,T> > > > > operator* (Ls<1,vs<1,hline<x> > >, T) { return gen(); }
  template<int n, typename T> Ls<1,vLs<n,vs<1,os<3,T> > > > operator* (Ls<1,vLs<n,vs<1,hline<0> > > >, T) { return gen(); }
  template<int n, int x, typename T> Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,T> > > > > > operator* (Ls<1,vLs<n,vs<1,hline<x> > > >, T) { return gen(); }
  template<int z, typename T> os<2,vLs<z,vs<1,os<3,T> > > > operator* (os<2,vLs<z,vs<1,hline<0> > > >, T) { return gen(); }
  template<int x, int z, typename T> os<1,vLs<z,vs<1,os<1,hs<x,os<2,T> > > > > > operator* (os<1,vLs<z,vs<1,hline<x> > > >, T) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,T> > > > > > >  > operator* (os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > >, T) { return gen(); }
  template<int x> os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,vline<2> > > > > > > > operator* (os<1,hs<x,os<1,vs<1,hline<x> > > > >, vs<2,os<1> >) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,os<2,T> > > > > > > operator* (os<1,hs<x,os<1,vs<1,hline<x> > > > >, T) { return gen(); }
  template<int x, typename T> os<1,vs<1,os<1,hs<x,os<2,T> > > > > operator* (os<1,vs<1,hline<x> > >, T) { return gen(); }
  template<int x> os<1,vs<1,os<1,hs<x,os<1,vline<2> > > > > > operator* (os<1,vs<1,hline<x> > >, vs<2,os<1> >) { return gen(); }
  template<int n, int x> hs<n,os<1,vs<1,os<1,hs<x,os<1,vline<2> > > > > > > operator* (hs<n,os<1,vs<1,hline<x> > > >, vs<2,os<1> >) { return gen(); }
  template<int n, int x, typename T> hs<n,os<1,vs<1,os<1,hs<x,os<2,T> > > > > > operator* (hs<n,os<1,vs<1,hline<x> > > >, T) { return gen(); }
  template<int n, int x, int z, typename T> hs<n,os<1,vLs<z,vs<1,os<1,hs<x,os<2,T> > > > > > > operator* (hs<n,os<1,vLs<z,vs<1,hline<x> > > > >, T) { return gen(); }

  // Two pipes to the right of the rear-bottom-left corner
  template<int z, typename T> os<2,vLs<z,vs<1,os<3,vs<2,T> > > > > operator| (os<2,vLs<z,vs<1,os<1,hline<0> > > > >, vs<1,T>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,T> > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,hline<0> > > > > > > >, vs<1,T>) { return gen(); }
  template<int z, int h, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,T> > > > > > > operator| (os<2,vLs<z,vs<1,os<1,vline<h> > > > >, vs<1,T>) { return gen(); }
  template<int x, int z, int h, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,T> > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,vline<h> > > > > > > >, vs<1,T>) { return gen(); }
  template<int z, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Ls<1,T> > > > > > operator| (os<2,vLs<z,vs<1,os<1,hline<0> > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Ls<1,T> > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,hline<0> > > > > > > >, vLs<1,T>) { return gen(); }
  template<int z, int h, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1,T> > > > > > > > operator| (os<2,vLs<z,vs<1,os<1,vline<h> > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int h, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Ls<1,T> > > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,vline<h> > > > > > > >, vLs<1,T>) { return gen(); }

  template<int z, typename T> os<2,vLs<z,vs<1,os<3,vs<2,T> > > > > operator|| (os<2,vLs<z,vs<1,os<1,hline<0> > > > >, T) { return gen(); }
  template<int z, int h, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,T> > > > > > > operator|| (os<2,vLs<z,vs<1,os<1,vline<h> > > > >, T) { return gen(); }

  // Pipes forming part of "Lvvs"
  template<int x, int z> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<1,Ls<1> > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Ls<1> > > > > > > > > >, vLs<1>) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<p+1,Ls<1> > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<p,Ls<1> > > > > > > > > > >, vLs<1>) { return gen(); }
  template<int x, int z, int q, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<q+1,T> > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Ls<1> > > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int z, int p, int q, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<p+q+1,T> > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<p,Ls<1> > > > > > > > > > >, vs<1, Lvvs<q,T> >) { return gen(); }
  template<int z, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<1,Ls<1,T> > > > > > > operator| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, vLs<1,T>) { return gen(); }
  template<int z, int p, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p+1,Ls<1,T> > > > > > > operator| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int h, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<1,Ls<1,T> > > > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Ls<1> > > > > > > > > > > >, vLs<1,T>) { return gen(); }
  template<int n, int z, int h, int p, typename T> os<n,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p+1,Ls<1,T> > > > > > > > > operator| (os<n,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int h, int p, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<p+1,Ls<1,T> > > > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int h, int q, typename T> os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<q+1,T> > > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Ls<1> > > > > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int h, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<1,Ls<1,T> > > > > > > > > operator| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, vLs<1,T>) { return gen(); }
  template<int z, int h, int q, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<q+1,T> > > > > > > > operator| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<q+1,T> > > > > > operator| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int p, int q, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p+q+1,T> > > > > > operator| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }

  template<int z, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<1,T> > > > > > operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, T) { return gen(); }
  template<int z, int q, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<q+1,T> > > > > > operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int p, int q, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p+q+1,T> > > > > > operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int p, typename T> os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p+1,T> > > > > > operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, T) { return gen(); }
  template<int z, int h, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<1,T> > > > > > > > operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, T) { return gen(); }
  template<int z, int h, int q, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<q+1,T> > > > > > > > operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int h, int p, int q, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p+q+1,T> > > > > > > > operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int h, int p, typename T> os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p+1,T> > > > > > > > operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > >, T) { return gen(); }

  // Near-bottom-left corner

  os<1,hs<1,os<2,vs<2,hline<1> > > > > operator- (os<1,hs<1,os<1,vline<2> > > >, os<1>) { return gen(); }
  template<int x> os<1,hs<x,os<2,vs<2,hline<x> > > > > operator- (os<1,hs<x,os<1,vline<2> > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int p> os<1,hs<x,os<2,vs<2,hline<x> > > > > operator- (os<1,hs<x,os<2,vs<2,os<1,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> os<1,hs<x,os<2,vs<2,hline<x> > > > > operator- (os<1,hs<x,os<2,vs<2,os<1,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int z> os<1,hs<1,os<2,vs<2,Lvvs<z,hline<1> > > > > > operator- (os<1,hs<1,os<2,vs<2,Lvvs<z,os<1> > > > > >, os<1>) { return gen(); }
  template<int x, int z> os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > operator- (os<1,hs<x,os<2,vs<2,Lvvs<z,os<1> > > > > >, hs<x-1, os<1> >) { return gen(); }
  template<int x, int z, int p> os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > operator- (os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<p> > > > > > >, hs<x-p-1, os<1> >) { return gen(); }
  template<int x, int z> os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > operator- (os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<int h> os<1,hs<1,os<1,vs<h,os<1,vs<2,hline<1> > > > > > > operator- (os<1,hs<1,os<1,vs<h,vline<2> > > > >, os<1>) { return gen(); }
  template<int x, int h> os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vs<h,os<1,vs<2,os<1,hs<x-1> > > > > > > >, os<1>) { return gen(); }
  template<int x, int h, int p> os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vs<h,os<1,vs<2,os<1,hs<p> > > > > > > >, hs<x-p-1, os<1> >) { return gen(); }
  template<int x, int h> os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vs<h,vline<2> > > > >, hs<x-1, os<1> >) { return gen(); }
  template<int z, int h> os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<1> > > > > > > > operator- (os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > >, os<1>) { return gen(); }
  template<int x, int z, int h> os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int h, int p> os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z, int h> os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > >, os<1>) { return gen(); }
  template<int h> vs<h,os<1,vs<2,hline<1> > > > operator- (vs<h,vline<2> >, os<1>) { return gen(); }
  template<int h, int p> vs<h,os<1,vs<2,hline<p+1> > > > operator- (vs<h,os<1,vs<2,os<1,hs<p> > > > >, os<1>) { return gen(); }
  template<int h, int p, int q> vs<h,os<1,vs<2,hline<p+q+1> > > > operator- (vs<h,os<1,vs<2,os<1,hs<p> > > > >, hs<q, os<1> >) { return gen(); }
  template<int h, int q> vs<h,os<1,vs<2,hline<q+1> > > > operator- (vs<h,vline<2> >, hs<q, os<1> >) { return gen(); }
  template<int n, int z> vs<n,os<1,vs<2,Lvvs<z,hline<1> > > > > operator- (vs<n,os<1,vs<2,Lvvs<z,os<1> > > > >, os<1>) { return gen(); }
  template<int n, int z, int q> vs<n,os<1,vs<2,Lvvs<z,hline<q+1> > > > > operator- (vs<n,os<1,vs<2,Lvvs<z,os<1> > > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int z, int p, int q> vs<n,os<1,vs<2,Lvvs<z,hline<p+q+1> > > > > operator- (vs<n,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int z, int p> vs<n,os<1,vs<2,Lvvs<z,hline<p+1> > > > > operator- (vs<n,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > >, os<1>) { return gen(); }
  template<int z> Ls<1,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,hline<1> > > > > > > > operator- (Ls<1,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,os<1> > > > > > > >, os<1>) { return gen(); }
  template<int x, int z> Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1> > > > > > > >, hs<x-1, os<1> >) { return gen(); }
  template<int x, int z, int p> Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > >, hs<x-p-1, os<1> >) { return gen(); }
  template<int x, int z> Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > >, os<1>) { return gen(); }
  template<int z, int h> Ls<1,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<1> > > > > > > > > > operator- (Ls<1,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > >, os<1>) { return gen(); }
  template<int x, int z, int h> Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > >, hs<x-1, os<1> >) { return gen(); }
  template<int x, int z, int h, int p> Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > > > >, hs<x-p-1, os<1> >) { return gen(); }
  template<int x, int z, int h> Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > operator- (Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > > > >, os<1>) { return gen(); }
  template<int n, int z> Ls<1,vLs<n,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,hline<1> > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,os<1> > > > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1> > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int z, int p> Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z> Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > > >, os<1>) { return gen(); }
  template<int n, int z, int h> Ls<1,vLs<n,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<1> > > > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int z, int h> Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int z, int h, int p> Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int z, int h> Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > operator- (Ls<1,vLs<n,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > > > > >, os<1>) { return gen(); }

  // Near-bottom-right corner

// Constructing a cuboid with H > D: o---o|&!&!o---o|!!!!!o|!&!!&!!o---o

  // Rear-top-left corner
  cuboid<0,1,0> operator* (os<1,vs<1,os<3,vs<2,hline<0> > > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator* (hs<x,os<1,vs<1,os<1,hs<x,os<2,vs<2,hline<x> > > > > > > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator* (os<1,vLs<z,vs<1,os<3,vs<2,Lvvs<z,hline<0> > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator* (hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > >) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator* (os<1,vs<1,os<2,vs<h,os<1,vs<2,hline<0> > > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator* (hs<x,os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator* (os<1,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator* (hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > >) { return gen(); }

  cuboid<1,1,0> operator- (os<1>, os<1,vs<1,os<1,hs<1,os<2,vs<2,hline<1> > > > > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1>, hs<x-1,os<1,vs<1,os<1,hs<x,os<2,vs<2,hline<x> > > > > > > >) { return gen(); }
  template<int x, int p> cuboid<x,1,0> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vs<1,os<1,hs<x,os<2,vs<2,hline<x> > > > > > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1,hs<x-1> >, os<1,vs<1,os<1,hs<x,os<2,vs<2,hline<x> > > > > > >) { return gen(); }
  template<int z> cuboid<1,z+1,z> operator- (os<1>, os<1,vLs<z,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,hline<1> > > > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1>, hs<x-1,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z+1,z> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1,hs<x-1> >, os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > >) { return gen(); }
  template<int h> cuboid<1,h/3+1,0> operator- (os<1>, os<1,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,hline<1> > > > > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1>, hs<x-1,os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int h, int p> cuboid<x,h/3+1,0> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1,hs<x-1> >, os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > > >) { return gen(); }
  template<int z, int h> cuboid<1,z+h/3+1,z> operator- (os<1>, os<1,vLs<z,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<1> > > > > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1>, hs<x-1,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > >) { return gen(); }
  template<int x, int z, int h, int p> cuboid<x,z+h/3+1,z> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x-1> >, os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > >) { return gen(); }

  // Rear-top-right corner
  cuboid<0,1,0> operator* (os<1>, vs<1,os<3,vs<2,hline<0> > > >) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator* (os<1>, vs<1,os<2,vs<h,os<1,vs<2,hline<0> > > > > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator* (os<1>, vLs<z,vs<1,os<3,vs<2,Lvvs<z,hline<0> > > > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator* (os<1>, vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator* (os<1,hs<x> >, vs<1,os<1,hs<x,os<2,vs<2,hline<x> > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator* (os<1,hs<x> >, vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator* (os<1,hs<x> >, vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator* (os<1,hs<x> >, vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > >) { return gen(); }

  cuboid<0,2,1> operator| (hline<0>, Ls<1,vs<1,os<3,vs<2,Lvvs<1,hline<0> > > > > >) { return gen(); }
  template<int h> cuboid<0,h/3+2,1> operator| (hline<0>, Ls<1,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<1,hline<0> > > > > > > >) { return gen(); }
  template<int x> cuboid<x,2,1> operator| (hline<x>, Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<1,hline<x> > > > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+2,1> operator| (hline<x>, Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<1,hline<x> > > > > > > > > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (hline<0>, Ls<1,vLs<z-1,vs<1,os<3,vs<2,Lvvs<z,hline<0> > > > > > >) { return gen(); }
  template<int z, int p> cuboid<0,z+1,z> operator| (os<2,vLs<p> >, Ls<1,vLs<z-p-1,vs<1,os<3,vs<2,Lvvs<z,hline<0> > > > > > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (os<2,vLs<z-1> >, Ls<1,vs<1,os<3,vs<2,Lvvs<z,hline<0> > > > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (hline<0>, Ls<1,vLs<z-1,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > > > > >) { return gen(); }
  template<int z, int h, int p> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<p> >, Ls<1,vLs<z-p-1,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > > > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z-1> >, Ls<1,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (hline<x>, Ls<1,vLs<z-1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,vLs<z-p-1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z-1> > > >, Ls<1,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (hline<x>, Ls<1,vLs<z-1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > >) { return gen(); }
  template<int x, int z, int h, int p> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,vLs<z-p-1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z-1> > > >, Ls<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > > > >) { return gen(); }

  // Single pipe to the left of the near-top-left corner
  cuboid<0,1,0> operator| (hline<0>, os<3,vs<2,hline<0> > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (os<2,vLs<z> >, os<3,vs<2,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator| (hline<x>, os<1,hs<x,os<2,vs<2,hline<x> > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z> > > >, os<1,hs<x,os<2,vs<2,Lvvs<z,hline<x> > > > > >) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator| (hline<0>, os<2,vs<h,os<1,vs<2,hline<0> > > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z> >, os<2,vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator| (hline<x>, os<1,hs<x,os<1,vs<h,os<1,vs<2,hline<x> > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z> > > >, os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > > >) { return gen(); }

  // Near-top-left corner

  cuboid<1,1,0> operator- (os<1,hs<1,vline<1> > >, os<2,vs<2,hline<1> > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1,hs<x,vline<1> > >, hs<x-1,os<2,vs<2,hline<x> > > >) { return gen(); }
  template<int x, int p> cuboid<x,1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<p> > > > > >, hs<x-p-1,os<2,vs<2,hline<x> > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x-1> > > > > >, os<2,vs<2,hline<x> > >) { return gen(); }
  template<int z> cuboid<1,z+1,z> operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1> > > > > >, os<2,vs<2,Lvvs<z,hline<1> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1> > > > > >, hs<x-1,os<2,vs<2,Lvvs<z,hline<x> > > > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<p> > > > > > >, hs<x-p-1,os<2,vs<2,Lvvs<z,hline<x> > > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x-1> > > > > > >, os<2,vs<2,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int h> cuboid<1,h/3+1,0> operator- (os<1,hs<1,vline<1> > >, os<1,vs<h,os<1,vs<2,hline<1> > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1,hs<x,vline<1> > >, hs<x-1,os<1,vs<h,os<1,vs<2,hline<x> > > > > >) { return gen(); }
  template<int x, int h, int p> cuboid<x,h/3+1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<p> > > > > >, hs<x-p-1,os<1,vs<h,os<1,vs<2,hline<x> > > > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x-1> > > > > >, os<1,vs<h,os<1,vs<2,hline<x> > > > >) { return gen(); }
  template<int z, int h> cuboid<1,z+h/3+1,z> operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1> > > > > >, os<1,vs<h,os<1,vs<2,Lvvs<z,hline<1> > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1> > > > > >, hs<x-1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > >) { return gen(); }
  template<int x, int z, int h, int p> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<p> > > > > > >, hs<x-p-1,os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x-1> > > > > > >, os<1,vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > > >) { return gen(); }

  // Near-top-right corner
  cuboid<0,1,0> operator* (os<1,vline<1> >, os<1,vs<2,hline<0> > >) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator* (os<1,vline<1> >, vs<h,os<1,vs<2,hline<0> > > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator* (os<2,vLs<z,vs<1,os<1> > > >, os<1,vs<2,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator* (os<2,vLs<z,vs<1,os<1> > > >, vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, os<1,vs<2,hline<x> > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, vs<h,os<1,vs<2,hline<x> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, os<1,vs<2,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator* (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x> > > > > > >, vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > >) { return gen(); }

  template<int h> cuboid<0,h/3+2,0> operator| (os<2,vs<1,hline<0> > >, vs<h,os<1,vs<2,hline<0> > > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+2,0> operator| (os<1,hs<x,os<1,vs<1,hline<x> > > > >, vs<h,os<1,vs<2,hline<x> > > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+2,z> operator| (os<2,vLs<z,vs<1,hline<0> > > >, vs<h,os<1,vs<2,Lvvs<z,hline<0> > > > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+2,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > >, vs<h,os<1,vs<2,Lvvs<z,hline<x> > > > >) { return gen(); }

  // Rear-bottom-left corner
  cuboid<0,1,0> operator* (os<2,vs<1,hline<0> > >, vs<2,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator* (os<2,vLs<z,vs<1,hline<0> > > >, vs<2,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator* (os<1,hs<x,os<1,vs<1,hline<x> > > > >, vs<2,hline<x> >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator* (os<1,hs<x,os<1,vLs<z,vs<1,hline<x> > > > > >, vs<2,Lvvs<z,hline<x> > >) { return gen(); }

  // Two pipes to the right of the rear-bottom-left corner
  cuboid<0,1,0> operator| (os<2,vs<1,os<1,hline<0> > > >, vs<1,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (os<2,vLs<z,vs<1,os<1,hline<0> > > > >, vs<1,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x> cuboid<x,1,0> operator| (os<1,hs<x,os<1,vs<1,os<1,hs<x,hline<0> > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,hline<0> > > > > > > >, vs<1,Lvvs<z,hline<x> > >) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator| (os<2,vs<1,os<1,vline<h> > > >, vs<1,hline<0> >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z,vs<1,os<1,vline<h> > > > >, vs<1,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator| (os<1,hs<x,os<1,vs<1,os<1,hs<x,vline<h> > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,vline<h> > > > > > > >, vs<1,Lvvs<z,hline<x> > >) { return gen(); }

  cuboid<0,1,0> operator|| (os<2,vs<1,os<1,hline<0> > > >, hline<0>) { return gen(); }
  template<int h> cuboid<0,h/3+1,0> operator|| (os<2,vs<1,os<1,vline<h> > > >, hline<0>) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator|| (os<2,vLs<z,vs<1,os<1,hline<0> > > > >, Lvvs<z,hline<0> >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator|| (os<2,vLs<z,vs<1,os<1,vline<h> > > > >, Lvvs<z,hline<0> >) { return gen(); }

  // Pipes forming part of "Lvvs"
  cuboid<0,2,1> operator| (os<2,vLs<1,vs<1,os<3,vs<2,Ls<1> > > > > >, vs<1,hline<0> >) { return gen(); }
  template<int x> cuboid<x,2,1> operator| (os<1,hs<x,os<1,vLs<1,vs<1,os<1,hs<x,os<2,vs<2,Ls<1> > > > > > > > > >, vs<1, hline<x> >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Ls<1> > > > > > > > > >, vs<1,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<p,Ls<1> > > > > > > > > > >, vs<1,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z-1,Ls<1> > > > > > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, vs<1,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int z, int p> cuboid<0,z+1,z> operator| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, vs<1,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<z-1,Ls<1> > > > > > >, vs<1,hline<0> >) { return gen(); }
  template<int h> cuboid<0,h/3+2,1> operator| (os<2,vLs<1,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, vs<1,hline<0> >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+2,1> operator| (os<1,hs<x,os<1,vLs<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Ls<1> > > > > > > > > > > >, vs<1, hline<x> >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Ls<1> > > > > > > > > > > >, vs<1,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int x, int z, int h, int p> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > > > > > >, vs<1,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator| (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z-1,Ls<1> > > > > > > > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, vs<1,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int z, int h, int p> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > >, vs<1,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z-1,Ls<1> > > > > > > > >, vs<1,hline<0> >) { return gen(); }

  cuboid<0,2,1> operator|| (os<2,vLs<1,vs<1,os<3,vs<2,Ls<1> > > > > >, hline<0>) { return gen(); }
  template<int h> cuboid<0,h/3+2,1> operator|| (os<2,vLs<1,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, hline<0>) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Ls<1> > > > > >, Lvvs<z-1,hline<0> >) { return gen(); }
  template<int z, int p> cuboid<0,z+1,z> operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<p,Ls<1> > > > > > >, Lvvs<z-p-1,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z+1,z> operator|| (os<2,vLs<z,vs<1,os<3,vs<2,Lvvs<z-1,Ls<1> > > > > > >, hline<0>) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Ls<1> > > > > > > >, Lvvs<z-1,hline<0> >) { return gen(); }
  template<int z, int h, int p> cuboid<0,z+h/3+1,z> operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<p,Ls<1> > > > > > > > >, Lvvs<z-p-1,hline<0> >) { return gen(); }
  template<int z, int h> cuboid<0,z+h/3+1,z> operator|| (os<2,vLs<z,vs<1,os<2,vs<h,os<1,vs<2,Lvvs<z-1,Ls<1> > > > > > > > >, hline<0>) { return gen(); }

  // Near-bottom-left corner

  cuboid<1,1,0> operator- (os<1,hs<1,os<1,vs<1,os<1,hs<1,os<1,vline<2> > > > > > > >, os<1>) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,vline<2> > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int p> cuboid<x,1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<2,vs<2,os<1,hs<p> > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x> cuboid<x,1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<2,vs<2,os<1,hs<x-1> > > > > > > > > >, os<1>) { return gen(); }
  template<int h> cuboid<1,h/3+1,0> operator- (os<1,hs<1,os<1,vs<1,os<1,hs<1,os<1,vs<h,vline<2> > > > > > > > >, os<1>) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,vs<h,vline<2> > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int h, int p> cuboid<x,h/3+1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,os<1,hs<p> > > > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int h> cuboid<x,h/3+1,0> operator- (os<1,hs<x,os<1,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,os<1,hs<x-1> > > > > > > > > > > >, os<1>) { return gen(); }
  template<int z> cuboid<1,z+1,z> operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1,hs<1,os<2,vs<2,Lvvs<z,os<1> > > > > > > > > > >, os<1>) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1> > > > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z> cuboid<x,z+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<2,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > > > > >, os<1>) { return gen(); }
  template<int z, int h> cuboid<1,z+h/3+1,z> operator- (os<1,hs<1,os<1,vLs<z,vs<1,os<1,hs<1,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > > > > >, os<1>) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1> > > > > > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int z, int h, int p> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<p> > > > > > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int z, int h> cuboid<x,z+h/3+1,z> operator- (os<1,hs<x,os<1,vLs<z,vs<1,os<1,hs<x,os<1,vs<h,os<1,vs<2,Lvvs<z,os<1,hs<x-1> > > > > > > > > > > > > >, os<1>) { return gen(); }

  // Near-bottom-right corner

// Misc 3-D constructions for H < D

  // Rear-bottom-left corner
  template<int x, int y, typename T> os<1,hs<x,os<1,vLs<y,os<1,T> > > > > operator* (os<1,hs<x,os<1,vLs<y> > > >, T) { return gen(); }

  // Ls
  template<int q, typename T> os<3,Ls<q+1,T> > operator& (os<1,hline<0> >, Ls<q,T>) { return gen(); }
  template<int p, int q, typename T> os<3,Ls<p+q+1,T> > operator& (os<3,Ls<p> >, Ls<q,T>) { return gen(); }
  template<int x, int q, typename T> os<1,hs<x,os<2,Ls<q+1,T> > > > operator& (os<1,hs<x,hline<0> > >, Ls<q,T>) { return gen(); }
  template<int x, int p, int q, typename T> os<1,hs<x,os<2,Ls<p+q+1,T> > > > operator& (os<1,hs<x,os<2,Ls<p> > > >, Ls<q,T>) { return gen(); }
  template<int x, int y, int q, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<q+1,T> > > > > > operator& (os<1,hs<x,os<1,vLs<y,os<1> > > > >, Ls<q,T>) { return gen(); }
  template<int x, int y, int p, int q, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<p+q+1,T> > > > > > operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<p> > > > > >, Ls<q,T>) { return gen(); }
  template<int x, int y, int p, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<p+1,T> > > > > > operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<p> > > > > >, T) { return gen(); }
  template<int q, typename T> Ls<1,os<1,Ls<q+1,T> > > operator& (Ls<1,os<1> >, Ls<q,T>) { return gen(); }
  template<int p, int q, typename T> Ls<1,os<1,Ls<p+q+1,T> > > operator& (Ls<1,os<1,Ls<p> > >, Ls<q,T>) { return gen(); }
  template<int p, typename T> Ls<1,os<1,Ls<p+1,T> > > operator& (Ls<1,os<1,Ls<p> > >, T) { return gen(); }
  template<int n, int q, typename T> Ls<1,vLs<n,os<1,Ls<q+1,T> > > > operator& (Ls<1,vLs<n,os<1> > >, Ls<q,T>) { return gen(); }
  template<int n, int p, int q, typename T> Ls<1,vLs<n,os<1,Ls<p+q+1,T> > > > operator& (Ls<1,vLs<n,os<1,Ls<p> > > >, Ls<q,T>) { return gen(); }
  template<int n, int p, typename T> Ls<1,vLs<n,os<1,Ls<p+1,T> > > > operator& (Ls<1,vLs<n,os<1,Ls<p> > > >, T) { return gen(); }
  template<int y, int q, typename T> os<2,vLs<y,os<1,Ls<q+1,T> > > > operator& (os<2,vLs<y,os<1> > >, Ls<q,T>) { return gen(); }
  template<int y, int p, int q, typename T> os<2,vLs<y,os<1,Ls<p+q+1,T> > > > operator& (os<2,vLs<y,os<1,Ls<p> > > >, Ls<q,T>) { return gen(); }
  template<int y, int p, typename T> os<2,vLs<y,os<1,Ls<p+1,T> > > > operator& (os<2,vLs<y,os<1,Ls<p> > > >, T) { return gen(); }

  template<int p, int q, typename T> os<3,Ls<p+q+2,T> > operator&& (os<3,Ls<p> >, Ls<q,T>) { return gen(); }
  template<int x, int p, int q, typename T> os<1,hs<x,os<2,Ls<p+q+2,T> > > > operator&& (os<1,hs<x,os<2,Ls<p> > > >, Ls<q,T>) { return gen(); }

  // Near-top-left corner
  os<3,Ls<2,hline<0> > > operator* (os<3,Ls<2> >, os<1>) { return gen(); }
  template<int n, int x> hs<n,os<2,Ls<2,hline<x> > > > operator* (hs<n,os<2,Ls<2> > >, hs<x,os<1> >) { return gen(); }
  template<int n, typename T> hs<n,os<2,Ls<2,os<1,T> > > > operator* (hs<n,os<2,Ls<2> > >, T) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<2,Ls<2,os<1,T> > > > > operator* (os<1,hs<x,os<2,Ls<2> > > >, T) { return gen(); }
  template<int x> os<2,Ls<2,rectangle<x,0> > > operator* (os<2,Ls<2> >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int x> os<2,Ls<2,hline<x> > > operator* (os<2,Ls<2> >, hs<x,os<1> >) { return gen(); }
  template<int y, int d> os<2,vLs<y,os<1,Ls<d,hline<0> > > > > operator* (os<2,vLs<y,os<1,Ls<d> > > >, os<1>) { return gen(); }
  template<int y, int d, typename T> os<2,vLs<y,os<1,Ls<d,os<2,T> > > > > operator* (os<2,vLs<y,os<1,Ls<d> > > >, os<1,T>) { return gen(); }
  template<int n> Ls<n,rectangle<0,0> > operator* (Ls<n>, os<1,hline<0> >) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator* (Ls<n>, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int n> Ls<n,hline<0> > operator* (Ls<n>, os<1>) { return gen(); }
  template<int n, typename T> Ls<n,os<2,T> > operator* (Ls<n>, os<1,T>) { return gen(); }
  template<int n, typename T> Ls<n,os<1,T> > operator* (Ls<n>, T) { return gen(); }
  template<int d> Ls<1,os<1,Ls<d,hline<0> > > > operator* (Ls<1,os<1,Ls<d> > >, os<1>) { return gen(); }
  template<int n, int d> Ls<1,vLs<n,os<1,Ls<d,hline<0> > > > > operator* (Ls<1,vLs<n,os<1,Ls<d> > > >, os<1>) { return gen(); }
  template<int n, int x> Ls<n,hline<x> > operator* (Ls<n>, hs<x,os<1> >) { return gen(); }
  template<int d, int x> Ls<1,os<1,Ls<d,hline<x> > > > operator* (Ls<1,os<1,Ls<d> > >, hs<x,os<1> >) { return gen(); }
  template<int n, int d, int x> Ls<1,vLs<n,os<1,Ls<d,hline<x> > > > > operator* (Ls<1,vLs<n,os<1,Ls<d> > > >, hs<x,os<1> >) { return gen(); }
  template<int d, typename T> Ls<1,os<1,Ls<d,os<2,T> > > > operator* (Ls<1,os<1,Ls<d> > >, os<1,T>) { return gen(); }
  template<int d, typename T> Ls<1,os<1,Ls<d,os<1,T> > > > operator* (Ls<1,os<1,Ls<d> > >, T) { return gen(); }
  template<int n, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<2,T> > > > > operator* (Ls<1,vLs<n,os<1,Ls<d> > > >, os<1,T>) { return gen(); }
  template<int n, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,T> > > > > operator* (Ls<1,vLs<n,os<1,Ls<d> > > >, T) { return gen(); }
  template<int n, int x, int y, int d> hs<n,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > operator* (hs<n,os<1,vLs<y,os<1,Ls<d> > > > >, hs<x,os<1> >) { return gen(); }
  template<int n, int y, int d, typename T> hs<n,os<1,vLs<y,os<1,Ls<d,os<1,T> > > > > > operator* (hs<n,os<1,vLs<y,os<1,Ls<d> > > > >, T) { return gen(); }
  template<int x, int y, int d> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > > operator* (os<1,hs<x,os<1,vLs<y,os<1,Ls<d> > > > > >, hs<x,os<1> >) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,T> > > > > > > operator* (os<1,hs<x,os<1,vLs<y,os<1,Ls<d> > > > > >, T) { return gen(); }
  template<int x, int y, int d> os<1,vLs<y,os<1,Ls<d,hline<x> > > > > operator* (os<1,vLs<y,os<1,Ls<d> > > >, hs<x,os<1> >) { return gen(); }
  template<int y, int d, typename T> os<1,vLs<y,os<1,Ls<d,os<1,T> > > > > operator* (os<1,vLs<y,os<1,Ls<d> > > >, T) { return gen(); }
  template<typename T> os<2,Ls<2,os<1,T> > > operator* (os<2,Ls<2> >, T) { return gen(); }
  template<int n, int x> hs<n,os<2,Ls<2,rectangle<x,0> > > > operator* (hs<n,os<2,Ls<2> > >, hs<x,os<1,hline<x> > >) { return gen(); }

  template<int n> Ls<n,hline<1> > operator- (Ls<n,os<1> >, os<1>) { return gen(); }
  template<int n, int q> Ls<n,hline<q+1> > operator- (Ls<n,os<1> >, hs<q,os<1> >) { return gen(); }
  template<int n, int p, int q> Ls<n,hline<p+q+1> > operator- (Ls<n,os<1,hs<p> > >, hs<q,os<1> >) { return gen(); }
  template<int n, int p> Ls<n,hline<p+1> > operator- (Ls<n,os<1,hs<p> > >, os<1>) { return gen(); }
  template<int n, typename T> Ls<n,os<1,hs<1,T> > > operator- (Ls<n,os<1> >, T) { return gen(); }
  template<int n, int q, typename T> Ls<n,os<1,hs<q+1,T> > > operator- (Ls<n,os<1> >, hs<q,T>) { return gen(); }
  template<int n, int p, int q, typename T> Ls<n,os<1,hs<p+q+1,T> > > operator- (Ls<n,os<1,hs<p> > >, hs<q,T>) { return gen(); }
  template<int n, int p, typename T> Ls<n,os<1,hs<p+1,T> > > operator- (Ls<n,os<1,hs<p> > >, T) { return gen(); }
  template<int d> Ls<1,os<1,Ls<d,hline<1> > > > operator- (Ls<1,os<1,Ls<d,os<1> > > >, os<1>) { return gen(); }
  template<int d, int q> Ls<1,os<1,Ls<d,hline<q+1> > > > operator- (Ls<1,os<1,Ls<d,os<1> > > >, hs<q,os<1> >) { return gen(); }
  template<int d, int p, int q> Ls<1,os<1,Ls<d,hline<p+q+1> > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<p> > > > >, hs<q,os<1> >) { return gen(); }
  template<int d, int p> Ls<1,os<1,Ls<d,hline<p+1> > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<p> > > > >, os<1>) { return gen(); }
  template<int d, typename T> Ls<1,os<1,Ls<d,os<1,hs<1,T> > > > > operator- (Ls<1,os<1,Ls<d,os<1> > > >, T) { return gen(); }
  template<int d, int q, typename T> Ls<1,os<1,Ls<d,os<1,hs<q+1,T> > > > > operator- (Ls<1,os<1,Ls<d,os<1> > > >, hs<q,T>) { return gen(); }
  template<int d, int p, int q, typename T> Ls<1,os<1,Ls<d,os<1,hs<p+q+1,T> > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<p> > > > >, hs<q,T>) { return gen(); }
  template<int d, int p, typename T> Ls<1,os<1,Ls<d,os<1,hs<p+1,T> > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<p> > > > >, T) { return gen(); }
  template<int n, int d> Ls<1,vLs<n,os<1,Ls<d,hline<1> > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1> > > > >, os<1>) { return gen(); }
  template<int n, int d, int q> Ls<1,vLs<n,os<1,Ls<d,hline<q+1> > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1> > > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int d, int p, int q> Ls<1,vLs<n,os<1,Ls<d,hline<p+q+1> > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p> > > > > >, hs<q,os<1> >) { return gen(); }
  template<int n, int d, int p> Ls<1,vLs<n,os<1,Ls<d,hline<p+1> > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p> > > > > >, os<1>) { return gen(); }
  template<int n, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<1,T> > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1> > > > >, T) { return gen(); }
  template<int n, int d, int q, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<q+1,T> > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1> > > > >, hs<q,T>) { return gen(); }
  template<int n, int d, int p, int q, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p+q+1,T> > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p> > > > > >, hs<q,T>) { return gen(); }
  template<int n, int d, int p, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p+1,T> > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<p> > > > > >, T) { return gen(); }
  template<int x, int y, int d> os<1,hs<1,os<1,vLs<y,os<1,Ls<d,hline<1> > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, os<1>) { return gen(); }
  template<int x, int y, int d> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int y, int d, int p> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<p> > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int y, int d> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x-1> > > > > > > >, os<1>) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<1,os<1,vLs<y,os<1,Ls<d,os<1,hs<1,T> > > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, T) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,T> > > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, hs<x-1,T>) { return gen(); }
  template<int x, int y, int d, int p, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,T> > > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<p> > > > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,T> > > > > > > > operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x-1> > > > > > > >, T) { return gen(); }
  template<int n> Ls<n,rectangle<1,0> > operator- (Ls<n,os<1> >, os<1,hline<1> >) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1> >, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int n, int x, int p> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1,hs<p> > >, hs<x-p-1,os<1,hline<x> > >) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1,hs<x-1> > >, os<1,hline<x> >) { return gen(); }
  template<int d, typename T> os<1,hs<1,os<2,Ls<d,os<1,hs<1,T> > > > > > operator- (os<1,hs<1,os<2,Ls<d,os<1> > > > >, T) { return gen(); }
  template<int x, int d, typename T> os<1,hs<x,os<2,Ls<d,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,os<2,Ls<d,os<1> > > > >, hs<x-1,T>) { return gen(); }
  template<int x, int d, int p, typename T> os<1,hs<x,os<2,Ls<d,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<p> > > > > >, hs<x-p-1,T>) { return gen(); }
  template<int x, int d, typename T> os<1,hs<x,os<2,Ls<d,os<1,hs<x,T> > > > > > operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<x-1> > > > > >, T) { return gen(); }

  // Near-top-right corner
  Ls<1,rectangle<0,0> > operator* (Ls<1,os<1> >, hline<0>) { return gen(); }
  template<int d, typename T> Ls<1,os<1,Ls<d,os<2,T> > > > operator* (Ls<1,os<1,Ls<d,os<1> > > >, T) { return gen(); }
  template<int x, int d, typename T> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,T> > > > > > operator* (Ls<1,os<1,Ls<d,os<1,hs<x> > > > >, T) { return gen(); }
  template<int n, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<2,T> > > > > operator* (Ls<1,vLs<n,os<1,Ls<d,os<1> > > > >, T) { return gen(); }
  template<int n, int x, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,T> > > > > > > operator* (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x> > > > > >, T) { return gen(); }
  template<int y, int d, typename T> os<2,vLs<y,os<1,Ls<d,os<2,T> > > > > operator* (os<2,vLs<y,os<1,Ls<d,os<1> > > > >, T) { return gen(); }
  template<int n, int x, typename T> Ls<n,os<1,hs<x,os<1,T> > > > operator* (Ls<n,os<1,hs<x> > >, T) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,T> > > > > > > > > operator* (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x> > > > > > > >, T) { return gen(); }
  template<int n, int x, int y, int d, typename T> hs<n,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,T> > > > > > > > operator* (hs<n,os<1,vLs<y,os<1,Ls<d,os<1,hs<x> > > > > > >, T) { return gen(); }
  template<int x> Ls<1,os<1,hs<x,hline<0> > > > operator* (Ls<1,os<1,hs<x> > >, os<1>) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator* (Ls<n,os<1,hs<x> > >, hline<x>) { return gen(); }
  template<int x> os<1,hs<x,os<2,Ls<2,hline<x> > > > > operator* (os<1,hs<x,os<2,Ls<2> > > >, hs<x,os<1> >) { return gen(); }
  template<int x, typename T> Ls<1,os<1,hs<x,os<2,T> > > > operator* (Ls<1,os<1,hs<x> > >, os<1,T>) { return gen(); }
  template<int n, int x> hs<n,os<2,Ls<2,rectangle<x,0> > > > operator* (hs<n,os<2,Ls<2,os<1,hs<x> > > > >, hline<x>) { return gen(); }
  template<int n, int x> hs<n,os<2,Ls<2,os<1,hs<x,hline<0> > > > > > operator* (hs<n,os<2,Ls<2,os<1,hs<x> > > > >, os<1>) { return gen(); }
  template<int n, int x, typename T> hs<n,os<2,Ls<2,os<1,hs<x,os<2,T> > > > > > operator* (hs<n,os<2,Ls<2,os<1,hs<x> > > > >, os<1,T>) { return gen(); }
  template<int x> os<1,hs<x,os<2,Ls<2,os<1,hs<x,hline<0> > > > > > > operator* (os<1,hs<x,os<2,Ls<2,os<1,hs<x> > > > > >, os<1>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<1,vs<1,os<1,hs<x,os<2,T> > > > > > > operator* (os<1,hs<x,os<1,vs<1,os<1,hs<x> > > > > >, os<1,T>) { return gen(); }
  template<int x, typename T> os<1,hs<x,os<2,Ls<2,os<1,hs<x,os<2,T> > > > > > > operator* (os<1,hs<x,os<2,Ls<2,os<1,hs<x> > > > > >, os<1,T>) { return gen(); }

  // Ampersands forming part of "Lvvs"
  template<int d, typename T> Ls<1,os<1,Ls<d,os<2,Lvvs<1,T> > > > > operator& (Ls<1,os<1,Ls<d,hline<0> > > >, vs<2,T>) { return gen(); }
  template<int d, int q, typename T> Ls<1,os<1,Ls<d,os<2,Lvvs<q+1,T> > > > > operator& (Ls<1,os<1,Ls<d,hline<0> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int d, int q, typename T> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<q+1,T> > > > > > > operator& (Ls<1,os<1,Ls<d,hline<x> > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int d, typename T> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<1,T> > > > > > > operator& (Ls<1,os<1,Ls<d,hline<x> > > >, vs<2,T>) { return gen(); }
  template<int y, int d, typename T> os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<1,T> > > > > > operator& (os<2,vLs<y,os<1,Ls<d,hline<0> > > > >, vs<2,T>) { return gen(); }
  template<int y, int d, int q, typename T> os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<q+1,T> > > > > > operator& (os<2,vLs<y,os<1,Ls<d,hline<0> > > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int x, int y, int d, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<1,T> > > > > > > > > > operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > >, vs<2,T>) { return gen(); }
  template<int x, int y, int d, int q, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<q+1,T> > > > > > > > > > operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<2,Lvvs<1,T> > > > > > operator& (Ls<1,vLs<n,os<1,Ls<d,hline<0> > > > >, vs<2,T>) { return gen(); }
  template<int n, int d, int q, typename T> Ls<1,vLs<n,os<1,Ls<d,os<2,Lvvs<q+1,T> > > > > > operator& (Ls<1,vLs<n,os<1,Ls<d,hline<0> > > > >, vs<2,Lvvs<q,T> >) { return gen(); }
  template<int n, int x, int d, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<1,T> > > > > > > > operator& (Ls<1,vLs<n,os<1,Ls<d,hline<x> > > > >, vs<2,T>) { return gen(); }
  template<int n, int x, int d, int q, typename T> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<q+1,T> > > > > > > > operator& (Ls<1,vLs<n,os<1,Ls<d,hline<x> > > > >, vs<2,Lvvs<q,T> >) { return gen(); }

  // Pipes forming part of "Lvvs"
  template<int y, int d, int q, typename T> os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<q+1,T> > > > > > operator| (os<2,vLs<y,os<1,Ls<d,os<2,Ls<1> > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int y, int d, int p, int q, typename T> os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<p+q+1,T> > > > > > operator| (os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int y, int d, int q, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<q+1,T> > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Ls<1> > > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int x, int y, int d, int p, int q, typename T> os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<p+q+1,T> > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > > > >, vs<1,Lvvs<q,T> >) { return gen(); }
  template<int z, int d, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<1,Ls<1,T> > > > > > > operator| (os<2,vLs<z,os<1,Ls<d,os<2,Ls<1> > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int d, typename T> os<1,hs<x,os<1,vLs<z,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<1,Ls<1,T> > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<1,Ls<d,os<1,hs<x,os<1,Ls<1> > > > > > > > > >, vLs<1,T>) { return gen(); }
  template<int z, int d, int p, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p+1,Ls<1,T> > > > > > > operator| (os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, vLs<1,T>) { return gen(); }
  template<int x, int z, int d, int p, typename T> os<1,hs<x,os<1,vLs<z,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<p+1,Ls<1,T> > > > > > > > > > > operator| (os<1,hs<x,os<1,vLs<z,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > > > >, vLs<1,T>) { return gen(); }

  template<int z, int d, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<1,T> > > > > > operator|| (os<2,vLs<z,os<1,Ls<d,os<2,Ls<1> > > > > >, T) { return gen(); }
  template<int z, int d, int q, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<q+1,T> > > > > > operator|| (os<2,vLs<z,os<1,Ls<d,os<2,Ls<1> > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int d, int p, int q, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p+q+1,T> > > > > > operator|| (os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, Lvvs<q,T>) { return gen(); }
  template<int z, int d, int p, typename T> os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p+1,T> > > > > > operator|| (os<2,vLs<z,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, T) { return gen(); }

  // Near-bottom-left corner
  template<int n> Ls<n,rectangle<0,0> > operator* (Ls<n,hline<0> >, os<1>) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator* (Ls<n,hline<x> >, hs<x,os<1> >) { return gen(); }
  template<int n, int x, int d> os<n,Ls<d,rectangle<x,0> > > operator* (os<n,Ls<d,hline<x> > >, hs<x,os<1> >) { return gen(); }
  template<int n, int x> hs<n,os<2,Ls<2,rectangle<x,0> > > > operator* (hs<n,os<2,Ls<2,hline<x> > > >, hs<x,os<1> >) { return gen(); }

  template<int n, int y> Ls<n,os<1,hs<1,os<1,Lvvs<y,hline<1> > > > > > operator- (Ls<n,os<1,hs<1,os<1,Lvvs<y,os<1> > > > > >, os<1>) { return gen(); }
  template<int n, int x, int y> Ls<n,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > operator- (Ls<n,os<1,hs<x,os<1,Lvvs<y,os<1> > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int y, int p> Ls<n,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > operator- (Ls<n,os<1,hs<x,os<1,Lvvs<y,os<1,hs<p> > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int y> Ls<n,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > operator- (Ls<n,os<1,hs<x,os<1,Lvvs<y,os<1,hs<x-1> > > > > > >, os<1>) { return gen(); }
  template<int y> os<1,hs<1,os<1,Lvvs<y,hline<1> > > > > operator- (os<1,hs<1,os<1,Lvvs<y,os<1> > > > >, os<1>) { return gen(); }
  template<int x, int y> os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > operator- (os<1,hs<x,os<1,Lvvs<y,os<1> > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int y, int p> os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > operator- (os<1,hs<x,os<1,Lvvs<y,os<1,hs<p> > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int y> os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > operator- (os<1,hs<x,os<1,Lvvs<y,os<1,hs<x-1> > > > > >, os<1>) { return gen(); }
  template<int y, int d> Ls<1,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,hline<1> > > > > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,os<1> > > > > > > >, os<1>) { return gen(); }
  template<int x, int y, int d> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1> > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int y, int d, int p> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<p> > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int y, int d> Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > operator- (Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<x-1> > > > > > > > >, os<1>) { return gen(); }
  template<int n, int y, int d> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,hline<1> > > > > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,os<1> > > > > > > > >, os<1>) { return gen(); }
  template<int n, int x, int y, int d> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1> > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int y, int d, int p> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<p> > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x, int y, int d> Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > operator- (Ls<1,vLs<n,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<x-1> > > > > > > > > >, os<1>) { return gen(); }
  template<int n> Ls<n,rectangle<1,0> > operator- (Ls<n,os<1,hs<1,hline<0> > > >, os<1>) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1,hs<x,hline<0> > > >, hs<x-1,os<1> >) { return gen(); }
  template<int n, int x, int p> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1,hs<x,os<2,hs<p> > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int n, int x> Ls<n,rectangle<x,0> > operator- (Ls<n,os<1,hs<x,os<2,hs<x-1> > > > >, os<1>) { return gen(); }

  // Near-bottom-right corner

// Constructing a cuboid with H < D: o---o|&o&&&&o---o&!!o---o

  // Rear-top-left corner
  template<int d> cuboid<0,0,d/2> operator* (os<2,Ls<d,rectangle<0,0> > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator* (os<1,vLs<y,os<1,Ls<d,os<2,Lvvs<y,hline<0> > > > > > >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator* (hs<x,os<2,Ls<d,rectangle<x,0> > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator* (hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > >) { return gen(); }

  template<int d> cuboid<1,0,d/2> operator- (os<1>, os<2,Ls<d,rectangle<1,0> > >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1>, hs<x-1,os<2,Ls<d,rectangle<x,0> > > >) { return gen(); }
  template<int x, int d, int p> cuboid<x,0,d/2> operator- (os<1,hs<p> >, hs<x-p-1,os<2,Ls<d,rectangle<x,0> > > >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1,hs<x-1> >, os<2,Ls<d,rectangle<x,0> > >) { return gen(); }
  template<int y, int d> cuboid<1,y,y+d/2> operator- (os<1>, os<1,vLs<y,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,hline<1> > > > > > > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1>, hs<x-1,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int y, int d, int p> cuboid<x,y,y+d/2> operator- (os<1,hs<p> >, hs<x-p-1,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1,hs<x-1> >, os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > >) { return gen(); }

  // Rear-top-right corner
  cuboid<0,0,1> operator* (os<1>, os<1,Ls<2,rectangle<0,0> > >) { return gen(); }
  template<int x> cuboid<x,0,1> operator* (os<1,hs<x> >, os<1,Ls<2,rectangle<x,0> > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator* (os<1>, vLs<y,os<1,Ls<d,os<2,Lvvs<y,hline<0> > > > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator* (os<1,hs<x> >, vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > >) { return gen(); }

  // vLs
  template<int d> cuboid<0,1,1+d/2> operator| (hline<0>, Ls<1,os<1,Ls<d,os<2,Lvvs<1,hline<0> > > > > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator| (hline<0>, Ls<1,vLs<y-1,os<1,Ls<d,os<2,Lvvs<y,hline<0> > > > > > >) { return gen(); }
  template<int y, int d, int p> cuboid<0,y,y+d/2> operator| (os<2,vLs<p> >, Ls<1,vLs<y-p-1,os<1,Ls<d,os<2,Lvvs<y,hline<0> > > > > > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator| (os<2,vLs<y-1> >, Ls<1,os<1,Ls<d,os<2,Lvvs<y,hline<0> > > > > >) { return gen(); }
  template<int x, int d> cuboid<x,1,1+d/2> operator| (hline<x>, Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<1,hline<x> > > > > > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator| (hline<x>, Ls<1,vLs<y-1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > >) { return gen(); }
  template<int x, int y, int d, int p> cuboid<x,y,y+d/2> operator| (os<1,hs<x,os<1,vLs<p> > > >, Ls<1,vLs<y-p-1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator| (os<1,hs<x,os<1,vLs<y-1> > > >, Ls<1,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > > > >) { return gen(); }

  // Rear-bottom-left corner
  cuboid<0,0,1> operator* (hline<0>, Ls<2,rectangle<0,0> >) { return gen(); }
  template<int x> cuboid<x,0,1> operator* (hline<x>, Ls<2,rectangle<x,0> >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator* (os<2,vLs<y> >, Ls<d,os<2,Lvvs<y,hline<0> > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator* (os<1,hs<x,os<1,vLs<y> > > >, Ls<d,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > >) { return gen(); }

  // Ls
  template<int q> cuboid<0,0,(q+1)/2> operator& (os<1,hline<0> >, Ls<q,rectangle<0,0> >) { return gen(); }
  template<int p, int q> cuboid<0,0,(p+q+1)/2> operator& (os<3,Ls<p> >, Ls<q,rectangle<0,0> >) { return gen(); }
  template<int p> cuboid<0,0,(p+1)/2> operator& (os<3,Ls<p> >, rectangle<0,0>) { return gen(); }
  template<int x, int q> cuboid<x,0,(q+1)/2> operator& (os<1,hs<x,hline<0> > >, Ls<q,rectangle<x,0> >) { return gen(); }
  template<int x, int p, int q> cuboid<x,0,(p+q+1)/2> operator& (os<1,hs<x,os<2,Ls<p> > > >, Ls<q,rectangle<x,0> >) { return gen(); }
  template<int x, int p> cuboid<x,0,(p+1)/2> operator& (os<1,hs<x,os<2,Ls<p> > > >, rectangle<x,0>) { return gen(); }
  template<int y, int q> cuboid<0,y,y+(q+1)/2> operator& (os<2,vLs<y,os<1> > >, Ls<q,os<2,Lvvs<y,hline<0> > > >) { return gen(); }
  template<int y, int p, int q> cuboid<0,y,y+(p+q+1)/2> operator& (os<2,vLs<y,os<1,Ls<p> > > >, Ls<q,os<2,Lvvs<y,hline<0> > > >) { return gen(); }
  template<int y, int p> cuboid<0,y,y+(p+1)/2> operator& (os<2,vLs<y,os<1,Ls<p> > > >, os<2,Lvvs<y,hline<0> > >) { return gen(); }
  template<int x, int y, int q> cuboid<x,y,y+(q+1)/2> operator& (os<1,hs<x,os<1,vLs<y,os<1> > > > >, Ls<q,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > >) { return gen(); }
  template<int x, int y, int p, int q> cuboid<x,y,y+(p+q+1)/2> operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<p> > > > > >, Ls<q,os<1,hs<x,os<1,Lvvs<y,hline<x> > > > > >) { return gen(); }
  template<int x, int y, int p> cuboid<x,y,y+(p+1)/2> operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<p> > > > > >, os<1,hs<x,os<1,Lvvs<y,hline<x> > > > >) { return gen(); }

  template<int p, int q> cuboid<0,0,(p+q+2)/2> operator&& (os<3,Ls<p> >, Ls<q,rectangle<0,0> >) { return gen(); }
  template<int x, int p, int q> cuboid<x,0,(p+q+2)/2> operator&& (os<1,hs<x,os<2,Ls<p> > > >, Ls<q,rectangle<x,0> >) { return gen(); }

  // Near-top-left corner
  cuboid<0,0,1> operator* (os<3,Ls<2> >, os<1,hline<0> >) { return gen(); }
  template<int x> cuboid<x,0,1> operator* (os<1,hs<x,os<2,Ls<2> > > >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator* (os<2,vLs<y,os<1,Ls<d> > > >, os<1,Lvvs<y,hline<0> > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator* (os<1,hs<x,os<1,vLs<y,os<1,Ls<d> > > > > >, hs<x,os<1,Lvvs<y,hline<x> > > >) { return gen(); }

  template<int d> cuboid<1,0,d/2> operator- (os<1,hs<1,os<2,Ls<d,os<1> > > > >, os<1,hline<1> >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1> > > > >, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int d, int p> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<p> > > > > >, hs<x-p-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<x-1> > > > > >, os<1,hline<x> >) { return gen(); }
  template<int y, int d> cuboid<1,y,y+d/2> operator- (os<1,hs<1,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, os<1,Lvvs<y,hline<1> > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1> > > > > > >, hs<x-1,os<1,Lvvs<y,hline<x> > > >) { return gen(); }
  template<int x, int y, int d, int p> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<p> > > > > > > >, hs<x-p-1,os<1,Lvvs<y,hline<x> > > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x-1> > > > > > > >, os<1,Lvvs<y,hline<x> > >) { return gen(); }

  // Near-top-right corner
  template<int d> cuboid<0,0,d/2> operator* (os<3,Ls<d,os<1> > >, hline<0>) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator* (os<1,hs<x,os<2,Ls<d,os<1,hs<x> > > > > >, hline<x>) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator* (os<2,vLs<y,os<1,Ls<d,os<1> > > > >, Lvvs<y,hline<0> >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator* (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x> > > > > > > >, Lvvs<y,hline<x> >) { return gen(); }

  // Ampersands forming part of "Lvvs"
  template<int d> cuboid<0,1,1+d/2> operator& (os<2,vLs<1,os<1,Ls<d,hline<0> > > > >, vs<2,hline<0> >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator& (os<2,vLs<y,os<1,Ls<d,hline<0> > > > >, vs<2,Lvvs<y-1,hline<0> > >) { return gen(); }
  template<int x, int d> cuboid<x,1,1+d/2> operator& (os<1,hs<x,os<1,vLs<1,os<1,Ls<d,hline<x> > > > > > >, vs<2,hline<x> >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator& (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,hline<x> > > > > > >, vs<2,Lvvs<y-1,hline<x> > >) { return gen(); }

  // Pipes forming part of "Lvvs"
  template<int d> cuboid<0,1,1+d/2> operator| (os<2,vLs<1,os<1,Ls<d,os<2,Ls<1> > > > > >, vs<1,hline<0> >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator| (os<2,vLs<y,os<1,Ls<d,os<2,Ls<1> > > > > >, vs<1,Lvvs<y-1,hline<0> > >) { return gen(); }
  template<int y, int d, int p> cuboid<0,y,y+d/2> operator| (os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, vs<1,Lvvs<y-p-1,hline<0> > >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator| (os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<y-1,Ls<1> > > > > > >, vs<1,hline<0> >) { return gen(); }
  template<int x, int d> cuboid<x,1,1+d/2> operator| (os<1,hs<x,os<1,vLs<1,os<1,Ls<d,os<1,hs<x,os<1,Ls<1> > > > > > > > > >, vs<1,hline<x> >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator| (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Ls<1> > > > > > > > > >, vs<1,Lvvs<y-1,hline<x> > >) { return gen(); }
  template<int x, int y, int d, int p> cuboid<x,y,y+d/2> operator| (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > > > >, vs<1,Lvvs<y-p-1,hline<x> > >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator| (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y-1,Ls<1> > > > > > > > > > >, vs<1,hline<x> >) { return gen(); }

  template<int d> cuboid<0,1,1+d/2> operator|| (os<2,vLs<1,os<1,Ls<d,os<2,Ls<1> > > > > >, hline<0>) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator|| (os<2,vLs<y,os<1,Ls<d,os<2,Ls<1> > > > > >, Lvvs<y-1,hline<0> >) { return gen(); }
  template<int y, int d, int p> cuboid<0,y,y+d/2> operator|| (os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<p,Ls<1> > > > > > >, Lvvs<y-p-1,hline<0> >) { return gen(); }
  template<int y, int d> cuboid<0,y,y+d/2> operator|| (os<2,vLs<y,os<1,Ls<d,os<2,Lvvs<y-1,Ls<1> > > > > > >, hline<0>) { return gen(); }

  // Near-bottom-left corner
  cuboid<0,0,1> operator* (os<3,Ls<2,hline<0> > >, os<1>) { return gen(); }
  template<int x> cuboid<x,0,1> operator* (os<1,hs<x,os<2,Ls<2,hline<x> > > > >, hs<x,os<1> >) { return gen(); }

  template<int d> cuboid<1,0,d/2> operator- (os<1,hs<1,os<2,Ls<d,os<1,hs<1,hline<0> > > > > > >, os<1>) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<x,hline<0> > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int d, int p> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<x,os<2,hs<p> > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int d> cuboid<x,0,d/2> operator- (os<1,hs<x,os<2,Ls<d,os<1,hs<x,os<2,hs<x-1> > > > > > > >, os<1>) { return gen(); }
  template<int y, int d> cuboid<1,y,y+d/2> operator- (os<1,hs<1,os<1,vLs<y,os<1,Ls<d,os<1,hs<1,os<1,Lvvs<y,os<1> > > > > > > > > > >, os<1>) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1> > > > > > > > > > >, hs<x-1,os<1> >) { return gen(); }
  template<int x, int y, int d, int p> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<p> > > > > > > > > > > >, hs<x-p-1,os<1> >) { return gen(); }
  template<int x, int y, int d> cuboid<x,y,y+d/2> operator- (os<1,hs<x,os<1,vLs<y,os<1,Ls<d,os<1,hs<x,os<1,Lvvs<y,os<1,hs<x-1> > > > > > > > > > > >, os<1>) { return gen(); }


// Convenience namespaces that can be "using namespace"'d:

  namespace symbols
  {
    using analog_literals::o;
    using analog_literals::oo;
    using analog_literals::ooo;
    using analog_literals::L;
    using analog_literals::LL;
    using analog_literals::oL;
    using analog_literals::Lo;
    using analog_literals::Loo;
  }

  namespace shapes
  {
    using analog_literals::line;
    using analog_literals::rectangle;
    using analog_literals::cuboid;
  }

} // analog_literals

#endif // header guard