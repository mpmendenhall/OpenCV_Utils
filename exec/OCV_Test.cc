/// \file OCV_Test.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015


// dcraw -D -T -4 IMG_4665.CR4

#include "ClickGetter.hh"
#include "Downsample.hh"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
using std::string;

using namespace std;
using namespace cv;

// load blue channel, downsampled in 3x3 block sums
Mat_<float> load_blue_downsampled(const string& fname) {
    cout << "Loading file...\n";
    Mat src = imread(fname.c_str(), IMREAD_ANYDEPTH | IMREAD_GRAYSCALE);
    cout << "Loaded data of type " << src.type() << "\n";
    
    // downsampled floating point image; scale to display nicely.
    Mat_<float> resamp;
    // blue = 1,1
    // red = 0,0
    sumSubsample<uint16_t>(src, 2, 3, 1, 1, resamp, 1./(1<<10));
    return resamp;
}

// load downsampled blue with dark frame subtraction
Mat_<float> load_darkframed(const string& fLight, const string& fDark = "") {
    Mat_<float> mLight = load_blue_downsampled(fLight);
    if(fDark=="") {
        double minval, maxval;
        minMaxLoc(mLight, &minval, &maxval);
        cout << "Image range " << minval << " to " << maxval << "\n";
        mLight -= minval;
    } else {
        Mat_<float> mDark = load_blue_downsampled(fDark);
        mLight -= mDark;
    }
    return mLight;
}

int main(int, char**) {
    // display window with mouse click tracking
    const char* window_name = "OpenCV image";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    ClickGetter CG;
    setMouseCallback(window_name, clickGetterCallback, &CG);
    
    // 4667: small bottle, unyellowed scintillator
    // 4689: big bottle
    // 4693: big bottle, pre-bubble
    // 4697: after 5min bubble
    // 4701:      +5m
    // 4709,10,12,13: +15m
    // 4715:      +115m
    // 4746: after sitting overnight
    // 4750: +day of bubbling
    string basedir = "/Users/michael/Desktop/BubbleScans/";
    //string basedir = "/home/mpmendenhall/Documents/FaNS/20151110_Scintillator_Bubbling/BubbleScans/";
    string fLightDF = "IMG_4693.tiff";
    string fLight = "IMG_4694.tiff";
    string fSample = "IMG_4695.tiff";
    string fSampleDF = "IMG_4696.tiff";
    
    // load sample image as greyscale
    Mat_<float> resamp = load_darkframed(basedir+fSample, basedir+fSampleDF);
    imwrite((basedir+fSample+"_sample.jpg").c_str(), resamp*120);
    Mat_<float> illum = load_darkframed(basedir+fLight, basedir+fLightDF);
    imwrite((basedir+fSample+"_illum.jpg").c_str(), illum*120);
    
    resamp /= illum;
    
    
    // optional rotate
    //transpose(resamp,resamp);
    //flip(resamp, resamp, 0);
    
    /// ratio image
    imwrite((basedir+fSample+"_Ratio.jpg").c_str(), resamp*256.);
    
    // identify sub-region to sum
    imshow(window_name, resamp);
    
    printf("Click bottle edge near 100...\n");
    auto c0 = CG.getClick(); // "100" point
    printf("Click bottle edge near 600...\n");
    auto c1 = CG.getClick(); // "600" point
    // spacing between 100ml intervals on 1l bottle
    double mm_per_100 = 70./5.;
    // calculate pixels conversion
    double pixels_per_mm = (c0.p.y-c1.p.y)/(5*mm_per_100);
    printf("Scale: %g pixels per mm\n", pixels_per_mm);
    
    Rect wholeimg(0,0, resamp.cols, resamp.rows);
    Rect sumslice(0, c0.p.y-100*pixels_per_mm, resamp.cols, 100*pixels_per_mm);
    Rect whitebox(c0.p.x-28*pixels_per_mm, c0.p.y-100*pixels_per_mm, 24*pixels_per_mm, 100*pixels_per_mm);
    Rect greybox(c0.p.x+30*pixels_per_mm, c0.p.y-100*pixels_per_mm, 60*pixels_per_mm, 100*pixels_per_mm);
    
    double whiteratio = mean(resamp(whitebox))[0];
    printf("Illumination ratio = %g\n", whiteratio);
    resamp /= whiteratio;
    double greylevel = mean(resamp(greybox))[0];
    printf("Average transmission = %g\n", greylevel);
    
    // show sampled areas
    if(false) {
        resamp *= 0.95;
        resamp(whitebox) = 0;
        resamp(greybox) = 1;
        imshow(window_name, resamp);
        waitKey(0);
        return EXIT_SUCCESS;
    }
    
    // column sum average
    Mat subregion = resamp(sumslice);
    bool yproject = false;
    Mat colSum;
    reduce(subregion, colSum, yproject, REDUCE_AVG);
    
    // output average slice to file
    string outname = basedir+fSample+"_slice.txt";
    ofstream fout(outname);
    fout << "# " << fLightDF << "\t" << fLight << "\t" << fSample << "\t" << fSampleDF << "\n";
    fout << "# white normalization " << whiteratio << "\taverage transmission " << greylevel << "\n";
    fout << "# scale " << pixels_per_mm << " pixels/mm\n";
    fout << "# x [mm]\tT\n";
    int px = 0;
    for(auto it = colSum.begin<float>(); it != colSum.end<float>(); it++)
        fout << ((px++)-c0.p.x)/pixels_per_mm << "\t" << *it << "\n";
    fout.close();
    printf("Output to %s\n", outname.c_str());
    
    // stretch to 200 y lines for display
    Mat linestretch;
    if(yproject) repeat(colSum, 1, 100, linestretch);
    else repeat(colSum, 100, 1, linestretch);
    imshow(window_name, linestretch);
    waitKey(0);
    
    return EXIT_SUCCESS;
}
