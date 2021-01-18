#ifndef SPLINE_H
#define SPLINE_H

#include <opencv2/opencv.hpp>
#include "voronoi_diagram.h"

using namespace cv;

class polygon
{
public:
	
	polygon();
	
	std::vector<Point> vertex;
	
};

class spline
{
public:
	
	spline();
	spline(std::vector<Point> vertex_list);
	
	std::vector<polygon> holes;
	polygon spline_poly;
	Scalar color;
	
	void find_holes();
	Mat draw_spline(float scale, int rows, int cols);
	
private:

	std::vector<Point>::iterator it_p1;
	std::vector<Point>::iterator it_p2;
	std::vector<Point> vertex;
	
	bool find_edge(std::vector<Point>::iterator& it_1, std::vector<Point>::iterator& it_2);
};

class B_splines
{
public:
	
	B_splines();
	B_splines(voronoi& voronoi_diagram);
	
	std::vector<spline> splines;
	
	void find_holes_splines();
	void print_spline(int id_spline);
	
};

#endif
