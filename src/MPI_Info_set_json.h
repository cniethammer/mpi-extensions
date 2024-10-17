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

#ifndef SRC_MPI_INFO_CREATE_FROM_JSON_H_
#define SRC_MPI_INFO_CREATE_FROM_JSON_H_

#include <mpi.h>

#if __cplusplus
extern "C" {
#endif


/** MPI_Info_set_json adds (key,value) pairs to an MPI info input as a JSON string
 *
 * MPI_Info_set_json sets info key-value pairs for an info object from a JSON string
 *
 * Example call:
 *   MPI_Info_set_json(info, "{'key1': 'value1', 'key2': 'value2'}");
 *
 * @param[inout] info
 * @param[in]    json_str JSON string with dict containing key: value pairs
 */
int MPI_Info_set_json(MPI_Info info, const char *json_str);

/** PMPI interface corresponding to MPI call */
int PMPI_Info_set_json(MPI_Info info, const char *json_str);

#if __cplusplus
}
#endif

#endif // SRC_MPI_INFO_CREATE_FROM_JSON_H_
