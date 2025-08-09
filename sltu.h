/* 
 * Auto-generated from RISC-V UDB YAML: sltu.yaml
 * Instruction: sltu
 */

#ifndef RISCV_INST_SLTU_H
#define RISCV_INST_SLTU_H

/* Instruction Information */
#define INST_SLTU_NAME "sltu"
#define INST_SLTU_LONG_NAME "Set on less than unsigned"
#define INST_SLTU_DESCRIPTION "Places the value 1 in register `xd` if register `xs1` is less than the value in register `xs2`, where\nboth sources are treated as unsigned numbers, else 0 is written to `xd`."
#define INST_SLTU_DEFINED_BY "I"
#define INST_SLTU_ASSEMBLY "xd, xs1, xs2"

/* Encoding Information */
#define INST_SLTU_MATCH_PATTERN "0000000----------011-----0110011"

/* Encoding Variables */
/*
    /* xs2: 24-20 */
    /* xs1: 19-15 */
    /* xd: 11-7 */
*/

/* Access Modes */
#define INST_SLTU_ACCESS_S "always"
#define INST_SLTU_ACCESS_U "always"
#define INST_SLTU_ACCESS_VS "always"
#define INST_SLTU_ACCESS_VU "always"

/* Data Independent Timing */
#define INST_SLTU_DATA_INDEPENDENT_TIMING true

/* Operation */
#define INST_SLTU_OPERATION "X[xd] = (X[xs1] < X[xs2]) ? 1 : 0;\n"

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
} riscv_instruction_sltu_t;

/* Global instance */
extern const riscv_instruction_sltu_t riscv_inst_sltu;

/* Inline getter function */
static inline const riscv_instruction_sltu_t* get_riscv_inst_sltu(void) {
    static const riscv_instruction_sltu_t inst = {
        .name = INST_SLTU_NAME,
        .long_name = INST_SLTU_LONG_NAME,
        .description = INST_SLTU_DESCRIPTION,
        .defined_by = INST_SLTU_DEFINED_BY,
        .assembly = INST_SLTU_ASSEMBLY,
        .match_pattern = INST_SLTU_MATCH_PATTERN,
        .access_s = INST_SLTU_ACCESS_S,
        .access_u = INST_SLTU_ACCESS_U,
        .access_vs = INST_SLTU_ACCESS_VS,
        .access_vu = INST_SLTU_ACCESS_VU,
        .data_independent_timing = INST_SLTU_DATA_INDEPENDENT_TIMING,
        .operation = INST_SLTU_OPERATION
    };
    return &inst;
}

#endif /* RISCV_INST_SLTU_H */
