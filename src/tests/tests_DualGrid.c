#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dbg.h"
#include "icf_utils.h"
#include "MeshReader.h"
#include "PrimaryGrid.h"
#include "DualGrid.h"

static const char *test_grid = "/datadisk/Code/C-Code/SimpleSolver/input/grid/PrimaryGrid.dat";

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


  DualGrid_destroy( dualgrid );
  PrimaryGrid_destroy( primgrid );
  MeshReader_destroy( mesh_reader );


  return ICF_SUCCESS;

} /* test_DualGrid_build_boundaries() */


/*********************************************************************
* 
*********************************************************************/
int run_tests_DualGrid()
{
  check( test_DualGrid_create_destroy(), 
      "> test_DualGrid_create_destroy() failed" ); 

  check( test_DualGrid_build_boundaries(), 
      "> test_DualGrid_build_boundaries() failed" ); 

  fprintf(stderr, "> test_DualGrid() succeeded\n");
  return ICF_SUCCESS;

error:
  fprintf(stderr, "> test_DualGrid() failed\n");
  return ICF_ERROR;

} /* run_tests_DualGrid() */
