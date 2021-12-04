#ifndef DEFS_H
#define DEFS_H

#define _USE_MATH_DEFINES // we want M_PI, etc. to be defined

#include <cmath>
#include <cassert>
#include <cstdlib>

// inline pass-throughs to various basic math functions
// written in this style to allow for easy substitution with more efficient versions
inline double SINE_FUNCTION (double x) { return std::sin(x); }
inline double COSINE_FUNCTION (double x) { return std::cos(x); }
inline double SQRT_FUNCTION (double x) { return std::sqrt(x); }

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p) = NULL; } }
#endif

/********** CAMP Internals Debug **********/

#define CAMP_DEBUG

#ifdef CAMP_DEBUG
	#include <iostream>		
	#define DBG_OUT(a) { std::cerr.flags(std::ios_base::scientific); std::cerr << (a) << std::endl; }
#endif

#ifndef CAMP_DEBUG
	#define DBG_OUT(a) { }
#endif

#endif
