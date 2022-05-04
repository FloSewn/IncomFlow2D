#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dbg.h"
#include "icf_utils.h"
#include "MeshReader.h"
#include "PrimaryGrid.h"
#include "DualGrid.h"

static const char *test_grid = "/datadisk/Code/C-Code/SimpleSolver/input/grid/TestGrid.dat";

/*********************************************************************
* Test creation / destruction of DualGrid structure 
*********************************************************************/
int test_DualGrid_create_destroy()
{
  DualGrid *dualgrid = DualGrid_create();

  DualGrid_destroy( dualgrid );

  return ICF_SUCCESS;

} /* test_DualGrid_create_destroy() */

/*********************************************************************
* Test building of dualgrid boundaries 
*********************************************************************/
int test_DualGrid_build_boundaries()
{

  MeshReader  *mesh_reader = MeshReader_create( test_grid );
  PrimaryGrid *primgrid    = PrimaryGrid_create();
  DualGrid    *dualgrid    = DualGrid_create();

  MeshReader_read_primgrid( mesh_reader, primgrid );

  /*------------------------------------------------------------------
  | Manually set boundary definition 
  | -> This will be passed by the input parameter file
  ------------------------------------------------------------------*/
  BoundaryDef* bdry_def = dualgrid->boundaries->bdry_def;
  bdry_def->n_bdry_markers = 4;
  bdry_def->bdry_markers = calloc(4, sizeof(int));
  bdry_def->bdry_markers[0] = 1; // INLET
  bdry_def->bdry_markers[1] = 2; // DOMAIN WALL
  bdry_def->bdry_markers[2] = 3; // OUTLET
  bdry_def->bdry_markers[3] = 4; // RECTANGLE WALL

  bdry_def->bdry_types = calloc(4, sizeof(BoundaryType));
  bdry_def->bdry_types[0] = INLET;
  bdry_def->bdry_types[1] = WALL;
  bdry_def->bdry_types[2] = OUTLET;
  bdry_def->bdry_types[3] = WALL;
  
  /*------------------------------------------------------------------
  | Build the dualgrid 
  ------------------------------------------------------------------*/
  DualGrid_build(dualgrid, bdry_def, primgrid);

  /*------------------------------------------------------------------
  | Test the boundary nodes
  ------------------------------------------------------------------*/
  Boundary *bdry; 

  check(dualgrid->boundaries->n_boundaries == 4,
      "Wrong number of dualgrid boundaries");

  int i_bdry = 0;

  for ( bdry = dualgrid->boundaries->start;
        bdry != NULL;
        bdry = bdry->next )
  {
    if ( i_bdry == 0 )
    {
      check(bdry->type == INLET, "Wrong boundary type");
      check(bdry->n_bdry_points == 5, "Wrong number of boundary points");
      check(bdry->n_bdry_edges == 4, "Wrong number of boundary points");

      /* Boundary vertices are ordered in ascencing index order */
      check(bdry->bdry_points[0] == 0, "Wrong boundary point ID");
      check(bdry->bdry_points[1] == 1, "Wrong boundary point ID");
      check(bdry->bdry_points[2] == 2, "Wrong boundary point ID");
      check(bdry->bdry_points[3] == 3, "Wrong boundary point ID");
      check(bdry->bdry_points[4] == 4, "Wrong boundary point ID");

      /* Boundary edges are ordered similar to input mesh format */
      check(bdry->bdry_edges[0][0] == 0, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[0][1] == 1, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][0] == 1, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][1] == 2, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][0] == 2, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][1] == 3, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][0] == 3, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][1] == 4, "Wrong boundary edge point ID");
    }

    if ( i_bdry == 1 )
    {
      check(bdry->type == WALL, "Wrong boundary type");
      check(bdry->n_bdry_points == 5, "Wrong number of boundary points");
      check(bdry->n_bdry_edges == 4, "Wrong number of boundary points");

      /* Boundary vertices are ordered in ascencing index order */
      check(bdry->bdry_points[0] == 4, "Wrong boundary point ID");
      check(bdry->bdry_points[1] == 5, "Wrong boundary point ID");
      check(bdry->bdry_points[2] == 6, "Wrong boundary point ID");
      check(bdry->bdry_points[3] == 7, "Wrong boundary point ID");
      check(bdry->bdry_points[4] == 8, "Wrong boundary point ID");

      /* Boundary edges are ordered similar to input mesh format */
      check(bdry->bdry_edges[0][0] == 4, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[0][1] == 5, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][0] == 5, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][1] == 6, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][0] == 6, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][1] == 7, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][0] == 7, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][1] == 8, "Wrong boundary edge point ID");
    }

    if ( i_bdry == 2 )
    {
      check(bdry->type == OUTLET, "Wrong boundary type");
      check(bdry->n_bdry_points == 5, "Wrong number of boundary points");
      check(bdry->n_bdry_edges == 4, "Wrong number of boundary points");

      /* Boundary vertices are ordered in ascencing index order */
      check(bdry->bdry_points[0] == 8, "Wrong boundary point ID");
      check(bdry->bdry_points[1] == 9, "Wrong boundary point ID");
      check(bdry->bdry_points[2] == 10, "Wrong boundary point ID");
      check(bdry->bdry_points[3] == 11, "Wrong boundary point ID");
      check(bdry->bdry_points[4] == 12, "Wrong boundary point ID");

      /* Boundary edges are ordered similar to input mesh format */
      check(bdry->bdry_edges[0][0] == 8, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[0][1] == 9, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][0] == 9, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][1] == 10, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][0] == 10, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][1] == 11, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][0] == 11, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][1] == 12, "Wrong boundary edge point ID");
    }

    if ( i_bdry == 3 )
    {
      check(bdry->type == WALL, "Wrong boundary type");
      check(bdry->n_bdry_points == 5, "Wrong number of boundary points");
      check(bdry->n_bdry_edges == 4, "Wrong number of boundary points");

      /* Boundary vertices are ordered in ascencing index order */
      check(bdry->bdry_points[0] == 0, "Wrong boundary point ID");
      check(bdry->bdry_points[1] == 12, "Wrong boundary point ID");
      check(bdry->bdry_points[2] == 13, "Wrong boundary point ID");
      check(bdry->bdry_points[3] == 14, "Wrong boundary point ID");
      check(bdry->bdry_points[4] == 15, "Wrong boundary point ID");

      /* Boundary edges are ordered similar to input mesh format */
      check(bdry->bdry_edges[0][0] == 12, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[0][1] == 13, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][0] == 13, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[1][1] == 14, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][0] == 14, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[2][1] == 15, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][0] == 15, "Wrong boundary edge point ID");
      check(bdry->bdry_edges[3][1] ==  0, "Wrong boundary edge point ID");
    }

    ++i_bdry;
  }

  /*------------------------------------------------------------------
  | Clean up
  ------------------------------------------------------------------*/
  DualGrid_destroy( dualgrid );
  PrimaryGrid_destroy( primgrid );
  MeshReader_destroy( mesh_reader );

  return ICF_SUCCESS;

