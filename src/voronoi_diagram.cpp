#include "voronoi_diagram.h"

cell::cell() : px(0.0f), py(0.0f) {}

cell::cell(float x, float y) : px(x), py(y) {}

int cell::get_id_vertex_origin(int k)
{
	int id;
	
	switch(k)
	{
		// haut droite
		case 1:
		id = id_vertex[1];
		break;
		
		// bas droite	
		case 3:
		id = id_vertex[2];
		break;
		
		// bas gauche
		case 5:
		id = id_vertex[3];
		break;
		
		// haut droite	
		case 7:
		id = id_vertex[0];
		break;
	}
	
	if(id == -1){std::cout<<"cross not solved !"<<std::endl;}
	return id;
}

int cell::get_update_id_vertex_origin(int k)
{
	int id;
	
	switch(k)
	{
		// haut droite
		case 1:
		id = id_vertex[1];
		id_vertex[1] = -1;
		id_vertex[2]++;
		id_vertex[3]++;
		break;
		
		// bas droite	
		case 3:
		id = id_vertex[2];
		id_vertex[2] = -1;
		id_vertex[3]++;
		break;
		
		// bas gauche
		case 5:
		id = id_vertex[3];
		id_vertex[3] = -1;
		break;
		
		// haut droite	
		case 7:
		id = id_vertex[0];
		id_vertex[0] = -1;
		id_vertex[1]++;
		id_vertex[2]++;
		id_vertex[3]++;
		break;
	}
	
	if(id == -1){std::cout<<"vertex already changed !"<<std::endl;}
	return id;
}

voronoi::voronoi(){}

voronoi::voronoi(Mat image ,Mat pixels_nodes, float diagram_scale)
{
	rows = image.rows;
    	cols = image.cols;
	imagep = image;
	pixels_nodes_todo = pixels_nodes.clone();
	scale = diagram_scale;
}

void voronoi::init_cells()
{
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			cell c(i*scale + scale/2 , j*scale + scale/2);
			c.vertex.push_back(Point(j*scale,i*scale)); 		// haut gauche
			c.vertex.push_back(Point((j+1)*scale,i*scale)); 	// haut droite
			c.vertex.push_back(Point((j+1)*scale,(i+1)*scale)); 	// bas droite
			c.vertex.push_back(Point(j*scale,(i+1)*scale)); 	// bas gauche
			c.color = imagep.at<Vec3b>(i,j);
			c.id_vertex.push_back(0);
			c.id_vertex.push_back(1);
			c.id_vertex.push_back(2);
			c.id_vertex.push_back(3);
			cells.push_back(c);
		}
	}
}


