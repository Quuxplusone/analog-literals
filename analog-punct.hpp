
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
  template<int p, int q, typename T> Ls<p+q+1,T> operator* (os<p>, os<q,T>) { return gen(); }

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

  template<int n, int q> os<n,hline<q> > operator* (os<n>, hs<q,os<1> >) { return gen(); }
  template<int p, int n, int q> hs<p,os<n,hline<q> > > operator* (hs<p,os<n> >, hs<q,os<1> >) { return gen(); }
  template<int p, int n, int q> vs<p,os<n,hline<q> > > operator* (vs<p,os<n> >, hs<q,os<1> >) { return gen(); }

  template<int y> vs<y,hline<1> > operator- (vs<y,os<1> >, os<1>) { return gen(); }
  template<int y, int p> vs<y,hline<p+1> > operator- (vs<y,os<1,hs<p> > >, os<1>) { return gen(); }
  template<int y, int p, int q> vs<y,hline<p+q+1> > operator- (vs<y,os<1,hs<p> > >, hs<q,os<1> >) { return gen(); }
  template<int y, int q> vs<y,hline<q+1> > operator- (vs<y,os<1> >, hs<q,os<1> >) { return gen(); }

  // o|!!!o
  template<int y> vline<y> operator* (vs<y,os<1> >) { return gen(); }
  vline<1> operator| (os<1>, os<1>) { return gen(); }
  template<int y> vline<y+1> operator| (os<1>, vs<y,os<1> >) { return gen(); }

  template<int x, int y> os<1,hs<x,vline<y> > > operator* (os<1,hs<x> >, vs<y,os<1> >) { return gen(); }

  // Misc constructions
  template<template<int,class> class P, int n, int x> P<n,hs<x,hline<0> > > operator* (P<n,hs<x> >, os<1>) { return gen(); }
  template<template<int,class> class P, int n, int x, typename T> P<n,hs<x,os<2,T> > > operator* (P<n,hs<x> >, os<1,T>) { return gen(); }
  template<template<int,class> class P, int n, int x, typename T> P<n,hs<x,os<1,T> > > operator* (P<n,hs<x> >, T) { return gen(); }
  template<template<int,class> class P, int n, int x, typename T> P<n,hs<x+1,T> > operator- (P<n,nil>, hs<x,T>) { return gen(); }
  template<template<int,class> class P, int n, typename T> P<n,hs<1,T> > operator- (P<n,nil>, T) { return gen(); }
  template<template<int,class> class P, int n, int x, typename T> P<n,hs<x+1,T> > operator- (P<n,hs<x> >, T) { return gen(); }
  template<template<int,class> class P, int n, int p, int q, typename T> P<n,hs<p+q+1,T> > operator- (P<n,hs<p> >, hs<q,T>) { return gen(); }

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

  // Misc constructions
  template<typename T> vLs<1,T> operator! (Ls<1,T>) { return gen(); }
  template<int n, typename T> vLs<n+1,T> operator! (Ls<1,vLs<n,T> >) { return gen(); }

  template<typename T> Lvvs<1,T> operator& (vs<2,T>) { return gen(); }
  template<int n, typename T> Lvvs<n+1,T> operator& (vs<2,Lvvs<n,T> >) { return gen(); }

