/// \file BaseTypes.hh A few base type definitions
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef BASETYPES_HH
#define BASETYPES_HH

#include <opencv2/highgui/highgui.hpp>
using namespace cv;
#include <cassert>
#include <cmath>
#include <vector>
using std::vector;

typedef Point_<double> DPoint;
typedef Rect_<double> DRect;
typedef Size_<double> DSize;

/// Determine dimensions fitting within targ closest to aspect ratio of s
Size fitAspect(Size s, Size targ);
/// Determine dimensions fitting within targ closest to aspect ratio of s
Size fitAspect(DSize s, Size targ);
/// Expand rectangle R to match target aspect ratio
DRect expandAspect(DRect R, Size targ);
/// Calculate bounding rectangle for points list
DRect boundingRect(const vector<DPoint>& v);

#endif
