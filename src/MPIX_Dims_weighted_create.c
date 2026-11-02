/*
 * Copyright (c) 2018-2019 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER NOR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "MPIX_Dims_weighted_create.h"

#include <limits.h>
#include <math.h>
#include <mpi.h>

#include <stdio.h>
#include <stdlib.h>

/** Initial capacity for the divisors <= sqrt(n). 32 covers every n < 10^4
 * without reallocation; the worst case for 32-bit int (2095133040, 1600
 * divisors, 800 of them <= sqrt(n)) needs five doublings. */
#define DIVISORS_INITIAL_CAPACITY 32

/** Return list of divisors
 *
 * @param[in] n number for which divisors shall be computed, n >= 1
 * @param[out] divisors newly allocated list of all divisors of n in
 *                      descending order (last entry is 1); must be released
 *                      with free() by the caller
 * @return number of divisors if successful, 0 if memory allocation failed
 */
static int calc_divisors(int n, int **divisors) {
  *divisors = NULL;
  int capacity = DIVISORS_INITIAL_CAPACITY;
  int *divs = malloc(capacity * sizeof(int));
  if (NULL == divs) {
    return 0;
  }

  /* collect divisors f <= sqrt(n) in ascending order; odd n has no even
   * divisors. f <= n / f avoids both sqrt() and overflow of f * f. */
  int nsmall = 0;
  const int step = (n % 2 == 0) ? 1 : 2;
  for (int f = 1; f <= n / f; f += step) {
    if (n % f != 0) {
      continue;
    }
    if (nsmall == capacity) {
      capacity *= 2;
      int *tmp = realloc(divs, capacity * sizeof(int));
      if (NULL == tmp) {
        free(divs);
        return 0;
      }
      divs = tmp;
    }
    divs[nsmall++] = f;
  }

  /* each divisor f <= sqrt(n) pairs with n / f >= sqrt(n); for a perfect
   * square sqrt(n) pairs with itself and must only be listed once. */
  const int largest_small = divs[nsmall - 1];
  const int total = 2 * nsmall - ((largest_small == n / largest_small) ? 1 : 0);

  int *tmp = realloc(divs, total * sizeof(int));
  if (NULL == tmp) {
    free(divs);
    return 0;
  }
  divs = tmp;

  /* reorder in place into descending order:
   * [n/divs[0], ..., n/divs[nsmall-1], divs[nsmall-1], ..., divs[0]]
   * 1. move the small divisors reversed to the tail. Position total-1-k >= k
   *    always holds, so no unread entry is overwritten.
   * 2. fill the head with the complementary large divisors, reading only
   *    from the tail (indices >= nsmall). */
  for (int k = 0; k < nsmall; k++) {
    divs[total - 1 - k] = divs[k];
  }
  for (int k = 0; k < total - nsmall; k++) {
    divs[k] = n / divs[total - 1 - k];
  }

  *divisors = divs;
  return total;
}

/** recursive factorization optimization looping over (remaining) divisors for
 * each factor
 * @param[in] p number to factorize
 * @param[in] divisors list of divisors to check, entry 1 mark list end
 * @param[in] number of factors
 * @param[in] i index of current dimension
 * @param[in] dim_weights weight factors for dimensions, must be sorted in
 * increasing order
 * @param[out] dims computed optimal values for dimensions
 */
