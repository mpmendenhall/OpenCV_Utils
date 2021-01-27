/// \file QuadLayers.hh Split Bayer-filtered raw image into 4 OpenCV images

#ifndef QUADLAYERS_HH
#define QUADLAYERS_HH

#include <libraw/libraw.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/// Split Bayer-filtered raw image into 4 OpenCV images
class QuadLayers {
public:
    /// Constructor
    QuadLayers(LibRaw& I);

    /// Darkframe subtraction
    void operator-=(const QuadLayers& QL);

    double lsum[4] = {};    ///< pixels sum in each layer
    cv::Mat_<float> M[4];   ///< image in each layer
};

#endif
