/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 *
 * Auto-generated from RISC-V UDB YAML: fence.yaml
 * Instruction: fence
 */

#ifndef RISCV_INST_FENCE_H
#define RISCV_INST_FENCE_H

/* Instruction Information */
#define INST_FENCE_NAME "fence"
#define INST_FENCE_LONG_NAME "Memory ordering fence"
#define INST_FENCE_DESCRIPTION "Orders memory operations.\n\nThe `fence` instruction is used to order device I/O and memory accesses as\nviewed by other RISC-V harts and external devices or coprocessors. Any\ncombination of device input (I), device output (O), memory reads \\(R),\nand memory writes (W) may be ordered with respect to any combination of\nthe same. Informally, no other RISC-V hart or external device can\nobserve any operation in the _successor_ set following a `fence` before\nany operation in the _predecessor_ set preceding the `fence`.\n\nThe predecessor and successor fields have the same format to specify operation types:\n\n[%autowidth]\n|===\n4+| `pred` 4+| `succ`\n\n| 27 | 26 |25 | 24 | 23 | 22 | 21| 20\n| PI | PO |PR | PW | SI | SO |SR | SW\n|===\n\n[%autowidth,align=\"center\",cols=\"^1,^1,<3\",options=\"header\"]\n.Fence mode encoding\n|===\n|_fm_ field |Mnemonic |Meaning\n|0000 |_none_ |Normal Fence\n|1000 |TSO |With `FENCE RW,RW`: exclude write-to-read ordering; otherwise: _Reserved for future use._\n2+|_other_ |_Reserved for future use._\n|===\n\nWhen the mode field _fm_ is `0001` and both the predecessor and successor sets are 'RW',\nthen the instruction acts as a special-case `fence.tso`. `fence.tso` orders all load operations\nin its predecessor set before all memory operations in its successor set, and all store operations\nin its predecessor set before all store operations in its successor set. This leaves non-AMO store\noperations in the 'fence.tso's predecessor set unordered with non-AMO loads in its successor set.\n\nWhen mode field _fm_ is not `0001`, or when mode field _fm_ is `0001` but the _pred_ and\n_succ_ fields are not both 'RW' (0x3), then the fence acts as a baseline fence (_e.g._, _fm_ is\neffectively `0000`). This is unaffected by the FIOM bits, described below (implicit promotion does\nnot change how `fence.tso` is decoded).\n\nThe `xs1` and `xd` fields are unused and ignored.\n\nIn modes other than M-mode, `fence` is further affected by `menvcfg.FIOM`,\n`senvcfg.FIOM`<% if ext?(:H) %>, and/or `henvcfg.FIOM`<% end %>\nas follows:\n\n.Effective PR/PW/SR/SW in (H)S-mode\n[%autowidth,cols=\",,,\",options=\"header\",separator=\"!\"]\n!===\n! [.rotate]#`menvcfg.FIOM`# ! `pred.PI` +\n`pred.PO` +\n`succ.SI` +\n`succ.SO`\n! -> +\n-> +\n-> +\n->\n! effective `PR` +\neffective `PW` +\neffective `SR` +\neffective `SW`\n\n! 0 ! - ! ! from encoding\n! 1 ! 0 ! ! from encoding\n! 1 ! 1 ! ! 1\n!===\n\n.Effective PR/PW/SR/SW in U-mode\n[%autowidth,options=\"header\",separator=\"!\",cols=\",,,,\"]\n!===\n! [.rotate]#`menvcfg.FIOM`# ! [.rotate]#`senvcfg.FIOM`# !  `pred.PI` +\n`pred.PO` +\n`succ.SI` +\n`succ.SO`\n! -> +\n-> +\n-> +\n->\n! effective `PR` +\neffective `PW` +\neffective `SR` +\neffective `SW`\n\n! 0 ! 0 ! - ! ! from encoding\n! 0 ! 1 ! 0 ! ! from encoding\n! 0 ! 1 ! 1 ! ! 1\n! 1 ! - ! 0 ! ! from encoding\n! 1 ! - ! 1 ! ! 1\n!===\n\n<%- if ext?(:H) -%>\n.Effective PR/PW/SR/SW in VS-mode and VU-mode\n[%autowidth,options=\"header\",separator=\"!\",cols=\",,,,\"]\n!===\n! [.rotate]#`menvcfg.FIOM`# ! [.rotate]#`henvcfg.FIOM`# !  `pred.PI` +\n`pred.PO` +\n`succ.SI` +\n`succ.SO`\n! -> +\n-> +\n-> +\n->\n! effective `PR` +\neffective `PW` +\neffective `SR` +\neffective `SW`\n\n! 0 ! 0 ! - ! ! from encoding\n! 0 ! 1 ! 0 ! ! from encoding\n! 0 ! 1 ! 1 ! ! 1\n! 1 ! - ! 0 ! ! from encoding\n! 1 ! - ! 1 ! ! 1\n!===\n<%- end -%>"
#define INST_FENCE_DEFINED_BY "I"
#define INST_FENCE_ASSEMBLY "pred, succ"

