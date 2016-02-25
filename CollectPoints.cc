/// \file CollectPoints.cc Record selected points in image
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "ClickGetter.hh"
#include "ZoomView.hh"
#include "MarkSet.hh"

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
    
    string outnm = imname+"_points.txt";
    
    // load, display image
    Mat src = imread(imname.c_str());
    ZoomView ZV;
    ZV.setSource(src);
    ZV.updateView();
    
    // show instructions
    printf("Point selection commands:\n");
    printf("\tLeft-click to add new point;\n");
    printf("\tRight-click-and-drag to zoom in to selected region.\n");
    printf("\t[u] to unzoom to whole image.\n");
    printf("\t[Backspace] to remove previous point;\n");
    printf("\t[Enter] to commit points to file and start new goup;\n");
    printf("\t[Esc] to save remaining points and exit.\n");
    
    ///////////////////////
    // collect click points
    FILE* fout = fopen(outnm.c_str(),"w");
    int gp = 0;
    vector<DPoint> cpts;                // points in current group
    MarkSet MS;                         // image display annotations
    
    ZV.myCG.accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN, EVENT_RBUTTONUP, ClickGetter::EVENT_KEYBOARD };
    ClickGetter::click prevdown; // previous mouse-down location
    while(1) {
        auto c = ZV.myCG.getClick();
        
        // look for right-button click & drag zoom
        if(c.evt == EVENT_RBUTTONDOWN) { prevdown = c; continue; }
        if(c.evt == EVENT_RBUTTONUP && prevdown.evt == EVENT_RBUTTONDOWN) {
                ZV.zoomViewRegion(Rect(c.p, prevdown.p));
                MS.draw(ZV);
                ZV.updateView();
        }
        prevdown = ClickGetter::click();
        
        if(c.evt == ClickGetter::EVENT_KEYBOARD) {
            
            if(c.flags == 10 || c.flags == 13) {
                
                printf("Saving group %i with %zu points.\n", gp, cpts.size());
                for(auto& p: cpts) fprintf(fout, "%i\t%g\t%g\n", gp, p.x, p.y);
                gp++;
                
                for(auto m: MS.marks) m->setColor(CV_RGB(0,0,255));
                cpts.clear();
                ZV.refresh();
                MS.draw(ZV);
                ZV.updateView();
                
            } else if(c.flags == 127 || c.flags == 65288) {
                
                if(cpts.size()) {
                    auto p = cpts.back();
                    printf("Deleting point %i:\t%g\t%g\n", gp, p.x, p.y);
                    cpts.pop_back();
                    MS.popMark();
                    ZV.refresh();
                    MS.draw(ZV);
                    ZV.updateView();
                } else printf("No points left to delete.\n");
                
            } else if(c.flags == 117) {
                ZV.unzoom();
                MS.draw(ZV);
                ZV.updateView();
            } else if(c.flags == 27) break;
            else printf("Unknown key %i\n", c.flags);
            
        } else if(c.evt == EVENT_LBUTTONUP) {
            auto p = ZV.srcCoords(c.p);
            cpts.push_back(p);
            printf("Adding point %i:\t%g\t%g\n", gp, p.x, p.y);
            MS.addMark(new CircleMark(p,CV_RGB(0,255,0)), &ZV);
            ZV.updateView();
            
        } //else printf("unknown event: %i:%i\t%i, %i\n", c.evt, c.flags, c.p.x, c.p.y);
    }
    if(cpts.size()) printf("Saving group %i with %zu points.\n", gp, cpts.size());
    for(auto& p: cpts) fprintf(fout, "%i\t%g\t%g\n", gp, p.x, p.y);
    printf("Done!\n");
    fclose(fout);
    
    return EXIT_SUCCESS;
}
