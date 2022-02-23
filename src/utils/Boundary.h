/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef BOUNDARY_H
#define BOUNDARY_H

/***********************************************************************
* Forward declarations
***********************************************************************/
typedef struct PrimaryGrid PrimaryGrid;
typedef struct BoundaryList BoundaryList;
typedef struct SimData SimData;
typedef struct Boundary Boundary;
typedef struct BoundaryDef BoundaryDef;

/***********************************************************************
* Boundary Types
***********************************************************************/
typedef enum
{
  PERIODIC,
  SYMMETRY,
  INLET,
  OUTLET,
  WALL,
} BoundaryType;

/***********************************************************************
* Boundary function templates
***********************************************************************/
typedef void BdryFun_set_dirichlet(SimData  *sim_data,
                                   Boundary *boundary);

typedef void BdryFun_set_neumann(SimData  *sim_data,
                                 Boundary *boundary);

typedef void BdryFun_set_intr_mflux(SimData  *sim_data,
                                    Boundary *boundary);

typedef void BdryFun_set_bdry_mflux(SimData  *sim_data,
                                    Boundary *boundary);


/***********************************************************************
* BoundaryDef structure
***********************************************************************/
typedef struct BoundaryDef
{
  /* Number of total boundary markers */
  int n_bdry_markers;

  /* Used boundary marker indices */
  int *bdry_markers;

  /* Associated boundary type for every boundary marker */
  BoundaryType *bdry_types;

} BoundaryDef;

/***********************************************************************
* BoundaryList structure
***********************************************************************/
typedef struct BoundaryList 
{
  BoundaryDef *bdry_def;

  int       n_boundaries;

  Boundary *start;
  Boundary *end;

} BoundaryList;


/***********************************************************************
* Boundary structure
*
*               bdry_norm[v2] -> here is bdry_mflux[v2] stored
*
*                  ^       ^
*             e1   :       :    e2          
*     x-------o--------x--------o--------x
*  v1 |\      :         v2                v3
*     |  \    :-->
*     |    \  :  intr_norm[e1]
*     |______\:  -> these are also the edges, where intr_mflux[e1] 
*     |             is stored
*
*  bdry_points = [v1, v2, v3, ...]
*  bdry_edges  = [(v1,v2), (v2,v3), ...]
*
***********************************************************************/
typedef struct Boundary {

  BoundaryList *boundaries;
  Boundary     *next;
  Boundary     *prev;

  /* Name of this boundary */
  char name;

  int n_bdry_points;
  int n_bdry_edges;
  
  /* IDs of primary grid vertices adjacent to this boundary */
  int *bdry_points;

  /* IDs of primary grid vertices that define a boundary edge */
  int (*bdry_edges)[2];

  /*--------------------------------------------------------------------
  | These are quantities, that are stored at the boundary vertices,
  | and which are needed for the actual boundary fluxes
  --------------------------------------------------------------------*/
  double (*bdry_norm)[2];
  double *bdry_mflux;

  /*--------------------------------------------------------------------
  | These are quantities, that are stored at the boundary edges,
  | and which are needed for the interior fluxes of boundary elements
  --------------------------------------------------------------------*/
  double (*intr_norm)[2];
  double *intr_mflux;

  /*--------------------------------------------------------------------
  | Boundary functions
  --------------------------------------------------------------------*/
  BdryFun_set_dirichlet       *set_dirichlet;
  BdryFun_set_neumann         *set_neumann;
  BdryFun_set_intr_mflux      *set_intr_mflux;
  BdryFun_set_bdry_mflux      *set_bdry_mflux;


} Boundary;



/***********************************************************************
* Function to create and initialize a new BoundaryDef structure
***********************************************************************/
BoundaryDef *BoundaryDef_create();

/***********************************************************************
* Function to destroy BoundaryDef structure
***********************************************************************/
void BoundaryDef_destroy(BoundaryDef* bdry_def);

/***********************************************************************
* Function to read the boundary definitions from a parameter file
***********************************************************************/
void BoundaryDef_build(void *param_file);


/***********************************************************************
* Function to create and initialize a new BoundaryList structure
***********************************************************************/
BoundaryList *BoundaryList_create();

/***********************************************************************
* Function to destroy BoundaryList structure
***********************************************************************/
void BoundaryList_destroy(BoundaryList* boundaries);

/***********************************************************************
* Function to build the BoundaryList structure from a PrimaryGrid
***********************************************************************/
void BoundaryList_build(BoundaryList *boundaries, 
                        PrimaryGrid  *primgrid);



#endif /* BOUNDARY_H */
