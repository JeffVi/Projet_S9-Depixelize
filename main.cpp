#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "src/similarity_graph.h"
#include "src/voronoi_diagram.h"
#include "src/spline.h"
#include "src/optimize.h"

using namespace cv;

int main()
{
    Mat image;
    image = imread("img/smw_boo_input.png");

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    imshow("Pixel art", image);
    
    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;
    
    std::cout<<"lignes : "<<nbr_lignes<<" colonnes : "<<nbr_colonnes<<std::endl;
    
    waitKey(0);
    
    similarity_graph graphe(image);
    imshow("YUV", graphe.image_YUV);
    
    waitKey(0);
    
    graphe.init_graph();
    graphe.compare_YUV();
    
    imshow("nodes avant resolution", graphe.draw_nodes(21));
    waitKey(0);
    
    graphe.resolu();
    
    imshow("nodes apres resolution", graphe.draw_nodes(21));
    waitKey(0);
    
    voronoi voro = voronoi(image ,graphe.pixels_nodes, 20.0f);
    voro.init_cells();
    Mat img_voro = voro.draw_voronoi();
    
    imshow("voronoi pixel", img_voro);
    waitKey(0);
    
    voro.compute_vertex();
    img_voro = voro.draw_voronoi();
    
    imshow("voronoi cellules", img_voro);
    waitKey(0);
    
    voro.draw_cells(img_voro);
    
    imshow("voronoi contours cellules", img_voro);
    waitKey(0);
    
    voro.polygon();
    voro.union_poly();
    Mat img_voro_union = voro.draw_cells_union();
    
    imshow("voronoi union cellules", img_voro_union);
    waitKey(0);
    
    B_splines bsplines = B_splines(voro);
    bsplines.find_holes_splines();
    
    optimize_Bsplines(bsplines);
    
    Mat drawed_splines = Mat(voro.rows*ceil(voro.scale),voro.cols*ceil(voro.scale),CV_8UC3,Scalar(255,255,255));
   
    Mat drawed_spline = bsplines.color_spline(drawed_splines);
    
    imshow("spline", drawed_spline);
    waitKey(0);
    

    return 0;
}
