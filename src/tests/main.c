#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "icf_utils.h"

#include "run_tests.h"

/*********************************************************************
* The main function
*********************************************************************/
int main()
{
  fprintf(stderr, "\n");
  fprintf(stderr, "==============================================\n");
  fprintf(stderr, "INCOMFLOW UNIT TESTS\n");
  fprintf(stderr, "==============================================\n");
  fprintf(stderr, "\n");

  run_tests_MeshReader();

  fprintf(stderr, "\n\nEverything works like a charm.\n\n");

  return EXIT_SUCCESS;
}
