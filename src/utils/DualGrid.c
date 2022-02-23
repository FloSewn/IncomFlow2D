/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "icf_utils.h"

#include "Boundary.h"
#include "DualGrid.h"
#include "PrimaryGrid.h"

/***********************************************************************
* Function to create and initialize a new dualgrid structure
***********************************************************************/
DualGrid *DualGrid_create()
{
  DualGrid *dualgrid = calloc(1, sizeof(DualGrid));
  check_mem(dualgrid);

  dualgrid->primgrid = NULL; 

  dualgrid->n_elements   = 0;
  dualgrid->n_intr_faces = 0;

  dualgrid->xy        = NULL;
  dualgrid->face_nbrs = NULL;

  dualgrid->vol        = NULL; 
  dualgrid->face_norms = NULL; 
  dualgrid->boundaries = BoundaryList_create();

  return dualgrid;
error:
  return NULL;

} /* DualGrid_create() */

/***********************************************************************
* Function to destroy a new dualgrid structure
***********************************************************************/
void DualGrid_destroy(DualGrid* dualgrid)
{
  free(dualgrid->vol);
  free(dualgrid->face_norms);

  BoundaryList_destroy( dualgrid->boundaries );

  free(dualgrid);

} /* DualGrid_destroy() */


/***********************************************************************
* Function to setup a dualgrid structure from a primary grid
***********************************************************************/
DualGrid *DualGrid_build(DualGrid* dualgrid, 
                         PrimaryGrid *primgrid)
{
  dualgrid->primgrid = primgrid;

  dualgrid->n_elements   = primgrid->n_vertices;
  dualgrid->n_intr_faces = primgrid->n_intr_edges;

  dualgrid->xy        = primgrid->vertex_coords;
  dualgrid->face_nbrs = primgrid->intr_edges;

  dualgrid->vol        = calloc(dualgrid->n_elements, sizeof(double)); 
  dualgrid->face_norms = calloc(dualgrid->n_intr_faces, 2*sizeof(double)); 


  BoundaryList_build(dualgrid->boundaries, primgrid);


} /* DualGrid_setup() */
