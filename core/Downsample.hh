/// \file Downsample.hh
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#include <opencv2/imgproc/imgproc.hpp>
#include <cassert>
#include <iostream>

using namespace cv;

template<typename T>
void sumSubsample(const Mat_<T>& src, size_t nsamp, size_t nsum, size_t dx, size_t dy, Mat_<float>& dst, double s = 1/256.) {
    assert(src.dims == 2);
    assert(dx < nsamp && dy < nsamp);
    size_t nx = src.cols/(nsamp*nsum);
    size_t ny = src.rows/(nsamp*nsum);
    dst = Mat_<float>(ny, nx);
    
    for(size_t ix=0; ix<nx; ix++) {
        for(size_t iy = 0; iy<ny; iy++) {
            for(size_t iix = 0; iix < nsum; iix++) {
                for(size_t iiy = 0; iiy < nsum; iiy++) {
                    double z = src(dy+(iy*nsum + iiy)*nsamp, dx+(ix*nsum + iix)*nsamp);
                    dst(iy, ix) += z;
                    //if(z > 12) std::cout << z << "\n";
                }
            }
        }
    }
    dst *= s/(nsum*nsum);
}
