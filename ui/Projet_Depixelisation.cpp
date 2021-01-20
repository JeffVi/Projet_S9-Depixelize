#include "Projet_Depixelisation.h"

using namespace cv;

std::string wchar2string(const wchar_t* str) // Permet de convertir le chemin en wchar_t* en string lu par la fonction imread
{
    std::string mystring;
    while (*str)
        mystring += (char)*str++;
    return  mystring;
}

void enregistrement_image(Mat m, std::string path)
{

    std::stringstream file; // to write the file name 

    path.erase(path.size() - 4, 4);

    Mat frame = m; // Create a object 

    std::cout << path << std::endl;

    file << path << "depix.png" ; // file name

    imwrite(file.str(), frame);
 
}

int Voronoi(const wchar_t* arg)
{

    Mat image;
    std::string filename;
    filename = wchar2string(arg);
    image = imread(filename);

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;

    std::cout << "lignes : " << nbr_lignes << " colonnes : " << nbr_colonnes << std::endl;

    similarity_graph graphe(image);

    graphe.init_graph();
    graphe.compare_YUV();
    graphe.resolu();

    voronoi voro = voronoi(image, graphe.pixels_nodes, 20.0f);
    voro.init_cells();
    Mat img_voro = voro.draw_voronoi();
    imshow("Pixel", img_voro);
    voro.compute_vertex();
    img_voro = voro.draw_voronoi();
    imshow("Voronoi", img_voro);
    enregistrement_image(img_voro, filename);
    voro.draw_cells(img_voro);
    imshow("Cellules", img_voro);
    
    return 0;
}

int Union(const wchar_t* arg)
{
    Mat image;
    std::string filename;
    filename = wchar2string(arg);
    image = imread(filename);

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;

    std::cout << "lignes : " << nbr_lignes << " colonnes : " << nbr_colonnes << std::endl;

    similarity_graph graphe(image);
    graphe.init_graph();
    graphe.compare_YUV();
    graphe.resolu();
    voronoi voro = voronoi(image, graphe.pixels_nodes, 20.0f);
    voro.init_cells();
    Mat img_voro = voro.draw_voronoi();
    voro.compute_vertex();
    img_voro = voro.draw_voronoi();
    voro.draw_cells(img_voro);
    voro.polygon();
    voro.union_poly();
    Mat img_voro_union = voro.draw_cells_union();

    imshow("voronoi fusion cellules", img_voro_union);
    waitKey(0);

    return 0;
}

int OpenCv(const wchar_t * arg)
{
    Mat image;
    std::string filename;
    filename =  wchar2string(arg);
    image = imread(filename);

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;
    std::cout << "lignes : " << nbr_lignes << " colonnes : " << nbr_colonnes << std::endl;
    similarity_graph graphe(image);
    graphe.init_graph();
    graphe.compare_YUV();
    imshow("nodes avant resolution", graphe.draw_nodes(21));
    waitKey(0);
    graphe.resolu();
    imshow("nodes apres resolution", graphe.draw_nodes(21));
    
    return 0;
}

int Splines(const wchar_t* arg)
{
    Mat image;
    std::string filename;
    filename = wchar2string(arg);
    image = imread(filename);

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;

    std::cout << "lignes : " << nbr_lignes << " colonnes : " << nbr_colonnes << std::endl;

    similarity_graph graphe(image);
    graphe.init_graph();
    graphe.compare_YUV();
    graphe.resolu();
    voronoi voro = voronoi(image, graphe.pixels_nodes, 20.0f);
    voro.init_cells();
    Mat img_voro = voro.draw_voronoi();
    voro.compute_vertex();
    img_voro = voro.draw_voronoi();
    voro.draw_cells(img_voro);
    voro.polygon();
    voro.union_poly();
    Mat img_voro_union = voro.draw_cells_union();
    B_splines bsplines = B_splines(voro);
    bsplines.find_holes_splines();

    for (int m = 0; m < bsplines.splines.size() - 1; m++)
    {
        bsplines.splines[m].calculate_spline(img_voro_union, bsplines.splines[m].spline_poly);
    }
    imshow("splines", img_voro_union);

    return 0;
}

int Depix(const wchar_t* arg)
{
    Mat image;
    std::string filename;
    filename = wchar2string(arg);
    image = imread(filename);

    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;

    std::cout << "lignes : " << nbr_lignes << " colonnes : " << nbr_colonnes << std::endl;

    similarity_graph graphe(image);
    graphe.init_graph();
    graphe.compare_YUV();
    graphe.resolu();
    voronoi voro = voronoi(image, graphe.pixels_nodes, 20.0f);
    voro.init_cells();
    Mat img_voro = voro.draw_voronoi();
    imshow("Initial", img_voro);
    voro.compute_vertex();
    img_voro = voro.draw_voronoi();
    voro.draw_cells(img_voro);

    voro.polygon();
    voro.union_poly();
    Mat img_voro_union = voro.draw_cells_union();
    B_splines bsplines = B_splines(voro);
    bsplines.find_holes_splines();

    optimize_Bsplines(bsplines);

    Mat drawed_splines = Mat(voro.rows * ceil(voro.scale), voro.cols * ceil(voro.scale), CV_8UC3, Scalar(255, 255, 255));

    Mat drawed_spline = bsplines.color_spline(drawed_splines);

    imshow("Resultat final", drawed_spline);
    enregistrement_image(drawed_spline, filename);
    

    return 0;
}
