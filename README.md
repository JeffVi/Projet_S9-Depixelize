# Projet S9
## Depixelizing
This project is about **depixelizing pixel art** using the [Kopf-Lischinski algorithm](https://johanneskopf.de/publications/pixelart/) and [OpenCV](https://opencv.org/) **C++** library.

## v1.8-beta

### Pixel Art
Draw the **pixel art**.
Compute and draw the **YUV pixel art**.

### Similarity Graph
Compute and draw the **similarity graph** of a pixel art with and without **crossing edges** resolution.

### Voronoi Diagram
Compute and draw the **Voronoi diagram** with cells reflecting the pixel art.
Compute and draw the **Voronoi diagram** with cells reflecting the connections in the **resolved similarity graph**.
Compute and draw the **Voronoi diagram** with **united cells** (max 51).

### B-Splines
Compute and draw the optimized **Spline Curves** (minimized spline curvature).
*Without junctions*

### GUI
Added a **Graphical User Interface** based on [Irrlicht](http://irrlicht.sourceforge.net/) !
