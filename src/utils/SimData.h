/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef SIMDATA_H
#define SIMDATA_H

#include "PrimaryGrid.h"
#include "DualGrid.h"

/***********************************************************************
* 
***********************************************************************/
#define ICF_N_VARIABLES (100)

/***********************************************************************
* Forward declarations
***********************************************************************/
struct SimData;

/***********************************************************************
* SimData structure
***********************************************************************/
typedef struct SimData {

  PrimaryGrid *primgrid;
  DualGrid    *dualgrid;

  /* Independent field variables */
  double     *vars[ICF_N_VARIABLES];

  /* Gradient and Hessian of field variables */
  double    (*grad[ICF_N_VARIABLES])[3];
  double    (*hess[ICF_N_VARIABLES])[6];

  /* Auxiliary dependent field variables */
  double     *dep_vars[ICF_N_VARIABLES];

  /* Mass flux at interior edges */
  double     *mflux;

} SimData;


#endif /* SIMDATA_H */
