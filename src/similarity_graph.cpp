#include "similarity_graph.h"

similarity_graph::similarity_graph(){}

similarity_graph::similarity_graph(Mat image)
{
	rows = image.rows;
    	cols = image.cols;
	
	int sizes[3];
	sizes[0] = rows;
	sizes[1] = cols;
	sizes[2] = 8;
	
	Mat m(3,sizes,CV_8UC1,Scalar(0));
	pixels_nodes = m;
	
	cvtColor(image,image_YUV,COLOR_BGR2YUV);
}

void similarity_graph::init_graph()
{
	//Connexion pour les pixels ayant 8 voisins.
	for(int i=1;i<rows-1;i++)
	{
		for(int j=1;j<cols-1;j++)
		{
			for(int k=0;k<8;k++)
			{
				pixels_nodes.at<int>(i,j,0) = 1;
			}
		}
	}
	
	//Connexion pour les pixels des bords.
	for(int j=1;j<cols-2;j++)
	{
		//Haut
		pixels_nodes.at<int>(0,j,2) = 1;
		pixels_nodes.at<int>(0,j,3) = 1;
		pixels_nodes.at<int>(0,j,4) = 1;
		pixels_nodes.at<int>(0,j,5) = 1;
		pixels_nodes.at<int>(0,j,6) = 1;
		
		//Bas
		pixels_nodes.at<int>(rows-1,j,6) = 1;
		pixels_nodes.at<int>(rows-1,j,7) = 1;
		pixels_nodes.at<int>(rows-1,j,0) = 1;
		pixels_nodes.at<int>(rows-1,j,1) = 1;
		pixels_nodes.at<int>(rows-1,j,2) = 1;
	}
	
	for(int i=1;i<rows-2;i++)
	{
		//Droite
		pixels_nodes.at<int>(i,cols-1,0) = 1;
		pixels_nodes.at<int>(i,cols-1,7) = 1;
		pixels_nodes.at<int>(i,cols-1,6) = 1;
		pixels_nodes.at<int>(i,cols-1,5) = 1;
		pixels_nodes.at<int>(i,cols-1,4) = 1;
		
		//Gauche
		pixels_nodes.at<int>(i,0,0) = 1;
		pixels_nodes.at<int>(i,0,1) = 1;
		pixels_nodes.at<int>(i,0,2) = 1;
		pixels_nodes.at<int>(i,0,3) = 1;
		pixels_nodes.at<int>(i,0,4) = 1;
	}
	
	//Connexion pour les pixels des coins.
	pixels_nodes.at<int>(0,0,2) = 1;
	pixels_nodes.at<int>(0,0,3) = 1;
	pixels_nodes.at<int>(0,0,4) = 1;
	
	pixels_nodes.at<int>(rows-1,0,0) = 1;
	pixels_nodes.at<int>(rows-1,0,1) = 1;
	pixels_nodes.at<int>(rows-1,0,2) = 1;
	
	pixels_nodes.at<int>(0,cols-1,4) = 1;
	pixels_nodes.at<int>(0,cols-1,5) = 1;
	pixels_nodes.at<int>(0,cols-1,6) = 1;
	
	pixels_nodes.at<int>(rows-1,cols-1,0) = 1;
	pixels_nodes.at<int>(rows-1,cols-1,7) = 1;
	pixels_nodes.at<int>(rows-1,cols-1,6) = 1;
	
}
