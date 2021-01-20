#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdlib.h>
#include "spline.h"

using namespace cv;

void optimize_Bsplines(B_splines& bsplines);

// Optimisation du spline
void optimize_spline(spline& spline);

// Calcul de l'énergie
float calc_energy(Point& p0, Point& p1, Point& p2, Point& p_hat);

float curvature(Point& m0, Point& p1, Point& m2, float t);

float smoothness_energy(Point& m0, Point& p1, Point& m2, int nbr_pas);

float positional_energy(Point& p1, Point& p_hat);

Point teta_cand(Point& p1);

#endif