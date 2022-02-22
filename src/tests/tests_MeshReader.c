#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dbg.h"
#include "icf_utils.h"
#include "MeshReader.h"
#include "PrimaryGrid.h"

const char *test_grid = "/datadisk/Code/C-Code/SimpleSolver/input/grid/PrimaryGrid.dat";

/*********************************************************************
* Test creation / destruction of MeshReader structure 
*********************************************************************/
int test_MeshReader_create_destroy()
{
  MeshReader *mesh_reader = MeshReader_create( test_grid );

  MeshReader_destroy( mesh_reader );

  return ICF_SUCCESS;

} /* test_MeshReader_create_destroy() */

/*********************************************************************
* Test reading of primary grid with MeshReader structure
*********************************************************************/
int test_MeshReader_read_primgrid()
{
  MeshReader *mesh_reader = MeshReader_create( test_grid );

  PrimaryGrid *prim_grid = PrimaryGrid_create();

  MeshReader_read_primgrid( mesh_reader, prim_grid );

  check( prim_grid->n_vertices == 1311,
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->n_tris == 1650, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->n_quads == 372, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->n_intr_edges == 3105, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->n_bdry_edges == 228, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->tris[0][0] == 549, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->tris[1][1] == 221, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->tris[2][2] == 564, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->quads[0][0] == 21, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quads[1][1] == 23, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quads[2][2] == 230, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quads[3][3] == 230, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->tri_neighbors[0][0] == 325, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->tri_neighbors[4][1] == 159, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->tri_neighbors[5][2] == -1, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->quad_neighbors[0][0] == 1, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quad_neighbors[1][1] == 81, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quad_neighbors[2][2] == 1, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->quad_neighbors[3][3] == -1, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->intr_edges[0][0] == 228, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->intr_edges[1][1] == 23, 
    "> MeshReader_read_primgrid() failed");


  check( prim_grid->bdry_edges[0][0] == 19, 
    "> MeshReader_read_primgrid() failed");
  check( prim_grid->bdry_edges[1][1] == 21, 
    "> MeshReader_read_primgrid() failed");

  check( prim_grid->bdry_edge_marker[2] == 2, 
    "> MeshReader_read_primgrid() failed");


  PrimaryGrid_destroy( prim_grid );

  MeshReader_destroy( mesh_reader );

  return ICF_SUCCESS;

error:
  return ICF_ERROR;

} /* test_MeshReader_read_primgrid() */


/*********************************************************************
* 
*********************************************************************/
int run_tests_MeshReader()
{
  check( test_MeshReader_create_destroy(), 
      "> test_MeshReader_create_destroy() failed" ); 

  check( test_MeshReader_read_primgrid(), 
      "> test_MeshReader_read_primgrid() failed" ); 

  fprintf(stderr, "> test_MeshReader() succeeded");
  return ICF_SUCCESS;

error:
  fprintf(stderr, "> test_MeshReader() failed");
  return ICF_ERROR;

} /* run_tests_MeshReader() */
