#include "voronoi_diagram.h"

cell::cell() : px(0.0f), py(0.0f) {}

cell::cell(float x, float y) : px(x), py(y) {}

voronoi::voronoi(){}

voronoi::voronoi(Mat image ,Mat pixels_nodes, float diagram_scale)
{
	rows = image.rows;
    	cols = image.cols;
	imagep = image;
	pixels_nodes_todo = pixels_nodes;
	scale = diagram_scale;
}
