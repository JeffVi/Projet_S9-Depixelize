#include "similarity_graph.h"

similarity_graph::similarity_graph(){}

similarity_graph::similarity_graph(int nbrows, int nbcols, Mat image)
{
	int sizes[3];
	sizes[0] = nbrows;
	sizes[1] = nbcols;
	sizes[2] = 8;
	
	Mat m(3,sizes,CV_8UC1,Scalar(0));
	
	pixels_nodes = m;
}
