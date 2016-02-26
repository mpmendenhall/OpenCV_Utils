######################################
# Makefile for OpenCV helper utilities
######################################
# This file was produced under the employ of the United States Government,
# and is consequently in the PUBLIC DOMAIN, free from all provisions of
# US Copyright Law (per USC Title 17, Section 105).
#
# -- Michael P. Mendenhall, 2015

# assure correct shell is used
SHELL = /bin/sh
# apply implicit rules only for listed file types
.SUFFIXES:
.SUFFIXES: .c .cc .cpp .o

# compiler flags
CXXFLAGS += -O3 --std=c++11 -fPIC `pkg-config --cflags opencv` -pedantic -Wall -Wextra
LDFLAGS += -L. -lOCVUtils `pkg-config --libs opencv`

# optional MPMUtils linkage
#ifdef MPMUTILS
#CXXFLAGS += -I${MPMUTILS}/GeneralUtils/
#LDFLAGS += -L${MPMUTILS}/GeneralUtils/ -lMPMGeneralUtils
#endif

VPATH = ./:

objects = BaseTypes.o ClickGetter.o ScaleFilter.o TransformView.o VTransformFilter.o
execs = OCV_Test CollectPoints
all: libOCVUtils.a libOCVUtils.so $(execs)

libOCVUtils.a: $(objects)
	ar rs libOCVUtils.a $(objects)
libOCVUtils.so: $(objects)
	$(CXX) -shared -fPIC -o libOCVUtils.so $(objects) `pkg-config --libs opencv`

# generic rule for everything else .cc
% : %.cc libOCVUtils.a
	$(CXX) $(CXXFLAGS) $< $(objects) $(LDFLAGS) -o $@

# cleanup
.PHONY: clean
clean:
	-rm -f *.o *.a *.so
	-rm -f $(execs)
