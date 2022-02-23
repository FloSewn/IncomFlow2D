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

  free(boundaries);

} /* BoundaryList_create() */

/***********************************************************************
* Function to build the BoundaryList structure from a PrimaryGrid
***********************************************************************/
void BoundaryList_build(BoundaryList *boundaries, 
                        PrimaryGrid  *primgrid)
{



} /* BoundaryList_build() */