static void optdims(const int p, int *divisors, const int ndims, int i,
                    double *dim_weights, int *dims, double *min_sum,
                    int *min_dims, int *min_diff) {
  if (p == 1) {
    for (int k = i; k < ndims; k++) {
      dims[k] = 1;
    }
    i = ndims - 1; /* no need for recursion, all remaining factors computed, so
                      set to end index */
  } else {
    if (i < ndims - 1) {
      /* try all possible divisors for current index in dims array */
      for (int *divisor = divisors; *divisor > 1; divisor++) {
        int k = *divisor;
        if (p % k != 0) {
          continue;
        }
        if (k < round(pow((double)p / k, 1. / (ndims - i - 1)))) {
          break; /* product of remaining factors smaller than k */
        }
        dims[i] = k;
        /* recurse for next index position */
        optdims(p / k, divisor, ndims, i + 1, dim_weights, dims, min_sum,
                min_dims, min_diff);
      }
    } else {
      /* last index reached (i >= ndims - 1) */
      dims[i] = p;
    }
  }

  /* compute optimization criteria metric; compare to best value found so far */
  if (i == ndims - 1) {
    double sum = 0.0;
    int min = ndims * p;
    int max = 0;
    for (int k = 0; k < ndims; k++) {
      sum += dim_weights[k] * dims[k];
      if (dims[k] < min) {
        min = dims[k];
      }
      if (dims[k] > max) {
        max = dims[k];
      }
    }
    int diff = max - min;
    if ((sum < *min_sum) || ((sum == *min_sum) && (diff < *min_diff)) ||
        ((sum == *min_sum) && (diff == *min_diff) && (p < dims[0]))) {
      for (int k = 0; k < ndims; k++) {
        min_dims[k] = dims[k];
      }
      *min_sum = sum;
      *min_diff = diff;
    }
  }
}

int MPIX_Dims_weighted_create(const int nnodes, const int ndims,
                             const double *dim_weights, int *dims) {
  int ret = PMPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  return ret;
}

int PMPIX_Dims_weighted_create(const int nnodes, const int ndims,
                              const double *dim_weights, int *dims) {
  if (nnodes < 1) {
    return MPI_ERR_ARG;
  }
  if (ndims < 0) {
    return MPI_ERR_DIMS;
  }

  int dims_product = 1;
  for (int d = 0; d < ndims; d++) {
    if (dims[d] > 0) {
      if (dims[d] > INT_MAX / dims_product) {
        /* integer overflow caused by provided dimenstions */
        return MPI_ERR_INTERN;
      }
      dims_product *= dims[d];
    }
    if (dims[d] < 0) {
      return MPI_ERR_DIMS;
    }
  }
  if ((nnodes % dims_product) != 0) {
    return MPI_ERR_DIMS;
  }

  if (ndims == 0) {
    if (nnodes == 1) {
      return MPI_SUCCESS;
    }
    return MPI_ERR_DIMS;
  }

  if (nnodes == 1) {
    for (int i = 0; i < ndims; i++) {
      dims[i] = 1;
    }
    return MPI_SUCCESS;
  }

  double tmp_dim_weights[ndims];
  if (dim_weights == MPIX_EQUAL_WEIGHTS) {
    for (int i = 0; i < ndims; i++) {
      tmp_dim_weights[i] = 1.0;
    }
  } else {
    for (int i = 0; i < ndims; i++) {
      tmp_dim_weights[i] = dim_weights[i];
    }
  }

  int permutation[ndims];
  for (int i = 0; i < ndims; i++) {
    permutation[i] = i;
  }
  for (int i = 0; i < ndims; i++) {
    for (int j = i + 1; j < ndims; j++) {
      if (tmp_dim_weights[i] > tmp_dim_weights[j]) {
        double tmp_weight = tmp_dim_weights[i];
        tmp_dim_weights[i] = tmp_dim_weights[j];
        tmp_dim_weights[j] = tmp_weight;
        int tmp_i = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = tmp_i;
      }
    }
  }

  double min_sum = ((double)nnodes) * ndims * tmp_dim_weights[ndims - 1];
  int min_diff = nnodes - 1;
  int min_dims[ndims];
  for (int i = 0; i < ndims; i++) {
    min_dims[i] = 1;
  }
  int tmp_dims[ndims];
  for (int i = 0; i < ndims; i++) {
    tmp_dims[i] = 1;
  }

  int *divisors = NULL;
  if (0 == calc_divisors(nnodes, &divisors)) {
    return MPI_ERR_NO_MEM;
  }
  optdims(nnodes, divisors, ndims, 0, tmp_dim_weights, tmp_dims, &min_sum,
          min_dims, &min_diff);
  free(divisors);

  for (int i = 0; i < ndims; i++) {
    dims[permutation[i]] = min_dims[i];
  }

  return MPI_SUCCESS;
}
