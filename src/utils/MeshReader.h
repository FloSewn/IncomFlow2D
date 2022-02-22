/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef MESHREADER_H
#define MESHREADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bstrlib.h"
#include "PrimaryGrid.h"

#define ICF_FILE_IO_ERROR -1

void test_mesh_io();


/***********************************************************************
* Mesh file reader structure
***********************************************************************/
struct MeshReader;
typedef struct MeshReader {
  const char      *path;    /* Path of file                 */
  bstring          txt;     /* bstring with file data       */
  struct bstrList *txtlist; /* file, splitted for newlines  */

  long             length;  /* Number of chars in total file*/
                            /* -> including '\0' at end     */
  int              nlines;  /* Number of lines in total file*/

} MeshReader;

/***********************************************************************
* Function to create a new mesh reader structure
***********************************************************************/
MeshReader *MeshReader_create(const char *file_path);

/***********************************************************************
* Function to destroy a mesh reader structure
***********************************************************************/
int MeshReader_destroy(MeshReader *mesh_reader);

/***********************************************************************
* Function to read a primary grid structure from a given grid file 
***********************************************************************/
void MeshReader_read_primgrid(MeshReader  *mesh_reader, 
                              PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid vertices from a grid file
***********************************************************************/
void MeshReader_read_vertices(MeshReader  *mesh_reader,
                              PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid triangles from a grid file
***********************************************************************/
void MeshReader_read_tris(MeshReader  *mesh_reader,
                          PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid triangle neighbors from a grid file
***********************************************************************/
void MeshReader_read_tri_neighbors(MeshReader  *mesh_reader,
                                   PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid quads from a grid file
***********************************************************************/
void MeshReader_read_quads(MeshReader  *mesh_reader,
                           PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid quad neighbors from a grid file
***********************************************************************/
void MeshReader_read_quad_neighbors(MeshReader  *mesh_reader,
                                    PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the interior primary grid edges from a grid file
***********************************************************************/
void MeshReader_read_intr_edges(MeshReader  *mesh_reader,
                                PrimaryGrid *prim_grid);

/***********************************************************************
* Function to read the primary grid boundary edges from a grid file
***********************************************************************/
void MeshReader_read_bdry_edges(MeshReader  *mesh_reader,
                                PrimaryGrid *prim_grid);


#endif /* MESHREADER_H */
