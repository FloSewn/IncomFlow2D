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

  PrimaryGrid *primgrid = PrimaryGrid_create();

  MeshReader_read_primgrid( mesh_reader, primgrid );

  check( primgrid->n_vertices == 24,
    "> MeshReader_read_primgrid() failed");
  check( primgrid->n_tris == 6, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->n_quads == 12, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->n_intr_edges == 25, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->n_bdry_edges == 16, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->tris[0][0] == 16, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->tris[1][1] == 18, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->tris[2][2] == 21, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->quads[0][0] == 1, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quads[1][1] == 3, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quads[2][2] == 19, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quads[3][3] == 19, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->tri_neighbors[0][0] == 16, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->tri_neighbors[1][1] == 16, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->tri_neighbors[2][2] == 3, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->quad_neighbors[0][0] == 1, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quad_neighbors[1][1] == 13, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quad_neighbors[2][2] == 8, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->quad_neighbors[3][3] == -1, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->intr_edges[0][0] == 17, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->intr_edges[1][1] == 3, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->intr_edge_nbrs[2][0] == 2, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->intr_edge_nbrs[3][1] == 2, 
    "> MeshReader_read_primgrid() failed");


  check( primgrid->bdry_edges[0][0] == 0, 
    "> MeshReader_read_primgrid() failed");
  check( primgrid->bdry_edges[1][1] == 2, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->bdry_edge_nbrs[2] == 1, 
    "> MeshReader_read_primgrid() failed");

  check( primgrid->bdry_edge_marker[3] == 1, 
    "> MeshReader_read_primgrid() failed");


  PrimaryGrid_destroy( primgrid );

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

  fprintf(stderr, "> test_MeshReader() succeeded\n");
  return ICF_SUCCESS;

error:
  fprintf(stderr, "> test_MeshReader() failed\n");
  return ICF_ERROR;

} /* run_tests_MeshReader() */
