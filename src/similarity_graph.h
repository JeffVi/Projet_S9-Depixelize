#ifndef SIMILARITY_GRAPH_H
#define SIMILARITY_GRAPH_H

#include <opencv2/opencv.hpp>

using namespace cv;

class similarity_graph
{
public:

	similarity_graph();
	similarity_graph(Mat image);
	
	Mat pixels_nodes;
	Mat image_YUV;
	
	void init_graph();
	
private:
	
	int rows;
	int cols;

};

#endif
