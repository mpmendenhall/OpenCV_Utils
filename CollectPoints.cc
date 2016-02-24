/// \file CollectPoints.cc Record selected points in image
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "ClickGetter.hh"
#include "ZoomView.hh"

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

int main(int argc, char** argv) {
    
    if(argc != 2) {
        printf("Usage: CollectPoints <image name>\n");
        return EXIT_FAILURE;
    }
    string imname = argv[1];
    //clicktest(imname);
    
    string outnm = imname+"_points.txt";
    
    ZoomView ZV;
    
    // load image
    Mat src = imread(imname.c_str());
    ZV.setSource(src);
    ZV.updateView();
    
    // Zoom in to ROI
    printf("Select ROI for points collection\n");
    ZV.zoomSelectedRegion();
    ZV.updateView();
    
    // collect click points
    printf("Point selection commands:\n");
    printf("\tLeft-click to add new point;\n");
    printf("\t[Backspace] to remove previous point;\n");
    printf("\t[Enter] to commit points to file and start new goup;\n");
    printf("\t[Esc] to save remaining points and exit.\n");
    FILE* fout = fopen(outnm.c_str(),"w");
    int gp = 0;
    vector< array<double,2> > cpts;
    ZV.myCG.accept = { EVENT_LBUTTONUP, ClickGetter::EVENT_KEYBOARD };
    int pointmark_radius = 4;
    int pointmark_thick = 1;
    while(1) {
        auto c = ZV.myCG.getClick();
        
        if(c.evt == ClickGetter::EVENT_KEYBOARD) {
            
            if(c.flags == 10) {
                printf("Saving group %i with %zu points.\n", gp, cpts.size());
                for(auto& p: cpts) {
                    fprintf(fout, "%i\t%g\t%g\n", gp, p[0], p[1]);
                    //circle(ZV.iview, p, pointmark_radius, CV_RGB(100,100,100), pointmark_thick, CV_AA);
                }
                cpts.clear();
                gp++;
                ZV.updateView();
            } else if(c.flags == 65288) {
                if(cpts.size()) {
                    auto p = cpts.back();
                    printf("Deleting point %i:\t%g\t%g\n", gp, p[0], p[1]);
                    cpts.pop_back();
                    //circle(ZV.iview, p, pointmark_radius, CV_RGB(100,100,100), pointmark_thick, CV_AA);
                    ZV.updateView();
                } else printf("No points left to delete.\n");
            } else if(c.flags == 27) break;
            else printf("Unknown key %i\n", c.flags);
            
        } else if(c.evt == EVENT_LBUTTONUP) {
            auto p = ZV.srcCoords(c.p);
            cpts.push_back(p);
            printf("Adding point %i:\t%g\t%g\n", gp, p[0], p[1]);
            circle(ZV.iview, c.p, pointmark_radius, CV_RGB(0,255,0), pointmark_thick, CV_AA);
            ZV.updateView();
            
        } else printf("unknown event: %i:%i\t%i, %i\n", c.evt, c.flags, c.p.x, c.p.y);
    }
    if(cpts.size()) printf("Saving group %i with %zu points.\n", gp, cpts.size());
    for(auto& p: cpts) fprintf(fout, "%i\t%g\t%g\n", gp, p[0], p[1]);
    printf("Done!\n");
    fclose(fout);
    
    return EXIT_SUCCESS;
}
