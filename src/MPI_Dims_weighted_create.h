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

#ifndef SRC_MPI_DIMS_WEIGHTED_CREATE_H_
#define SRC_MPI_DIMS_WEIGHTED_CREATE_H_

#if __cplusplus
extern "C" {
#endif

/** constant representing equal weights for all dimensions in a call to \
   MPI_Dims_weighted_create */
#define MPI_EQUAL_WEIGHTS 0

/** Compute dimensions based on weights
 *
 * MPI_Dims_weighted_create can be used to obtain dimensions useable for a
 * process grid of a given number of nodes based on three combined optimization
 * criteria. The optimization criteria include weights \f$\omega_i\f$ for each
 * dimension. The following criteria area applied in order:
// clang-format off
 *   1. minimization of
 *      the sum
 *      \f[
 *      \sum_{i=1}^{\text{ndims}} \omega_i \text{}_i \qquad, \text{with} \qquad
nnodes = \prod_{i=1}^{\text{ndims}} dims_i
 *      \f]
 *   2. minimization of the
 *      difference between minimum and maximum dimension \f[
 *      \max_{i=1}^{\text{ndims}} p_i - \min_{i=1}^{\text{ndims}}
 *      \f]
 *   3. minimization of the largest dimension
 *      \f[
 *      \max_{i=1}^{\text{ndims}} p_i
 *      \f]
// clang-format on
 * The criteria make it e.g. suitable to achieve an improved mapping of a
 * process grid to the underlying application grid, that involves communication
 * along grid dimensions (nearest neighbour communication). If the application
 * grid has \f$G = \prod_{i=1\^{\text{ndims}} g_i\f$ grid points, the weights
 * become $f$\omega_i = \frac{1}{g_i}$f$, assuming equal halo widths in all
 * dimensions.
 *
 * @param[in] nnodes number of processes
 * @param[in] number of dimensions
 * @param[in] dim_weights weight factors for dimensions or MPI_EQUAL_WEIGHTS
 * @param[out] dims computed optimal values for dimensions
 */
int MPI_Dims_weighted_create(const int nnodes, const int ndims,
                             const double *dim_weights, int *dims);

/** PMPI interface corresponding to MPI call */
int PMPI_Dims_weighted_create(const int nnodes, const int ndims,
                              const double *dim_weights, int *dims);

#if __cplusplus
}
#endif

#endif // SRC_MPI_DIMS_WEIGHTED_CREATE_H_
