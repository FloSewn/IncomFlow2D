#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "icf_utils.h"
#include "PrimaryGrid.h"

/***********************************************************************
* Function to create and initialize a new primary grid structure
***********************************************************************/
PrimaryGrid *PrimaryGrid_create()
{
  PrimaryGrid *prim_grid = calloc(1, sizeof(PrimaryGrid));
  check_mem(prim_grid);


  prim_grid->n_vertices   = 0;
  prim_grid->n_tris       = 0;
  prim_grid->n_quads      = 0;
  prim_grid->n_intr_edges = 0;
  prim_grid->n_bdry_edges = 0;

  prim_grid->vertex_coords = NULL;

  prim_grid->tris = NULL;
  prim_grid->quads = NULL;

  prim_grid->tri_neighbors = NULL;
  prim_grid->quad_neighbors = NULL;

  prim_grid->intr_edges = NULL;
  prim_grid->bdry_edges = NULL;

  prim_grid->bdry_edge_marker = NULL;


  return prim_grid;
error:
  return NULL;

} /* PrimaryGrid_create() */

/***********************************************************************
* Function to destroy a primary grid structure
***********************************************************************/
int PrimaryGrid_destroy(PrimaryGrid *prim_grid)
{

  free(prim_grid->vertex_coords);
  free(prim_grid->tris);
  free(prim_grid->quads);
  free(prim_grid->tri_neighbors);
  free(prim_grid->quad_neighbors);
  free(prim_grid->intr_edges);
  free(prim_grid->bdry_edges);
  free(prim_grid->bdry_edge_marker);

  free(prim_grid);

  return ICF_SUCCESS;

} /* PrimaryGrid_destroy() */