/* Encoding Information */
#define INST_FENCE_MATCH_PATTERN "-----------------000-----0001111"

/* Encoding Variables */
/*
    /* fm: 31-28 */
    /* pred: 27-24 */
    /* succ: 23-20 */
    /* xs1: 19-15 */
    /* xd: 11-7 */
*/

/* Access Modes */
#define INST_FENCE_ACCESS_S "always"
#define INST_FENCE_ACCESS_U "always"
#define INST_FENCE_ACCESS_VS "always"
#define INST_FENCE_ACCESS_VU "always"

/* Data Independent Timing */
#define INST_FENCE_DATA_INDEPENDENT_TIMING false

/* Operation */
#define INST_FENCE_OPERATION "Boolean is_pause;\n\nif (implemented?(ExtensionName::Zihintpause)) {\n  if ((pred == 1) && (succ == 0) && (xd == 0) && (xs1 == 0)) {\n    # this is a PAUSE instruction\n    is_pause = true;\n  }\n}\n\nBoolean pred_i = pred[3] == 1;\nBoolean pred_o = pred[2] == 1;\nBoolean pred_r = pred[1] == 1;\nBoolean pred_w = pred[0] == 1;\n\nBoolean succ_i = succ[3] == 1;\nBoolean succ_o = succ[2] == 1;\nBoolean succ_r = succ[1] == 1;\nBoolean succ_w = succ[0] == 1;\n\nif (is_pause) {\n  pause();\n} else {\n\n  # apply FIOM overrides\n  if (mode() == PrivilegeMode::S) {\n    if (CSR[menvcfg].FIOM == 1) {\n      if (pred_i) { pred_r = true; }\n      if (pred_o) { pred_w = true; }\n      if (succ_i) { succ_r = true; }\n      if (succ_o) { succ_w = true; }\n    }\n  } else if (mode() == PrivilegeMode::U) {\n    if ((CSR[menvcfg].FIOM | CSR[senvcfg].FIOM) == 1) {\n      if (pred_i) { pred_r = true; }\n      if (pred_o) { pred_w = true; }\n      if (succ_i) { succ_r = true; }\n      if (succ_o) { succ_w = true; }\n    }\n  } else if (mode() == PrivilegeMode::VS || mode() == PrivilegeMode::VU) {\n    if ((CSR[menvcfg].FIOM | CSR[henvcfg].FIOM) == 1) {\n      if (pred_i) { pred_r = true; }\n      if (pred_o) { pred_w = true; }\n      if (succ_i) { succ_r = true; }\n      if (succ_o) { succ_w = true; }\n    }\n  }\n\n  fence(\n    pred_i, pred_o, pred_r, pred_w,\n    succ_i, succ_o, succ_r, succ_w\n  );\n}\n"

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
} riscv_instruction_fence_t;

/* Global instance */
extern const riscv_instruction_fence_t riscv_inst_fence;

/* Inline getter function */
static inline const riscv_instruction_fence_t* get_riscv_inst_fence(void) {
    static const riscv_instruction_fence_t inst = {
        .name = INST_FENCE_NAME,
        .long_name = INST_FENCE_LONG_NAME,
        .description = INST_FENCE_DESCRIPTION,
        .defined_by = INST_FENCE_DEFINED_BY,
        .assembly = INST_FENCE_ASSEMBLY,
        .match_pattern = INST_FENCE_MATCH_PATTERN,
        .access_s = INST_FENCE_ACCESS_S,
        .access_u = INST_FENCE_ACCESS_U,
        .access_vs = INST_FENCE_ACCESS_VS,
        .access_vu = INST_FENCE_ACCESS_VU,
        .data_independent_timing = INST_FENCE_DATA_INDEPENDENT_TIMING,
        .operation = INST_FENCE_OPERATION
    };
    return &inst;
}

#endif /* RISCV_INST_FENCE_H */
