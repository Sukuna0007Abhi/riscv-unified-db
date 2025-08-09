/* 
 * Auto-generated from RISC-V UDB YAML: sltu_from_c.yaml
 * Instruction: 
 */

#ifndef RISCV_INST__H
#define RISCV_INST__H

/* Instruction Information */
#define INST__NAME ""
#define INST__LONG_NAME ""
#define INST__DESCRIPTION ""
#define INST__DEFINED_BY ""
#define INST__ASSEMBLY ""

/* Encoding Information */
#define INST__MATCH_PATTERN ""

/* Encoding Variables */
/*
    /* No encoding variables */
*/

/* Access Modes */
#define INST__ACCESS_S ""
#define INST__ACCESS_U ""
#define INST__ACCESS_VS ""
#define INST__ACCESS_VU ""

/* Data Independent Timing */
#define INST__DATA_INDEPENDENT_TIMING false

/* Operation */
#define INST__OPERATION ""

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
} riscv_instruction__t;

/* Global instance */
extern const riscv_instruction__t riscv_inst_;

/* Inline getter function */
static inline const riscv_instruction__t* get_riscv_inst_(void) {
    static const riscv_instruction__t inst = {
        .name = INST__NAME,
        .long_name = INST__LONG_NAME,
        .description = INST__DESCRIPTION,
        .defined_by = INST__DEFINED_BY,
        .assembly = INST__ASSEMBLY,
        .match_pattern = INST__MATCH_PATTERN,
        .access_s = INST__ACCESS_S,
        .access_u = INST__ACCESS_U,
        .access_vs = INST__ACCESS_VS,
        .access_vu = INST__ACCESS_VU,
        .data_independent_timing = INST__DATA_INDEPENDENT_TIMING,
        .operation = INST__OPERATION
    };
    return &inst;
}

#endif /* RISCV_INST__H */
