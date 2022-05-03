/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "icf_utils.h"
#include "dbg.h"
#include "bstrlib.h"
#include "bstrlib_wrapper.h"
#include "PrimaryGrid.h"
#include "MeshReader.h"

void test_mesh_io()
{
  printf("test_mesh_io() works like a charm, too.");
}

/***********************************************************************
* Function to create a new mesh reader structure
***********************************************************************/
MeshReader *MeshReader_create(const char *file_path)
{
  /*-------------------------------------------------------------------
  | Allocate memory for reader structure 
  -------------------------------------------------------------------*/
  MeshReader *mesh_reader = calloc(1, sizeof(MeshReader));
  check_mem(mesh_reader);

  mesh_reader->path = file_path;

  /*-------------------------------------------------------------------
  | Open text file and copy its data 
  -------------------------------------------------------------------*/
  FILE *fptr = NULL;
  fptr = fopen(mesh_reader->path, "rb");
  check(fptr, "Failed to open %s.", mesh_reader->path);

  /* Estimate length of chars in whole file                          */
  fseek(fptr, 0, SEEK_END);
  long length = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  /* Read total file into buffer                                     */
  char *buffer = (char *) malloc(length + 1);
  buffer[length] = '\0';
  fread(buffer, 1, length, fptr);

  /* Copy relevant data to MeshReader structure                      */
  bstring bbuffer = bfromcstr( buffer );
  mesh_reader->txt    = bbuffer;
  mesh_reader->length = length + 1;

  /* Split buffer according to newlines                              */
  char splitter = '\n';
  mesh_reader->txtlist = bsplit(bbuffer, splitter);
  mesh_reader->nlines = mesh_reader->txtlist->qty;

  fclose(fptr);
  free(buffer);

  return mesh_reader;
error:
  return NULL;

} /* MeshReader_create() */

/***********************************************************************
* Function to destroy a mesh reader structure
***********************************************************************/
int MeshReader_destroy(MeshReader *mesh_reader)
{
  bstrListDestroy(mesh_reader->txtlist);
  bdestroy(mesh_reader->txt);
  free(mesh_reader);
  return ICF_SUCCESS;

} /* MeshReader_destroy() */

/***********************************************************************
* Function to read a primary grid structure from a given grid file 
***********************************************************************/
void MeshReader_read_primgrid(MeshReader  *mesh_reader, 
                              PrimaryGrid *prim_grid)
{
  MeshReader_read_vertices(mesh_reader, prim_grid);

  MeshReader_read_quads(mesh_reader, prim_grid);
  MeshReader_read_quad_neighbors(mesh_reader, prim_grid);

  MeshReader_read_tris(mesh_reader, prim_grid);
  MeshReader_read_tri_neighbors(mesh_reader, prim_grid);

  MeshReader_read_intr_edges(mesh_reader, prim_grid);
  MeshReader_read_bdry_edges(mesh_reader, prim_grid);

} /* MeshReader_read_primgrid() */

/***********************************************************************
* Function to read the primary grid vertices from a grid file
***********************************************************************/
void MeshReader_read_vertices(MeshReader  *mesh_reader,
                              PrimaryGrid *prim_grid)
{
  double (*xyNodes)[2];
  int     nNodes = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of nodes 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "VERTICES", 0, &nNodes);
  check(nNodes > 0, "No nodes defined in mesh file.");

  /*--------------------------------------------------------------------
  | Get line index where we start to gather nodes
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "VERTICES" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nNodes + 1;

  xyNodes = calloc(nNodes, 2*sizeof(double));

  int i_node = 0;

  for (i = i+1; i < iMax; i++)
  {
    line  = flPtr[i];

    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 2, 
        "Wrong definition for node coordinates.");

    xyNodes[i_node][0] = atof(valPtr[0]->data);
    xyNodes[i_node][1] = atof(valPtr[1]->data);

    ++i_node;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->vertex_coords = xyNodes;
  prim_grid->n_vertices    = nNodes;

  return;

error:
  return;

} /* MeshReader_read_vertices() */

