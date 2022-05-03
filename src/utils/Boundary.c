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
* Function to create and initialize a new Boundary structure
***********************************************************************/
Boundary *Boundary_create()
{
  Boundary *boundary = calloc(1, sizeof(Boundary));
  check_mem(boundary);

  boundary->boundaries = NULL;
  boundary->next = NULL;
  boundary->prev = NULL;

  //boundary->name = NULL;

  boundary->n_bdry_points = 0;
  boundary->n_bdry_edges = 0;

  boundary->bdry_points = NULL;
  boundary->bdry_edges = NULL;

  boundary->bdry_norm = NULL;
  boundary->bdry_mflux = NULL;

  boundary->set_dirichlet = NULL;
  boundary->set_neumann = NULL;
  boundary->set_bdry_mflux = NULL;

  return boundary;
error:
  return NULL;

} /* Boundary_create() */

/***********************************************************************
* Function to destroy a Boundary structure
***********************************************************************/
void Boundary_destroy(Boundary *boundary)
{
  free( boundary->bdry_points );
  free( boundary->bdry_edges );

  free( boundary->bdry_norm );
  free( boundary->bdry_mflux );

  free( boundary );

} /* Boundary_destroy() */


/***********************************************************************
* Function to create and initialize a new BoundaryDef structure
***********************************************************************/
BoundaryDef *BoundaryDef_create()
{
  BoundaryDef *bdry_def = calloc(1, sizeof(BoundaryDef));
  check_mem(bdry_def);

  return bdry_def;
error:
  return NULL;

} /* BoundaryDef_create() */

/***********************************************************************
* Function to destroy BoundaryDef structure
***********************************************************************/
void BoundaryDef_destroy(BoundaryDef* bdry_def)
{
  free( bdry_def->bdry_markers );
  free( bdry_def->bdry_types );

  free( bdry_def );

} /* BoundaryDef_destroy() */

/***********************************************************************
* Function to read the boundary definitions from a parameter file
***********************************************************************/
void BoundaryDef_build(void *param_file)
{
} /* BoundaryDef_build() */



/***********************************************************************
* Function to create and initialize a new BoundaryList structure
***********************************************************************/
BoundaryList *BoundaryList_create()
{
  BoundaryList *boundaries = calloc(1, sizeof(BoundaryList));
  check_mem(boundaries);

  boundaries->bdry_def = BoundaryDef_create();
  boundaries->n_boundaries = 0;

  boundaries->start = NULL;
  boundaries->end   = NULL;

  return boundaries;
error:
  return NULL;

} /* BoundaryList_create() */

/***********************************************************************
* Function to create and initialize a new BoundaryList structure
***********************************************************************/
void BoundaryList_destroy(BoundaryList* boundaries)
{
  BoundaryDef_destroy( boundaries->bdry_def );

  Boundary *bdry = boundaries->start;

  while ( bdry )
  {
    Boundary *next = bdry->next;
    Boundary_destroy( bdry );
    bdry = next;
  }

  free(boundaries);

} /* BoundaryList_create() */