// Constructing a cuboid with H == D: o---o|&!&|&*o---o&!!&!!&!!o---o
  cuboid<0,0,0> operator* (os<2,rectangle<0,0> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (hs<x,os<2,rectangle<x,0> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (vLs<z,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > > > >) { return gen(); }

  cuboid<1,0,0> operator- (os<1>, os<2,rectangle<1,0> >) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1>, os<1,vLs<z,os<2,hs<1,os<1,Lvvs<z,hline<1> > > > > > >) { return gen(); }
  template<int q> cuboid<q+1,0,0> operator- (os<1>, hs<q,os<3,hs<q+1,os<1,hline<q+1> > > > >) { return gen(); }
  template<int q, int z> cuboid<q+1,z,z> operator- (os<1>, hs<q,os<1,vLs<z,os<2,hs<q+1,os<1,Lvvs<z,hline<q+1> > > > > > > >) { return gen(); }
  template<int p, int q> cuboid<p+q+1,0,0> operator- (os<1,hs<p> >, hs<q,os<3,hs<p+q+1,hline<p+q+1> > > >) { return gen(); }
  template<int p, int q, int z> cuboid<p+q+1,z,z> operator- (os<1,hs<p> >, hs<q,os<1,vLs<z,os<2,hs<p+q+1,os<1,Lvvs<z,hline<p+q+1> > > > > > > >) { return gen(); }
  template<int p> cuboid<p+1,0,0> operator- (os<1,hs<p> >, os<3,hs<p+1,hline<p+1> > >) { return gen(); }
  template<int p, int z> cuboid<p+1,z,z> operator- (os<1,hs<p> >, os<1,vLs<z,os<2,hs<p+1,os<1,Lvvs<z,hline<p+1> > > > > > >) { return gen(); }

  // Rear-top-right corner
  cuboid<0,0,0> operator* (os<1>, os<1,rectangle<0,0> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<1>, vLs<z,os<3,Lvvs<z,hline<0> > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x> >, os<2,hs<x,os<1,hline<x> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x> >, vLs<z,os<2,hs<x,os<1,Lvvs<z,hline<x> > > > > >) { return gen(); }

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
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<1> > >, os<1,hs<x,os<1,hline<x> > > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z> > > >, os<1,hs<x,os<1,Lvvs<z,hline<x> > > > >) { return gen(); }

  // Near-top-left corner
  cuboid<0,0,0> operator* (os<1,hline<0> >, os<1,hline<0> >) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,os<1> > >, os<1,Lvvs<z,hline<0> > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<2> > >, hs<x,os<1,hline<x> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z,os<1> > > > >, hs<x,os<1,Lvvs<z,hline<x> > > >) { return gen(); }

  cuboid<1,0,0> operator- (os<1,hs<1,os<2> > >, os<1,hline<1> >) { return gen(); }
  template<int z> cuboid<1,z,z> operator- (os<1,hs<1,os<1,vLs<z,os<2> > > > >, os<1,Lvvs<z,hline<1> > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3> > >, hs<x-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2> > > > >, hs<x-1,os<1,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int x, int p> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<p> > > >, hs<x-p-1,os<1,hline<x> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<p> > > > > >, hs<x-p-1,os<1,Lvvs<z,hline<x> > > >) { return gen(); }
  template<int x> cuboid<x,0,0> operator- (os<1,hs<x,os<3,hs<x-1> > > >, os<1,hline<x> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator- (os<1,hs<x,os<1,vLs<z,os<2,hs<x-1> > > > > >, os<1,Lvvs<z,hline<x> > >) { return gen(); }

  // Near-top-right corner
  cuboid<0,0,0> operator* (rectangle<0,0>, hline<0>) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,hline<0> > >, Lvvs<z,hline<0> >) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<3,hs<x> > > >, hline<x>) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator* (os<1,hs<x,os<1,vLs<z,os<2,hs<x> > > > > >, Lvvs<z,hline<x> >) { return gen(); }

  template<int z> cuboid<0,z,z> operator& (os<2,vLs<z,hline<0> > >, vs<2,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,hline<x> > > > >, vs<2,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator& (os<2,vLs<z,os<3,Lvvs<p> > > >, vs<2,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p> > > > > > > >, vs<2,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator& (os<2,vLs<z,os<3,Lvvs<z-1> > > >, vs<2,hline<0> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator& (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z-1> > > > > > > >, vs<2,hline<x> >) { return gen(); }

  template<int z> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Ls<1> > > >, vs<1,Lvvs<z-1,hline<0> > >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Ls<1> > > > > > > >, vs<1,Lvvs<z-1,hline<x> > >) { return gen(); }
  template<int z, int p> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Lvvs<p,Ls<1> > > > >, vs<1,Lvvs<z-p-1,hline<0> > >) { return gen(); }
  template<int x, int z, int p> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<p,Ls<1> > > > > > > > >, vs<1,Lvvs<z-p-1,hline<x> > >) { return gen(); }
  template<int z> cuboid<0,z,z> operator| (os<2,vLs<z,os<3,Lvvs<z-1,Ls<1> > > > >, vs<1,hline<0> >) { return gen(); }
  template<int x, int z> cuboid<x,z,z> operator| (os<1,hs<x,os<1,vLs<z,os<2,hs<x,os<1,Lvvs<z-1,Ls<1> > > > > > > > >, vs<1,hline<x> >) { return gen(); }

  // Near-lower-left corner
  cuboid<0,0,0> operator* (os<1,rectangle<0,0> >, os<1>) { return gen(); }
  template<int z> cuboid<0,z,z> operator* (os<2,vLs<z,os<3,Lvvs<z> > > >, os<1>) { return gen(); }
  template<int x> cuboid<x,0,0> operator* (os<1,hs<x,os<3,hs<x,os<1> > > > >, hs<x,os<1> >) { return gen(); }
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