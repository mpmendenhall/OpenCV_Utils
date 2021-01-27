/// \file QuadLayers.cc

#include "QuadLayers.hh"
#include <stdio.h>

QuadLayers::QuadLayers(LibRaw& I) {
    auto nx = I.imgdata.sizes.raw_width;
    auto ny = I.imgdata.sizes.raw_height;

    printf("Image raw size: %d x %d\n", nx, ny);
    I.unpack();

    for(auto dx: {0,1}) {
        for(auto dy: {0,1}) {
            auto& m = M[dx + 2*dy] = cv::Mat_<float>(ny/2, nx/2);
            auto& s = lsum[dx + 2*dy] = 0;
            for(size_t x = 0; x < nx/2; ++x) {
                for(size_t y = 0; y < ny/2; ++y) {
                    auto z = I.imgdata.rawdata.raw_image[(2*x + dx) + (2*y + dy)*nx];
                    m[y][x] = z;
                    s += z;
                }
            }
        }
    }
}

void QuadLayers::operator-=(const QuadLayers& QL) {
    for(auto i: {0,1,2,3}) {
        M[i] -= QL.M[i];
        lsum[i] -= QL.lsum[i];
        printf("Component %d: %g\n", i, lsum[i]);
    }
}
