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

#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_CONSOLE_WIDTH 100
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

#include <array>
#include <cstdint>

#include <MPIX_Dims_weighted_create.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int result = Catch::Session().run(argc, argv);
  MPI_Finalize();
  return result;
}
TEST_CASE("MPIX_EQUAL_WEIGHTS is defined", "[MPIX_Dims_weighted_create]") {
  REQUIRE(MPIX_EQUAL_WEIGHTS == MPIX_EQUAL_WEIGHTS);
}

TEST_CASE("error checks for wrong input working",
          "[MPIX_Dims_weighted_create]") {

  int nnodes = 1;
  int ndims = 1;
  double *dim_weights = MPIX_EQUAL_WEIGHTS;
  int *dims = NULL;

  SECTION("if nnodes less than one return failure") {
    nnodes = 0;
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(ret != MPI_SUCCESS);
    nnodes = -1;
    ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(ret != MPI_SUCCESS);
  }

  SECTION("if ndims less than zero return failure") {
    nnodes = 1;
    ndims = -1;
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(ret != MPI_SUCCESS);
  }

  SECTION("if nnode not multiple of provided dims return failure") {
    nnodes = 7;
    ndims = 2;
    int dims[] = {2, 3};
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(ret != MPI_SUCCESS);
  }

  SECTION("if provided dim is negative return failure") {
    nnodes = 6;
    ndims = 2;
    int dims[] = {0, -3};
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(ret != MPI_SUCCESS);
  }
}

TEST_CASE("one node returns all-once dims vector and success",
          "[MPIX_Dims_weighted_create]") {
  int nnodes = 1;
  int ndims = 5;
  double *dim_weights = MPIX_EQUAL_WEIGHTS;
  int dims[5] = {0, 0, 0, 0, 0};
  int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);

  for (int d = 0; d < ndims; ++d) {
    REQUIRE(dims[d] == 1);
  }
  REQUIRE(ret == MPI_SUCCESS);
}

TEST_CASE("one node and zero ndims return success",
          "[MPIX_Dims_weighted_create]") {
  int nnodes = 1;
  int ndims = 0;
  double *dim_weights = MPIX_EQUAL_WEIGHTS;
  int *dims = NULL;
  int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  REQUIRE(ret == MPI_SUCCESS);
}

TEST_CASE("product of dims must be equal nnodes",
          "[MPIX_Dims_weighted_create]") {
  int nnodes = 30;
  int ndims = 3;
  double *dim_weights = MPIX_EQUAL_WEIGHTS;
  int dims[] = {0, 0, 0};
  int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  int dims_product = 1;
  for (int d = 0; d < ndims; ++d) {
    dims_product *= dims[d];
  }
  REQUIRE(dims_product == nnodes);
  REQUIRE(ret == MPI_SUCCESS);
}

// Regression test: the divisor list used to be a fixed stack array of 1344
// entries (the maximum for 9-digit numbers), which overflowed for valid int
// inputs with more divisors. Expected values are the unique optima, verified
// by brute force.
TEST_CASE("large nnodes with many divisors", "[MPIX_Dims_weighted_create]") {
  struct Case {
    int nnodes;
    int ndims;
    std::array<int, 3> expected;
  };
  const Case cases[] = {
      {1102701600, 2, {33264, 33150, 0}}, /* 1440 divisors */
      {1396755360, 2, {37620, 37128, 0}}, /* 1536 divisors */
      {2095133040, 2, {46189, 45360, 0}}, /* 1600 divisors, max for int32 */
      {2095133040, 3, {1292, 1287, 1260}},
      {2147395600, 2, {46340, 46340, 0}}, /* perfect square 46340^2 */
      {2147395600, 3, {1324, 1324, 1225}},
      {2147483647, 2, {2147483647, 1, 0}}, /* INT_MAX, prime */
  };

  for (const Case &c : cases) {
    CAPTURE(c.nnodes, c.ndims);
    std::array<int, 3> dims = {0, 0, 0};
    int ret = MPIX_Dims_weighted_create(c.nnodes, c.ndims, MPIX_EQUAL_WEIGHTS,
                                        dims.data());
    REQUIRE(ret == MPI_SUCCESS);
    for (int d = 0; d < c.ndims; ++d) {
      CHECK(dims[d] == c.expected[d]);
    }
  }
}

TEST_CASE("dim weights", "[MPIX_Dims_weighted_create]") {
  SECTION("2D example 1") {
    int nnodes = 12;
    int ndims = 2;
    double dim_weights[] = {1. / 6., 1. / 18.};
    int dims[] = {0, 0};
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(dims[0] == 2);
    REQUIRE(dims[1] == 6);
    REQUIRE(ret == MPI_SUCCESS);
  }
  SECTION("2D example 2") {
    int nnodes = 3;
    int ndims = 2;
    double dim_weights[] = {1. / 4., 1. / 12.};
    int dims[] = {0, 0};
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(dims[0] == 1);
    REQUIRE(dims[1] == 3);
    REQUIRE(ret == MPI_SUCCESS);
  }
  SECTION("3D example") {
    int nnodes = 3 * 4 * 24;
    int ndims = 3;
    double dim_weights[] = {1. / 40., 1. / 240., 1. / 30.};
    int dims[] = {0, 0, 0};
    int ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
    REQUIRE(dims[0] == 4);
    REQUIRE(dims[1] == 24);
    REQUIRE(dims[2] == 3);
    REQUIRE(ret == MPI_SUCCESS);
  }
}

// TODO
TEST_CASE("fixed dimensions stay", "[.][MPIX_Dims_weighted_create]") {
  int nnodes = 1;
  int ndims = 1;
  double *dim_weights = MPIX_EQUAL_WEIGHTS;
  int *dims = NULL;
  int ret = MPI_SUCCESS;

  nnodes = 30;
  ndims = 3;
  dims = std::array<int, 3>({3, 0, 0}).data();
  ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  REQUIRE(dims[0] == 3);
  dims = std::array<int, 3>({0, 3, 0}).data();
  ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  REQUIRE(dims[1] == 3);
  dims = std::array<int, 3>({0, 0, 3}).data();
  ret = MPIX_Dims_weighted_create(nnodes, ndims, dim_weights, dims);
  REQUIRE(dims[2] == 3);
  REQUIRE(ret == MPI_SUCCESS);
}
