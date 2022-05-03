/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef PRIMARYGRID_H
#define PRIMARYGRID_H

/***********************************************************************
* Primary grid structure
***********************************************************************/
struct PrimaryGrid;
typedef struct PrimaryGrid {

  int n_vertices;

  int n_tris;
  int n_quads;

  int n_intr_edges;
  int n_bdry_edges;

  double (*vertex_coords)[2];

  int (*tris)[3];
  int (*quads)[4];

  int (*tri_neighbors)[3];
  int (*quad_neighbors)[4];

  int (*intr_edges)[2];
  int (*bdry_edges)[2];

  int (*intr_edge_nbrs)[2];
  int  *bdry_edge_nbrs;

  int  *bdry_edge_marker; 



} PrimaryGrid;

/***********************************************************************
* Function to create and initialize a new primary grid structure
***********************************************************************/
PrimaryGrid *PrimaryGrid_create();

/***********************************************************************
* Function to destroy a primary grid structure
***********************************************************************/
int PrimaryGrid_destroy(PrimaryGrid *primary_grid);

#endif /* PRIMARYGRID_H */
