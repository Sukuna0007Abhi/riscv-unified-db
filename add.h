/* 
 * Auto-generated from RISC-V UDB YAML: add.yaml
 * Instruction: add
 */

#ifndef RISCV_INST_ADD_H
#define RISCV_INST_ADD_H

/* Instruction Information */
#define INST_ADD_NAME "add"
#define INST_ADD_LONG_NAME "Integer add"
#define INST_ADD_DESCRIPTION "Add the value in xs1 to xs2, and store the result in xd.\nAny overflow is thrown away."
#define INST_ADD_DEFINED_BY "I"
#define INST_ADD_ASSEMBLY "xd, xs1, xs2"

/* Encoding Information */
#define INST_ADD_MATCH_PATTERN "0000000----------000-----0110011"

/* Encoding Variables */
/*
    /* xs2: 24-20 */
    /* xs1: 19-15 */
    /* xd: 11-7 */
*/

/* Access Modes */
#define INST_ADD_ACCESS_S "always"
#define INST_ADD_ACCESS_U "always"
#define INST_ADD_ACCESS_VS "always"
#define INST_ADD_ACCESS_VU "always"

/* Data Independent Timing */
#define INST_ADD_DATA_INDEPENDENT_TIMING true

/* Operation */
#define INST_ADD_OPERATION "X[xd] = X[xs1] + X[xs2];"

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
} riscv_instruction_add_t;

/* Global instance */
extern const riscv_instruction_add_t riscv_inst_add;

/* Inline getter function */
static inline const riscv_instruction_add_t* get_riscv_inst_add(void) {
    static const riscv_instruction_add_t inst = {
        .name = INST_ADD_NAME,
        .long_name = INST_ADD_LONG_NAME,
        .description = INST_ADD_DESCRIPTION,
        .defined_by = INST_ADD_DEFINED_BY,
        .assembly = INST_ADD_ASSEMBLY,
        .match_pattern = INST_ADD_MATCH_PATTERN,
        .access_s = INST_ADD_ACCESS_S,
        .access_u = INST_ADD_ACCESS_U,
        .access_vs = INST_ADD_ACCESS_VS,
        .access_vu = INST_ADD_ACCESS_VU,
        .data_independent_timing = INST_ADD_DATA_INDEPENDENT_TIMING,
        .operation = INST_ADD_OPERATION
    };
    return &inst;
}

#endif /* RISCV_INST_ADD_H */