/***********************************************************************
* Function to read the primary grid triangles from a grid file
***********************************************************************/
void MeshReader_read_tris(MeshReader  *mesh_reader,
                          PrimaryGrid *prim_grid)
{
  int (*idxTris)[3];
  int nTris = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "TRIANGLES", 0, &nTris);

  if ( nTris < 1 )
  {
    prim_grid->n_tris = 0;
    return;
  }

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "TRIANGLES" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nTris + 1;

  idxTris = calloc(nTris, 3*sizeof(int));

  int i_tri = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 3, 
        "Wrong definition for triangles.");

    idxTris[i_tri][0] = atoi(valPtr[0]->data);
    idxTris[i_tri][1] = atoi(valPtr[1]->data);
    idxTris[i_tri][2] = atoi(valPtr[2]->data);
    
    i_tri++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->tris   = idxTris;
  prim_grid->n_tris = nTris;

  return;

error:
  return;

} /* MeshReader_read_tris() */

/***********************************************************************
* Function to read the primary grid triangle neighbors from a grid file
***********************************************************************/
void MeshReader_read_tri_neighbors(MeshReader  *mesh_reader,
                                   PrimaryGrid *prim_grid)
{
  int (*idxTriNbrs)[3];
  int nTris = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "TRIANGLENEIGHBORS", 0, &nTris);

  if ( nTris < 1 )
    return;

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "TRIANGLENEIGHBORS" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nTris + 1;

  idxTriNbrs = calloc(nTris, 3*sizeof(int));

  int i_tri = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 3, 
        "Wrong definition for triangles.");

    idxTriNbrs[i_tri][0] = atoi(valPtr[0]->data);
    idxTriNbrs[i_tri][1] = atoi(valPtr[1]->data);
    idxTriNbrs[i_tri][2] = atoi(valPtr[2]->data);

    i_tri++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->tri_neighbors = idxTriNbrs;

  return;

error:
  return;

} /* MeshReader_read_tri_neighbors() */

/***********************************************************************
* Function to read the primary grid quads from a grid file
***********************************************************************/
void MeshReader_read_quads(MeshReader  *mesh_reader,
                           PrimaryGrid *prim_grid)
{
  int (*idxQuads)[4];
  int nQuads = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "QUADS", 0, &nQuads);

  if ( nQuads < 1 )
  {
    prim_grid->n_quads = 0;
    return;
  }

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "QUADS" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nQuads + 1;

  idxQuads = calloc(nQuads, 4*sizeof(int));

  int i_quad = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 4, 
        "Wrong definition for quads.");

    idxQuads[i_quad][0] = atoi(valPtr[0]->data);
    idxQuads[i_quad][1] = atoi(valPtr[1]->data);
    idxQuads[i_quad][2] = atoi(valPtr[2]->data);
    idxQuads[i_quad][3] = atoi(valPtr[3]->data);
    
    i_quad++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->quads   = idxQuads;
  prim_grid->n_quads = nQuads;

  return;

error:
  return;

} /* MeshReader_read_quads() */

/***********************************************************************
* Function to read the primary grid quad neighbors from a grid file
***********************************************************************/
void MeshReader_read_quad_neighbors(MeshReader  *mesh_reader,
                                    PrimaryGrid *prim_grid)
{
  int (*idxQuadNbrs)[4];
  int nQuads = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "QUADNEIGHBORS", 0, &nQuads);

  if ( nQuads < 1 )
    return;

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "QUADNEIGHBORS" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nQuads + 1;

  idxQuadNbrs = calloc(nQuads, 4*sizeof(int));

  int i_quad = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 4, 
        "Wrong definition for quads.");

    idxQuadNbrs[i_quad][0] = atoi(valPtr[0]->data);
    idxQuadNbrs[i_quad][1] = atoi(valPtr[1]->data);
    idxQuadNbrs[i_quad][2] = atoi(valPtr[2]->data);
    idxQuadNbrs[i_quad][3] = atoi(valPtr[3]->data);

    i_quad++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->quad_neighbors = idxQuadNbrs;

  return;

