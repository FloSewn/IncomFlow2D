/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef BSTRLIB_WRAPPER_H
#define BSTRLIB_WRAPPER_H

#include "bstrlib.h"

/***********************************************************************
* Function returns a bstring list of lines, that 
* do not contain a certain specifier
***********************************************************************/
struct bstrList *bstrlib_pop_lines_with(struct bstrList *txtlist,
                                        const char *fltr);

/***********************************************************************
* Function returns a bstring list of lines, that 
* do contain a certain specifier
***********************************************************************/
struct bstrList *bstrlib_get_lines_with(struct bstrList *txtlist,
                                        const char *fltr);

/***********************************************************************
* Function searches for a specifier <fltr> in a bstrList.
* The parameter behind the specifier is then extracted 
* from the file and stored into <value>.
* The value is casted to a prescribed type
* type = 0: integer
* type = 1: double
* type = 2: string
*
* Returns 0 if specifier was not found in the file.
* Otherwise, it returns the number of times, the 
* specifier was found.
* Returns -1 on errors.
***********************************************************************/
int bstrlib_extract_param(struct bstrList *txtlist,
                          const char *fltr, int type,
                          void *value);

/***********************************************************************
* Function searches for a specifier <fltr> in a bstrList.
* The string behind the specifier is then extracted 
* from the file and processed as an array of values 
* and stored in <value>.
* The values are casted to a prescribed type
* type = 0: integer
* type = 1: double
* type = 2: string
*
* Returns 0 if specifier was not found in the file.
* Otherwise, it returns the number of times, the 
* specifier was found.
* Returns -1 on errors.
***********************************************************************/
int bstrlib_extract_array(struct bstrList *txtlist,
                          const char *fltr, int type,
                          void *value);

#endif /* BSTRLIB_WRAPPER_H */
