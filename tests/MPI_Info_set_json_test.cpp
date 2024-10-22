/*
 * Copyright (c) 2023      High Performance Computing Center Stuttgart,
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
#include <catch2/matchers/catch_matchers_string.hpp>

#include <array>
#include <cstdint>

#include "MPI_Info_set_json.h"
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int result = Catch::Session().run(argc, argv);
  MPI_Finalize();
  return result;
}

TEST_CASE("MPI_Info_set_json is defined", "[MPI_Info_set_json]") {
  MPI_Info info;
  MPI_Info_create(&info);
  int ret = MPI_Info_set_json(info, "");
  MPI_Info_free(&info);
  REQUIRE(ret == MPI_SUCCESS);
}

TEST_CASE("MPI_Info_set_json set single pair", "[MPI_Info_set_json]") {
  MPI_Info info;
  MPI_Info_create(&info);
  int ret = MPI_Info_set_json(info, "{'key1': 'value1'}");
  REQUIRE(ret == MPI_SUCCESS);

  int valuelen = 0;
  int flag = 0;
  MPI_Info_get_valuelen(info, "key1", &valuelen, &flag);
  REQUIRE( (flag) );
  REQUIRE( valuelen == 6);

  char *value = new char[valuelen+1];
  MPI_Info_get(info, "key1", valuelen, value, &flag);
  REQUIRE_THAT(value, Catch::Matchers::Equals("value1"));

  MPI_Info_free(&info);
}

TEST_CASE("MPI_Info_set_json set multiple pairs", "[MPI_Info_set_json]") {
  MPI_Info info;
  MPI_Info_create(&info);
  int ret = MPI_Info_set_json(info, "{'key1': 'value1', 'key2': 'value2'}");
  REQUIRE(ret == MPI_SUCCESS);

  int valuelen = 0;
  int flag = 0;
  MPI_Info_get_valuelen(info, "key1", &valuelen, &flag);
  REQUIRE( (flag) );
  REQUIRE( valuelen == 6);

  char *value = new char[valuelen+1];
  MPI_Info_get(info, "key1", valuelen, value, &flag);
  REQUIRE_THAT(value, Catch::Matchers::Equals("value1"));
  delete value;

  MPI_Info_get_valuelen(info, "key2", &valuelen, &flag);
  REQUIRE( (flag) );
  REQUIRE( valuelen == 6);

  value = new char[valuelen+1];
  MPI_Info_get(info, "key2", valuelen, value, &flag);
  REQUIRE_THAT(value, Catch::Matchers::Equals("value2"));
  delete value;

  MPI_Info_free(&info);
}

