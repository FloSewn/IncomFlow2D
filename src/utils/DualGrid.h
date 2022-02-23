/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef DUALGRID_H
#define DUALGRID_H

#include "PrimaryGrid.h"
#include "Boundary.h"

/***********************************************************************
* DualGrid structure
***********************************************************************/
struct DualGrid;
typedef struct DualGrid 
{
  /* Pointer to PrimaryGrid structure */
  PrimaryGrid *primgrid;

  /* Number of dualgrid elements */
  int n_elements;

  /* Element centroids (primary grid vertex coordinates) */
  double (*xy)[2];

  /* Element volumes  */
  double *vol;

  /* Number of dualgrid faces */
  int n_intr_faces;

  /* Dualgrid face neighbors (primary grid edges) */
  int (*face_nbrs)[2];

  /* Associated face normals betwwen dualgrid elements */
  double (*face_norms)[2]; 


  /* The mesh boundary */
  BoundaryList *boundaries;

} DualGrid;

/***********************************************************************
* Function to create and initialize a new dualgrid structure
***********************************************************************/
DualGrid *DualGrid_create();

/***********************************************************************
* Function to destroy a new dualgrid structure
***********************************************************************/
void DualGrid_destroy(DualGrid* dualgrid);

/***********************************************************************
* Function to setup a dualgrid structure from a primary grid
***********************************************************************/
DualGrid *DualGrid_build(DualGrid* dualgrid, 
                         PrimaryGrid *primgrid);

#endif /* DUALGRID_H */
