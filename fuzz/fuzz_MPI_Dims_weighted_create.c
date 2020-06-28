#include "MPI_Dims_weighted_create.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

  /* Data layout required for multi parameter testing:
    nnodes(int),
    ndims(int),
    dim_weights(double[ndims]),
    dims(int[ndims])
    */

  /* we need at least nnodes and ndims */
  if (Size < 2 * sizeof(int)) {
    return 0;
  }

  int nnodes = ((int *)Data)[0];
  int ndims = ((int *)Data)[1];

  /* limit checks to a max dimension due to memory constraints */
  if (ndims > 3) {
    return 0;
  }
  /* check if we have correct amount of input data */
  if (Size != (2 + ndims) * sizeof(int) + ndims * sizeof(double)) {
    return 0;
  }
  double *dim_weights = calloc(1, ndims * sizeof(double));
  memcpy(dim_weights, &Data[2 * sizeof(int)], ndims * sizeof(double));

  int *dims = calloc(1, ndims * sizeof(int));
  memcpy(dims, &Data[2 * sizeof(int) + ndims * sizeof(int)],
         ndims * sizeof(int));

  MPI_Dims_weighted_create(nnodes, ndims, dim_weights, dims);

  free(dims);
  free(dim_weights);
  return 0; // Non-zero return values are reserved for future use.
}