/***********************************************************************
* Function to build the BoundaryList structure from a PrimaryGrid
***********************************************************************/
void BoundaryList_build(BoundaryList *boundaries, 
                        BoundaryDef*  bdry_def,
                        PrimaryGrid  *primgrid)
{

  boundaries->n_boundaries = bdry_def->n_bdry_markers;

  int i_bdry = 0;

  int n_vertices = primgrid->n_vertices;

  Boundary *prev_bdry = NULL;
  Boundary *bdry      = NULL;

  /*-------------------------------------------------------------------
  | Use this array to mark all vertices that are adjacent to the 
  | current boundary of interest
  -------------------------------------------------------------------*/
  int *point_ids = calloc(n_vertices, sizeof(int));

  /*-------------------------------------------------------------------
  | Loop over all defined boundaries and collect the associated 
  | edges and boundary elements
  -------------------------------------------------------------------*/
  while ( i_bdry < boundaries->n_boundaries )
  {
    int          marker    = bdry_def->bdry_markers[i_bdry];
    BoundaryType bdry_type = bdry_def->bdry_types[i_bdry];

    bdry = Boundary_create();
    bdry->type = bdry_type;
    bdry->prev = prev_bdry;

    if ( prev_bdry )
      prev_bdry->next = bdry;

    int i_pnt, i_edge;

    /*-----------------------------------------------------------------
    | Initialize array of vertex markers
    -----------------------------------------------------------------*/
    for (i_pnt = 0; i_pnt < n_vertices; i_pnt++)
      point_ids[i_pnt] = 0;

    /*-----------------------------------------------------------------
    | Count the number of boundary edges that are associated to this 
    | boundary marker and obtain the maximum vertex index that is 
    | associated to this boundary
    -----------------------------------------------------------------*/
    int  *bdry_edge_marker = primgrid->bdry_edge_marker;
    int (*bdry_edges)[2]   = primgrid->bdry_edges;
    int   n_marked_edges   = 0;

    for (i_edge = 0; i_edge < primgrid->n_bdry_edges; i_edge++)
    {
      if (bdry_edge_marker[i_edge] != marker)
        continue;

      ++n_marked_edges;

      int p0 = bdry_edges[i_edge][0];
      int p1 = bdry_edges[i_edge][1];

      check( p0 < n_vertices,
      "Boundary edge exceeds maximum number of primary grid vertices.");
      check( p1 < n_vertices,
      "Boundary edge exceeds maximum number of primary grid vertices.");

      ++point_ids[ p0 ];
      ++point_ids[ p1 ];
    }

    /*-----------------------------------------------------------------
    | Count the actual number of boundary vertices
    -----------------------------------------------------------------*/
    int n_bdry_points = 0;

    for ( i_pnt = 0; i_pnt < n_vertices; i_pnt++ ) 
      if ( point_ids[i_pnt] > 0 )
        ++n_bdry_points;

    /*-----------------------------------------------------------------
    | Allocate memory for the current boundary 
    -----------------------------------------------------------------*/
    bdry->n_bdry_edges  = n_marked_edges;
    bdry->n_bdry_points = n_bdry_points;

    bdry->bdry_points = calloc( n_bdry_points, sizeof(int) );
    bdry->bdry_edges  = calloc( n_marked_edges, 2*sizeof(int) );
    bdry->bdry_norm   = calloc( n_bdry_points, 2*sizeof(double) );
    bdry->bdry_mflux  = calloc( n_bdry_points, sizeof(double) );

    /*-----------------------------------------------------------------
    | Init boundary vertex IDs
    -----------------------------------------------------------------*/
    int i_bdry_pnt = 0;
    for ( i_pnt = 0; i_pnt < n_vertices; i_pnt++ ) 
    {
      if ( point_ids[i_pnt] < 1 )
        continue;

      bdry->bdry_points[i_bdry_pnt] = i_pnt;
      ++i_bdry_pnt;
    }

    /*-----------------------------------------------------------------
    | Init boundary edges 
    -----------------------------------------------------------------*/
    int i_bdry_edge = 0;
    for (i_edge = 0; i_edge < primgrid->n_bdry_edges; i_edge++)
    {
      if (bdry_edge_marker[i_edge] != marker)
        continue;

      int p0 = bdry_edges[i_edge][0];
      int p1 = bdry_edges[i_edge][1];

      bdry->bdry_edges[i_bdry_edge][0] = p0;
      bdry->bdry_edges[i_bdry_edge][1] = p1;

      ++i_bdry_edge;
    }

    /*-----------------------------------------------------------------
    | Go to next boundary
    -----------------------------------------------------------------*/
    ++i_bdry;
    prev_bdry = bdry;
  }

  /*-------------------------------------------------------------------
  | Add boundaries to boundary list
  -------------------------------------------------------------------*/
  boundaries->end = bdry;

  while ( bdry->prev )
    bdry = bdry->prev;

  boundaries->start = bdry;


  /*-------------------------------------------------------------------
  | Free memory
  -------------------------------------------------------------------*/
  free( point_ids );


  return;
error:
  return;


} /* BoundaryList_build() */