error:
  return;
} /* MeshReader_read_quad_neighbors() */

/***********************************************************************
* Function to read the interior primary grid edges from a grid file
***********************************************************************/
void MeshReader_read_intr_edges(MeshReader  *mesh_reader,
                                PrimaryGrid *prim_grid)
{
  int (*idxEdges)[2];
  int (*idxEdgeNbrs)[2];
  int   nEdges = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "INTERIOREDGES", 0, &nEdges);
  check(nEdges > 0, "No interior edges defined in mesh file.");

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "INTERIOREDGES" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nEdges + 1;

  idxEdges    = calloc(nEdges, 2*sizeof(int));
  idxEdgeNbrs = calloc(nEdges, 2*sizeof(int));

  int i_edge = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check(nValues == 4, 
        "Wrong definition for interior edges.");

    idxEdges[i_edge][0] = atoi(valPtr[0]->data);
    idxEdges[i_edge][1] = atoi(valPtr[1]->data);

    idxEdgeNbrs[i_edge][0] = atoi(valPtr[2]->data);
    idxEdgeNbrs[i_edge][1] = atoi(valPtr[3]->data);
    
    i_edge++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->intr_edges     = idxEdges;
  prim_grid->intr_edge_nbrs = idxEdgeNbrs;
  prim_grid->n_intr_edges   = nEdges;

  return;

error:
  return;

} /* MeshReader_read_intr_edges() */

/***********************************************************************
* Function to read the primary grid boundary edges from a grid file
***********************************************************************/
void MeshReader_read_bdry_edges(MeshReader  *mesh_reader,
                                PrimaryGrid *prim_grid)
{
  int (*idxEdges)[2];
  int  *idxEdgeNbrs;
  int  *edgeMarker;
  int   nEdges = 0;

  int i, iMax, nValues;

  bstring line;
  bstring *valPtr;
  struct bstrList *values;

  /*--------------------------------------------------------------------
  | Get total number of triangles 
  --------------------------------------------------------------------*/
  bstrlib_extract_param(mesh_reader->txtlist, "BOUNDARYEDGES", 0, &nEdges);
  check(nEdges > 0, "No boundary edges defined in mesh file.");

  /*--------------------------------------------------------------------
  | Get line index where we start to gather tris
  --------------------------------------------------------------------*/
  bstring *flPtr  = mesh_reader->txtlist->entry;
  bstring markStr = bfromcstr( "BOUNDARYEDGES" );

  for (i = 0; i < mesh_reader->txtlist->qty; i++)
    if ( binstr(flPtr[i], 0, markStr) != BSTR_ERR )
      break;

  iMax = i + nEdges + 1;

  idxEdges    = calloc(nEdges, 2*sizeof(int));
  idxEdgeNbrs = calloc(nEdges, sizeof(int));
  edgeMarker  = calloc(nEdges, sizeof(int));

  int i_edge = 0;

  for (i = i+1; i < iMax; i++)
  {
    line    = flPtr[i];
    values  = bsplit(line, ',');
    nValues = values->qty;
    valPtr  = values->entry;

    check (nValues == 4, 
        "Wrong definition for boundary edges.");

    idxEdges[i_edge][0] = atoi(valPtr[0]->data);
    idxEdges[i_edge][1] = atoi(valPtr[1]->data);
    idxEdgeNbrs[i_edge] = atoi(valPtr[2]->data);
    edgeMarker[i_edge]  = atoi(valPtr[3]->data);
    
    i_edge++;

    bstrListDestroy( values );

  }

  bdestroy( markStr );

  prim_grid->bdry_edges       = idxEdges;
  prim_grid->bdry_edge_nbrs   = idxEdgeNbrs;
  prim_grid->bdry_edge_marker = edgeMarker;
  prim_grid->n_bdry_edges     = nEdges;

  return;

error:
  return;

} /* MeshReader_read_bdry_edges() */
