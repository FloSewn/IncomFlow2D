/*
* This file is part of the IncomFlow2D library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#ifndef ICF_UTILS_H
#define ICF_UTILS_H

#define ICF_TRUE  (1);
#define ICF_FALSE (0);

#define ICF_SUCCESS  (1);
#define ICF_ERROR    (0);

#define ICF_SMALL (1.0E-14)

/*********************************************************************
* Helper functions
*********************************************************************/
#ifndef SQR
#define SQR(x) ( (x) * (x) ) 
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) ) 
#endif

#ifndef MIN
#define MIN(a, b) ( (a) < (b) ? (a) : (b) ) 
#endif

#ifndef MAX0
#define MAX0(a) ( (a) > 0 ? (a) : 0 ) 
#endif

#ifndef MIN0
#define MIN0(a) ( (a) < 0 ? (a) : 0 ) 
#endif

#ifndef ABS
#define ABS(a) ( (a) < 0 ? -1 * (a) : (a) ) 
#endif

#ifndef EQ
#define EQ(a, b) ( ABS((a)- (b)) < ICF_SMALL ? 1 : 0 )
#endif

#ifndef MOD
#define MOD(n, M) ( ( ( (n) % (M) ) + (M) ) % (M) )
#endif



#endif /* ICF_UTILS_H */