void voronoi::compute_vertex()
{
	Mat cells_pixels_nodes = pixels_nodes_todo.clone();
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			
			for(int k=0;k<4;k++)
			{
				if(cells_pixels_nodes.at<int>(i,j,2*k+1) == 1) //seulement les diagonales
				{
					std::vector<int> voisin = get_cell_adj(i,j,2*k+1);
					int vi = voisin[0];
					int vj = voisin[1];
					int vk = voisin[2];
					int vi_adj1 = voisin[3];
					int vj_adj1 = voisin[4];
					int vk_adj1 = voisin[5];
					int vi_adj2 = voisin[6];
					int vj_adj2 = voisin[7];
					int vk_adj2 = voisin[8];
					
					cells_pixels_nodes.at<int>(i,j,2*k+1) = 0;
					cells_pixels_nodes.at<int>(vi,vj,vk) = 0;
					
					//Cellule courante
					cell cell_cour = cells[i*cols + j];
					std::vector<Point> vertex = cell_cour.vertex;
					int vertex_id = cell_cour.get_update_id_vertex_origin(2*k+1);
					Point P1 = Point(cell_cour.py,cell_cour.px);
					
					//Cellule voisine
					cell cell_vois = cells[vi*cols + vj];
					std::vector<Point> vertex_voisin = cell_vois.vertex;
					int vertex_id_voisin = cell_vois.get_update_id_vertex_origin(vk);
					Point P2 = Point(cell_vois.py,cell_vois.px);
					
					//Cellule adjacente 1
					cell cell_adj1 = cells[vi_adj1*cols + vj_adj1];
					std::vector<Point> vertex_voisin_adj1 = cell_adj1.vertex;
					int vertex_id_voisin_adj1 = cell_adj1.get_id_vertex_origin(vk_adj1);
					Point P3 = Point(cell_adj1.py,cell_adj1.px);
					if(vertex_id_voisin_adj1==-1){break;}
					
					//Cellule adjacente 2
					cell cell_adj2 = cells[vi_adj2*cols + vj_adj2];
					std::vector<Point> vertex_voisin_adj2 = cell_adj2.vertex;
					int vertex_id_voisin_adj2 = cell_adj2.get_id_vertex_origin(vk_adj2);
					Point P4 = Point(cell_adj2.py,cell_adj2.px);
					if(vertex_id_voisin_adj2==-1){break;}
					
					//Compute vertex
					Point midpoint = P1 + ((P2 - P1)/2.0f);
					Point vertex1 = midpoint + ((P3 - midpoint)/2.0f);
					Point vertex2 = midpoint + ((P4 - midpoint)/2.0f);
					
					//Update vertex
					vertex[vertex_id] = vertex2;
					vertex.insert(vertex.begin() + vertex_id, vertex1);
					
					vertex_voisin[vertex_id_voisin] = vertex1;
					vertex_voisin.insert(vertex_voisin.begin() + vertex_id_voisin, vertex2);
					
					vertex_voisin_adj1[vertex_id_voisin_adj1] = vertex1;
					vertex_voisin_adj2[vertex_id_voisin_adj2] = vertex2;
					
					//Update cellule
					cell_cour.vertex = vertex;
					cell_vois.vertex = vertex_voisin;
					cell_adj1.vertex = vertex_voisin_adj1;
					cell_adj2.vertex = vertex_voisin_adj2;
					
					cells[i*cols + j] = cell_cour;
					cells[vi*cols + vj] = cell_vois;
					cells[vi_adj1*cols + vj_adj1] = cell_adj1;
					cells[vi_adj2*cols + vj_adj2] = cell_adj2;
				}
			}			
		}
	}
}

std::vector<int> voronoi::get_cell_adj(int i, int j, int k)
{
	//Cellule voisine en diagonale
	int vi;
	int vj;
	int vk;
	
	//Cellule adjacente 1
	int vi_adj1;
	int vj_adj1;
	int vk_adj1;
	
	//Cellule adjacente 2
	int vi_adj2;
	int vj_adj2;
	int vk_adj2;
	
	std::vector<int> voisins;
	
	switch(k)
	{
		case 1:
		vi = i-1;
		vj = j+1;
		vk = 5;
		vi_adj1 = i-1;
		vj_adj1 = j;
		vk_adj1 = 3;
		vi_adj2 = i;
		vj_adj2 = j+1;
		vk_adj2 = 7;
		break;
				
		case 3:
		vi = i+1;
		vj = j+1;
		vk = 7;
		vi_adj1 = i;
		vj_adj1 = j+1;
		vk_adj1 = 5;
		vi_adj2 = i+1;
		vj_adj2 = j;
		vk_adj2 = 1;
		break;
		
		case 5:
		vi = i+1;
		vj = j-1;
		vk = 1;
		vi_adj1 = i+1;
		vj_adj1 = j;
		vk_adj1 = 7;
		vi_adj2 = i;
		vj_adj2 = j-1;
		vk_adj2 = 3;
		break;
				
		case 7:
		vi = i-1;
		vj = j-1;
		vk = 3;
		vi_adj1 = i;
		vj_adj1 = j-1;
		vk_adj1 = 1;
		vi_adj2 = i-1;
		vj_adj2 = j;
		vk_adj2 = 5;
		break;
	}
	
	voisins.push_back(vi);
	voisins.push_back(vj);
	voisins.push_back(vk);
	voisins.push_back(vi_adj1);
	voisins.push_back(vj_adj1);
	voisins.push_back(vk_adj1);
	voisins.push_back(vi_adj2);
	voisins.push_back(vj_adj2);
	voisins.push_back(vk_adj2);
	
	return voisins;
}

