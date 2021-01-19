#include "optimize.h"

void optimize_Bsplines(B_splines& bsplines)
{
	std::vector<spline>::iterator it_spline;
	int cpt = 0;
	
	for(it_spline = bsplines.splines.begin(); it_spline != bsplines.splines.end(); it_spline++)
	{
		optimize_spline(*it_spline);
		cpt++;
	}
}

void optimize_spline(spline& spline)
{
	polygon spline_optimized = spline.spline_poly;
	
	int nb_boucles = 5;
	int nb_cand = 5;
	
	for(int k=0; k<nb_boucles; k++)
	{
		for(int id_point_poly=0; id_point_poly<spline.spline_poly.vertex.size(); id_point_poly++)
		{
			int id_point_poly_prev;
			int id_point_poly_next;
			if(id_point_poly==0)
			{
				id_point_poly_prev = spline.spline_poly.vertex.size()-1;
				id_point_poly_next = 1;
			}
			else if(id_point_poly==spline.spline_poly.vertex.size()-1)
			{
				id_point_poly_prev = spline.spline_poly.vertex.size()-2;
				id_point_poly_next = 0;
			}
			else
			{
				id_point_poly_prev = id_point_poly-1;
				id_point_poly_next = id_point_poly+1;
			}
			
			Point p0 = spline_optimized.vertex[id_point_poly_prev];
			Point p1 = spline_optimized.vertex[id_point_poly];
			Point p2 = spline_optimized.vertex[id_point_poly_next];
			Point p_hat = spline.spline_poly.vertex[id_point_poly];
			float E = calc_energy(p0,p1,p2,p_hat);
			for(int k_cand=0; k_cand<nb_cand; k_cand++)
			{
				Point p_cand = teta_cand(p1);
				float E_cand = calc_energy(p0,p_cand,p2,p_hat);
				if(E_cand<E)
				{
					E = E_cand;
					p1 = p_cand;
					spline_optimized.vertex[id_point_poly] = p_cand;
					
					std::cout<<"optimized !"<<std::endl;
					
				}
			}
		}
	}
	spline.spline_poly = spline_optimized;
}

float calc_energy(Point& p0, Point& p1, Point& p2, Point& p_hat)
{
	Point m0 = p0 + ((p1 - p0)/2.0f);
	Point m2 = p1 + ((p2 - p1)/2.0f);
	
	float SE = smoothness_energy(m0, p1, m2, 20);
	float PE = positional_energy(p1, p_hat);
	
	return SE + PE;
}

float curvature(Point& m0, Point& p1, Point& m2, float t)
{
	float w0x = m0.x;
	float w0y = m0.y;
	float w1x = p1.x;
	float w1y = p1.y;
	float w2x = m2.x;
	float w2y = m2.y;
	
	float x_prime = 2*(1-t)*(w1x - w0x) + 2*t*(w2x - w1x);
	float y_prime = 2*(1-t)*(w1y - w0y) + 2*t*(w2y - w1y);
	
	float x_seconde = 2*(w2x - 2*w1x + w0x);
	float y_seconde = 2*(w2y - 2*w1y + w0y);
	
	float k = (x_prime*y_seconde - y_prime*x_seconde)/(pow(pow(x_prime,2)+pow(y_prime,2),3.0f/2.0f));
	
	return k;
}

float smoothness_energy(Point& m0, Point& p1, Point& m2, int nbr_pas)
{
	float t;
	float k;
	float SE = 0;
	
	for(int d=0; d<nbr_pas; d++)
	{
		t = (float)d / (float)nbr_pas;
		k = curvature(m0,p1,m2,t);
		SE +=  k/nbr_pas;
	}
	
	return SE;
}

float positional_energy(Point& m1, Point& m_hat)
{
	float PE = 0;
	double n = norm(m1 - m_hat);
	
	PE = pow(n,4);
	
	return PE;
}

// Pour toi Paulo <3
Point teta_cand(Point& p1)
{
	float r = 0.3f;
	
	float x = rand() % 1000 + 1;
	float y = rand() % 1000 + 1;
	
	x = ((x/1000.0f) - 0.5f)*2*r;
	y = ((y/1000.0f) - 0.5f)*2*r;
	
	Point p_cand = p1 + Point(x,y);
	
	return p_cand;
	
}
