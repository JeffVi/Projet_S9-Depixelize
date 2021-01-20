#ifndef PROJET_DEPIXELISATION_H
#define PROJET_DEPIXELISATION_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "../src/similarity_graph.h"
#include "../src/voronoi_diagram.h"
#include "../src/spline.h"
#include "../src/optimize.h"

int OpenCv(const wchar_t* arg);
int Union(const wchar_t* arg);
int Voronoi(const wchar_t* arg);
int Splines(const wchar_t* arg);
int Depix(const wchar_t* arg);

#endif
