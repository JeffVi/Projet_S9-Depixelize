#ifndef VORONOI_DIAGRAM_H
#define VORONOI_DIAGRAM_H

#include <opencv2/opencv.hpp>

using namespace cv;

class cell
{
public:
	
	cell();
	cell(float x, float y);
	
	float px;
	float py;
	
private:
	
	std::vector<Point> vertex;
	
};

class voronoi
{
public:

	voronoi();
	voronoi(Mat image ,Mat pixels_nodes);
	
	void init_cells();
	void compute_vertex();
	
	
private:
	
	int rows;
	int cols;
	Mat imagep;
	Mat pixels_nodes_todo;
	std::vector<cell> cells;

};

#endif
