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
* Function to compute the center of a primary grid triangle 
***********************************************************************/
static inline void calc_tri_centroid(const double coords[3][2],
                                     double       center[2]);

/***********************************************************************
* Function to compute the edge centroids of a primary grid triangle 
***********************************************************************/
static inline void calc_tri_edge_centroid(const double coords[3][2],
                                          double       centroids[3][2]);

/***********************************************************************
* Function to compute the center of a primary grid quad 
***********************************************************************/
static inline void calc_quad_centroid(const double coords[4][2],
                                      double       center[2]);

/***********************************************************************
* Function to compute the edge centroids of a primary grid quad 
***********************************************************************/
static inline void calc_quad_edge_centroid(const double coords[4][2],
                                           double       centroids[4][2]);

/***********************************************************************
* 
***********************************************************************/
static inline void calc_tri_centroid(const double coords[3][2],
                                     double       center[2]) 
{
  center[0] = (coords[0][0] + coords[1][0] + coords[2][0]) / 3.0;
  center[1] = (coords[0][1] + coords[1][1] + coords[2][1]) / 3.0;
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_tri_edge_centroid(const double coords[3][2],
                                          double       centroids[3][2])
{
  centroids[0][0] = 0.5 * ( coords[0][0] + coords[1][0] );
  centroids[0][1] = 0.5 * ( coords[0][1] + coords[1][1] );

  centroids[1][0] = 0.5 * ( coords[1][0] + coords[2][0] );
  centroids[1][1] = 0.5 * ( coords[1][1] + coords[2][1] );

  centroids[2][0] = 0.5 * ( coords[2][0] + coords[0][0] );
  centroids[2][1] = 0.5 * ( coords[2][1] + coords[0][1] );
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_quad_centroid(const double coords[4][2],
                                      double       center[2])
{
  center[0] = 0.25 * ( coords[0][0] + coords[1][0] 
                     + coords[2][0] + coords[3][0] );
  center[1] = 0.25 * ( coords[0][1] + coords[1][1] 
                     + coords[2][1] + coords[3][0] );
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_quad_edge_centroid(const double coords[4][2],
                                           double       centroids[4][2])
{
  centroids[0][0] = 0.5 * ( coords[0][0] + coords[1][0] );
  centroids[0][1] = 0.5 * ( coords[0][1] + coords[1][1] );

  centroids[1][0] = 0.5 * ( coords[1][0] + coords[2][0] );
  centroids[1][1] = 0.5 * ( coords[1][1] + coords[2][1] );

  centroids[2][0] = 0.5 * ( coords[2][0] + coords[3][0] );
  centroids[2][1] = 0.5 * ( coords[2][1] + coords[3][1] );

  centroids[3][0] = 0.5 * ( coords[3][0] + coords[0][0] );
  centroids[3][1] = 0.5 * ( coords[3][1] + coords[0][1] );
}



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
  free(dualgrid->face_nbrs);
  free(dualgrid->face_norms);

  BoundaryList_destroy( dualgrid->boundaries );

  free(dualgrid);

} /* DualGrid_destroy() */


/***********************************************************************
* Function to setup a dualgrid structure from a primary grid
***********************************************************************/
DualGrid *DualGrid_build(DualGrid    *dualgrid, 
                         BoundaryDef *bdry_def,
                         PrimaryGrid *primgrid)
{
  dualgrid->primgrid = primgrid;

  dualgrid->n_elements   = primgrid->n_vertices;
  dualgrid->n_intr_faces = primgrid->n_intr_edges 
                         + primgrid->n_bdry_edges;

  dualgrid->xy        = primgrid->vertex_coords;

  dualgrid->vol        = calloc(dualgrid->n_elements,     sizeof(double)); 
  dualgrid->face_nbrs  = calloc(dualgrid->n_intr_faces, 2*sizeof(int)); 
  dualgrid->face_norms = calloc(dualgrid->n_intr_faces, 2*sizeof(double)); 

  BoundaryList_build(dualgrid->boundaries, bdry_def, primgrid);

  /*--------------------------------------------------------------------
  | Initialize interior dualgrid element connectivity
  --------------------------------------------------------------------*/
  int ne_intr = primgrid->n_intr_edges;
  int ne_bdry = primgrid->n_bdry_edges;
  int i_edge;

  for ( i_edge = 0; i_edge < ne_intr; i_edge++ )
  {
    dualgrid->face_nbrs[i_edge][0] = primgrid->intr_edges[i_edge][0];
    dualgrid->face_nbrs[i_edge][1] = primgrid->intr_edges[i_edge][1];
  }

  for ( i_edge = ne_intr; i_edge < (ne_intr+ne_bdry); i_edge++ )
  {
    dualgrid->face_nbrs[i_edge][0] = primgrid->bdry_edges[i_edge-ne_intr][0];
    dualgrid->face_nbrs[i_edge][1] = primgrid->bdry_edges[i_edge-ne_intr][1];
  }

  /*--------------------------------------------------------------------
  | Compute dualgrid volumes and interface normals
  --------------------------------------------------------------------*/
  int n_tris  = primgrid->n_tris;
  int n_quads = primgrid->n_quads;
  int n_elems = dualgrid->n_elements;
  int n_faces = dualgrid->n_intr_faces;

  int (*tris)[3]  = primgrid->tris;
  int (*quads)[4] = primgrid->quads;

  double (*v_coords)[2]   = primgrid->vertex_coords;
  double  *vol            = dualgrid->vol;
  int    (*face_nbrs)[2]  = dualgrid->face_nbrs;
  double (*face_norms)[2] = dualgrid->face_norms;

  int i_tri, i_quad, i_elem, i_face;


  /* Initialize arrays */
  for ( i_elem = 0; i_elem < n_elems; i_elem++ )
    vol[i_elem] = 0.0;

  for ( i_face = 0; i_face < n_faces; i_face++ )
    face_norms[i_face][0] = face_norms[i_face][1] = 0.0;



  /* Count number of dualgrid faces for each element */
  int *n_elem_faces = calloc(dualgrid->n_elements, sizeof(int)); 
   
  for ( i_elem = 0; i_elem < n_elems; i_elem++ )
    n_elem_faces[i_elem] = 0;

  for ( i_face = 0; i_face < n_faces; i_face++ )
  {
    int p0 = face_nbrs[i_face][0];
    int p1 = face_nbrs[i_face][1];

    ++n_elem_faces[p0];
    ++n_elem_faces[p1];
  }

  /* Count maximum occuring number of adjacent faces of an element */
  int n_max_faces = 0;
  for ( i_elem = 0; i_elem < n_elems; i_elem++ )
    n_max_faces = MAX( n_max_faces, n_elem_faces[i_elem] );

  /* Create connectivity list between elements and adjacents faces */
  int **elem_to_face = malloc( n_elems * sizeof(int*) );

  for ( i_elem = 0; i_elem < n_elems; i_elem++ )
  {
    elem_to_face[i_elem] = malloc( (n_max_faces + 1) * sizeof(int) );
    
    for ( i_face = 0; i_face < n_max_faces + 1; i_face++ )
      elem_to_face[i_elem][i_face] = 0;
  }

  for ( i_face = 0; i_face < n_faces; i_face++ )
  {
    int p0 = face_nbrs[i_face][0];
    int p1 = face_nbrs[i_face][1];

    int off_p0 = elem_to_face[p0][0] + 1;
    int off_p1 = elem_to_face[p1][0] + 1;

    elem_to_face[p0][off_p0] = i_face;
    elem_to_face[p1][off_p1] = i_face;

    ++elem_to_face[p0][0];
    ++elem_to_face[p1][0];
  }





  /* Loop over all triangular elements */
  for ( i_tri = 0; i_tri < n_tris; i_tri++ )
  {
    int *tri = tris[i_tri];

    double tri_coords[3][2] = {
      { v_coords[tri[0]][0], v_coords[tri[0]][1] },
      { v_coords[tri[1]][0], v_coords[tri[1]][1] },
      { v_coords[tri[2]][0], v_coords[tri[2]][1] },
    };

    double tri_centroid[2]      = { 0.0 };
    double edge_centroids[3][2] = { 0.0 };

    calc_tri_centroid(tri_coords, tri_centroid);
    calc_tri_edge_centroid(tri_coords, edge_centroids);

    /* Loop over all triangle edges and compute forward sub-triangles */
    for ( i_edge = 0; i_edge < 3; i_edge++ )
    {
      const double a[2] = {
        edge_centroids[i_edge][0] - tri_coords[i_edge][0],
        edge_centroids[i_edge][1] - tri_coords[i_edge][1],
      };

      const double b[2] = {
        tri_centroid[0] - tri_coords[i_edge][0],
        tri_centroid[1] - tri_coords[i_edge][1],
      };

      /* Sub-triangle area */
      const double area = 0.5 * ( a[0] * b[1] - a[1] * b[0] ) ;

      vol[tri[i_edge]] += area;

      /* Sub-triangle interface normal contribution */
      const double norm[2] = {
        -edge_centroids[i_edge][1] + tri_centroid[1],
         edge_centroids[i_edge][0] - tri_centroid[0],
      };

      /* Obtain face index for current interface */
      int p0 = tri[i_edge];
      int p1 = tri[MOD(i_edge+1,3)];

      if ( p1 < p0 )
        continue;

      int *faces_p0 = elem_to_face[p0];

      int k;

      for ( k = 0; k < faces_p0[0]; k++ )
      {
        i_face = faces_p0[k+1];

        if (face_nbrs[i_face][0] == p1 || face_nbrs[i_face][1] == p1)
          break;
      }

      face_norms[i_face][0] += norm[0];
      face_norms[i_face][1] += norm[1];
    }


  }


  /*--------------------------------------------------------------------
  | Free temporary memory
  --------------------------------------------------------------------*/
  free( n_elem_faces );

  for ( i_elem = 0; i_elem < n_elems; i_elem++ )
    free( elem_to_face[i_elem] );
  free( elem_to_face );


} /* DualGrid_setup() */