Mat voronoi::draw_voronoi()
{
	Mat voro = Mat(rows*ceil(scale),cols*ceil(scale),CV_8UC3,Scalar(0));
	std::vector<cell>::iterator it;
	
	for(it=cells.begin(); it!=cells.end(); it++)
	{
		cell cellule = *it;
		std::vector<Point> vertex_list = cellule.vertex;
		const int npt = vertex_list.size();
		const Point* ppt = &vertex_list[0];
		fillPoly(voro,&ppt,&npt,1,cellule.color);
	}
		
	return voro;
}


void voronoi::polygon_union()
{


	for(int i = 0;i < rows-1; i++)
	{
		for (int j = 0; j < cols-1; j++)
		{
			bool have_adj = false; 

			if (cells[j*i +j].united == false) // On vérifie si la cellule a déjà été uni et si elle a des voisins de couleur similaire
			{
				if (pixels_nodes_todo.at<int>(i, j, 2) == 1) have_adj = true;

				if (pixels_nodes_todo.at<int>(i, j, 3) == 1) have_adj = true;

				if (pixels_nodes_todo.at<int>(i, j, 4) == 1) have_adj = true;

				union_cells.push_back(cell(cells[i, j].px, cells[i, j].py));

				std::vector<std::vector<int>>cells_indice;  // Permet de stocker les indices des cellules composant la grande cellule

				std::vector<int> indices{ i,j };
				cells_indice.push_back(indices);

				while (have_adj) // Boucle permettant de créer l'union des toutes les cellules respectant les conditions
				{
					std::vector<std::vector<int>>::iterator it;
					std::vector<std::vector<int>>adjacent; // On met dans ce vecteur toutes les cellules adjacentes trouvées pour chaque petite cellule

					for (it = cells_indice.begin(); it != cells_indice.end(); it++) // on va parcourir la grosse cellule
					{
						std::vector<int> vec = *it;
						int indice_i = vec[0];
						int indice_j = vec[1];
						bool found = false;

						//Pour chaque cellule contenues dans la grosse cellule on cherche un voisin de couleur similaire non uni à la cellule

						if (pixels_nodes_todo.at<int>(indice_i, indice_j, 2) == 1 && cells[indice_i * indice_j + indice_j+1].united == false) // cellule droite
						{

							std::vector<int> cellule_adj{ indice_i, indice_j +1};
							adjacent.push_back(cellule_adj);

						}

						if (pixels_nodes_todo.at<int>(indice_i, indice_j, 3) == 1 && cells[(indice_i+1) * indice_j + indice_j + 1].united == false && found == false) // cellule diag
						{

							std::vector<int> cellule_adj{ indice_i+1, indice_j + 1 };
							adjacent.push_back(cellule_adj);

						}

						if (pixels_nodes_todo.at<int>(indice_i, indice_j, 4) == 1 && cells[(indice_i + 1) * indice_j + indice_j].united == false && found == false) // cellule bas
						{

							std::vector<int> cellule_adj{ indice_i + 1, indice_j };
							adjacent.push_back(cellule_adj);

						}
						// A partir de maintenant on a toutes les cellules adjacentes a la grosse cellule dans un vecteur
						// Ce vecteur peut contenir plusieur fois la meme cellule ce qui est normal
						// En fonction du nombre de fois qu'il contient une cellule l'union ne se fera pas de la meme façon (3 vertex partages)
						// On va alors unir la permiere cellule qui se repete avec la grosse cellule
						// Si aucune cellule ne se repete alors on va unir la premiere cellule du vecteur
						// A la fin de la booucle on ooublie pas de marquer comme traitee la cellule que l on vient d unir
						// On verifie a la fin de la boucle qu il y a encore des cellules adjacentes a la nouvelle grosse cellule
						// Si oui alors on reboucle si non alors on en a fini avec cette grosse cellule

						//TODO regarder les si des cellules ce repetent et appeler la bonne fonction rajouter la cellule a traiter dans le vecteur cells_indices
						//Avec le nouveau vecteur cell_indice verifier si il reste des cellules adjacentes a traiter


					}
				}
			}
		}	
	}
}


void union_2vertex(int indice_i, int indice_j) 
{
// TODO faire l union de la cellule avec indice_i indice_j avec la grosse cellule
// Pour faire l union on ajoute des vertex aux bons emplacements a la grosse cellule
}


void union_3vertex(int indice_i, int indice_j)
{

}



