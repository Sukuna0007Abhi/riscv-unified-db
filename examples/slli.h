/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 *
 * Auto-generated from RISC-V UDB YAML: slli.yaml
 * Instruction: slli
 */

#ifndef RISCV_INST_SLLI_H
#define RISCV_INST_SLLI_H

/* Instruction Information */
#define INST_SLLI_NAME "slli"
#define INST_SLLI_LONG_NAME "Shift left logical immediate"
#define INST_SLLI_DESCRIPTION "Shift the value in xs1 left by shamt, and store the result in xd"
#define INST_SLLI_DEFINED_BY "I"
#define INST_SLLI_ASSEMBLY "xd, xs1, shamt"

/* Encoding Information */
#define INST_SLLI_MATCH_PATTERN ""

/* Encoding Variables */
/*
    /* No encoding variables */
*/

/* Access Modes */
#define INST_SLLI_ACCESS_S "always"
#define INST_SLLI_ACCESS_U "always"
#define INST_SLLI_ACCESS_VS "always"
#define INST_SLLI_ACCESS_VU "always"

/* Data Independent Timing */
#define INST_SLLI_DATA_INDEPENDENT_TIMING true

/* Operation */
#define INST_SLLI_OPERATION "# shamt is between 0-(XLEN-1)\nX[xd] = X[xs1] << shamt;\n"

/* Instruction Structure */
typedef struct {
    const char* name;
    const char* long_name;
    const char* description;
    const char* defined_by;
    const char* assembly;
    const char* match_pattern;
    const char* access_s;
    const char* access_u;
    const char* access_vs;
    const char* access_vu;
    int data_independent_timing;
    const char* operation;
} riscv_instruction_slli_t;

/* Global instance */
extern const riscv_instruction_slli_t riscv_inst_slli;

/* Inline getter function */
static inline const riscv_instruction_slli_t* get_riscv_inst_slli(void) {
    static const riscv_instruction_slli_t inst = {
        .name = INST_SLLI_NAME,
        .long_name = INST_SLLI_LONG_NAME,
        .description = INST_SLLI_DESCRIPTION,
        .defined_by = INST_SLLI_DEFINED_BY,
        .assembly = INST_SLLI_ASSEMBLY,
        .match_pattern = INST_SLLI_MATCH_PATTERN,
        .access_s = INST_SLLI_ACCESS_S,
        .access_u = INST_SLLI_ACCESS_U,
        .access_vs = INST_SLLI_ACCESS_VS,
        .access_vu = INST_SLLI_ACCESS_VU,
        .data_independent_timing = INST_SLLI_DATA_INDEPENDENT_TIMING,
        .operation = INST_SLLI_OPERATION
    };
    return &inst;
}

#endif /* RISCV_INST_SLLI_H */