error:

  return ICF_ERROR;

} /* test_DualGrid_build_boundaries() */


/*********************************************************************
* Test building of dualgrid elements 
*********************************************************************/
int test_DualGrid_build()
{
  MeshReader  *mesh_reader = MeshReader_create( test_grid );
  PrimaryGrid *primgrid    = PrimaryGrid_create();
  DualGrid    *dualgrid    = DualGrid_create();

  MeshReader_read_primgrid( mesh_reader, primgrid );

  /*------------------------------------------------------------------
  | Manually set boundary definition 
  | -> This will be passed by the input parameter file
  ------------------------------------------------------------------*/
  BoundaryDef* bdry_def = dualgrid->boundaries->bdry_def;
  bdry_def->n_bdry_markers = 4;
  bdry_def->bdry_markers = calloc(4, sizeof(int));
  bdry_def->bdry_markers[0] = 1; // INLET
  bdry_def->bdry_markers[1] = 2; // DOMAIN WALL
  bdry_def->bdry_markers[2] = 3; // OUTLET
  bdry_def->bdry_markers[3] = 4; // RECTANGLE WALL

  bdry_def->bdry_types = calloc(4, sizeof(BoundaryType));
  bdry_def->bdry_types[0] = INLET;
  bdry_def->bdry_types[1] = WALL;
  bdry_def->bdry_types[2] = OUTLET;
  bdry_def->bdry_types[3] = WALL;
  
  /*------------------------------------------------------------------
  | Build the dualgrid 
  ------------------------------------------------------------------*/
  DualGrid_build(dualgrid, bdry_def, primgrid);

  /*------------------------------------------------------------------
  | Test dualgrid metrics
  ------------------------------------------------------------------*/
  double total_vol = 0.0;
  int pnt, iface;

  for ( pnt = 0; pnt < dualgrid->n_elements; pnt++ )
    total_vol += dualgrid->vol[pnt];

  check( EQ(total_vol, 1.0), "Wrong calculation of element areas.");


  for ( iface = 0; iface < dualgrid->n_intr_faces; iface++ )
  {
    const int p0 = dualgrid->face_nbrs[iface][0];
    const int p1 = dualgrid->face_nbrs[iface][1];

    /* Check face normals at triangle elements */
    check( !( p0 == 23 && p1 == 19 ),
        "Wrong face-to-element connectivity");
    if ( p0 == 19 && p1 == 23 ) 
    {
      check( EQ(dualgrid->face_norms[iface][0], -1./6.),
          "Wrong face normal calculation.");
      check( EQ(dualgrid->face_norms[iface][1], 0.),
          "Wrong face normal calculation.");
    }

    /* Check face normals at quad elements */
    check( !( p0 == 17 && p1 == 2 ),
        "Wrong face-to-element connectivity");
    if ( p0 == 2 && p1 == 17 ) 
    {
      check( EQ(dualgrid->face_norms[iface][0], 0.),
          "Wrong face normal calculation.");
      check( EQ(dualgrid->face_norms[iface][1], 1./4.),
          "Wrong face normal calculation.");
    }

    /* Check face normals at boundary elements */
    check( !( p0 == 2 && p1 == 1 ),
        "Wrong face-to-element connectivity");
    if ( p0 == 1 && p1 == 2 ) 
    {
      check( EQ(dualgrid->face_norms[iface][0], 1./8.),
          "Wrong face normal calculation.");
      check( EQ(dualgrid->face_norms[iface][1], 0.),
          "Wrong face normal calculation.");
    }
    
  }

  /* Print out face normals and elements 
  for ( iface = 0; iface < dualgrid->n_intr_faces; iface++ )
    printf("Face %d: %d->%d - (%lf,%lf)\n",
        iface, 
        dualgrid->face_nbrs[iface][0], 
        dualgrid->face_nbrs[iface][1],
        dualgrid->face_norms[iface][0], 
        dualgrid->face_norms[iface][1]);
  */


  /*------------------------------------------------------------------
  | Clean up
  ------------------------------------------------------------------*/
  DualGrid_destroy( dualgrid );
  PrimaryGrid_destroy( primgrid );
  MeshReader_destroy( mesh_reader );


  return ICF_SUCCESS;

error:

  return ICF_ERROR;

} /* test_DualGrid_build()*/


/*********************************************************************
* 
*********************************************************************/
int run_tests_DualGrid()
{
  check( test_DualGrid_create_destroy(), 
      "> test_DualGrid_create_destroy() failed" ); 

  check( test_DualGrid_build_boundaries(), 
      "> test_DualGrid_build_boundaries() failed" ); 

  check( test_DualGrid_build(), 
      "> test_DualGrid_build() failed" ); 

  fprintf(stderr, "> test_DualGrid() succeeded\n");
  return ICF_SUCCESS;

error:
  fprintf(stderr, "> test_DualGrid() failed\n");
  return ICF_ERROR;

} /* run_tests_DualGrid() */
