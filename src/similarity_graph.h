#ifndef SIMILARITY_GRAPH_H
#define SIMILARITY_GRAPH_H

#include <opencv2/opencv.hpp>

using namespace cv;

class similarity_graph
{
public:

	similarity_graph();
	similarity_graph(int nbrows, int nbcols, Mat image);
	
	int rows;
	int cols;
	Mat pixels_nodes;
	
private:
	

};

#endif
