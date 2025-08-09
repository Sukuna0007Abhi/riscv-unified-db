#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* RISC-V instruction data structure */
typedef struct {
    char name[256];
    char long_name[1024];
    char description[2048];
    char defined_by[64];
    char assembly[256];
    char match_pattern[64];
    char access_s[32];
    char access_u[32];
    char access_vs[32];
    char access_vu[32];
    int data_independent_timing;
    char operation[1024];
} instruction_data_t;

/* Convert C escape sequences back to original characters */
void unescape_string(char* dest, const char* src, size_t dest_size) {
    size_t i = 0, j = 0;
    
    while (src[i] && j < dest_size - 1) {
        if (src[i] == '\\' && src[i+1]) {
            switch (src[i+1]) {
                case 'n': dest[j++] = '\n'; i += 2; break;
                case 't': dest[j++] = '\t'; i += 2; break;
                case 'r': dest[j++] = '\r'; i += 2; break;
                case '\\': dest[j++] = '\\'; i += 2; break;
                case '"': dest[j++] = '"'; i += 2; break;
                default: dest[j++] = src[i++]; break;
            }
        } else {
            dest[j++] = src[i++];
        }
    }
    dest[j] = '\0';
}

/* Extract string value from #define line */
int extract_define_string(const char* line, const char* prefix, char* result, size_t result_size) {
    char* pos = strstr(line, prefix);
    if (!pos) return 0;
    
    pos += strlen(prefix);
    while (*pos && isspace(*pos)) pos++;
    
    if (*pos == '"') {
        pos++;
        char* end = strrchr(pos, '"');
        if (end) {
            size_t len = end - pos;
            if (len < result_size) {
                strncpy(result, pos, len);
                result[len] = '\0';
                unescape_string(result, result, result_size);
                return 1;
            }
        }
    }
    return 0;
}

/* Extract boolean value from #define line */
int extract_define_bool(const char* line, const char* prefix) {
    char* pos = strstr(line, prefix);
    if (!pos) return 0;
    
    pos += strlen(prefix);
    while (*pos && isspace(*pos)) pos++;
    
    return (strncmp(pos, "true", 4) == 0) ? 1 : 0;
}

/* Parse C header file and extract instruction data */
int parse_header_file(const char* filename, instruction_data_t* inst) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open header file %s\n", filename);
        return 0;
    }
    
    char line[4096];
    memset(inst, 0, sizeof(instruction_data_t));
    
    while (fgets(line, sizeof(line), fp)) {
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        if (*trimmed == '\0' || strncmp(trimmed, "/*", 2) == 0 || *trimmed == '*') {
            continue;
        }
        
        if (strstr(line, "_NAME ")) {
            extract_define_string(line, " ", inst->name, sizeof(inst->name));
        } else if (strstr(line, "_LONG_NAME ")) {
            extract_define_string(line, " ", inst->long_name, sizeof(inst->long_name));
        } else if (strstr(line, "_DESCRIPTION ")) {
            extract_define_string(line, " ", inst->description, sizeof(inst->description));
        } else if (strstr(line, "_DEFINED_BY ")) {
            extract_define_string(line, " ", inst->defined_by, sizeof(inst->defined_by));
        } else if (strstr(line, "_ASSEMBLY ")) {
            extract_define_string(line, " ", inst->assembly, sizeof(inst->assembly));
        } else if (strstr(line, "_MATCH_PATTERN ")) {
            extract_define_string(line, " ", inst->match_pattern, sizeof(inst->match_pattern));
        } else if (strstr(line, "_ACCESS_S ")) {
            extract_define_string(line, " ", inst->access_s, sizeof(inst->access_s));
        } else if (strstr(line, "_ACCESS_U ")) {
            extract_define_string(line, " ", inst->access_u, sizeof(inst->access_u));
        } else if (strstr(line, "_ACCESS_VS ")) {
            extract_define_string(line, " ", inst->access_vs, sizeof(inst->access_vs));
        } else if (strstr(line, "_ACCESS_VU ")) {
            extract_define_string(line, " ", inst->access_vu, sizeof(inst->access_vu));
        } else if (strstr(line, "_DATA_INDEPENDENT_TIMING ")) {
            inst->data_independent_timing = extract_define_bool(line, " ");
        } else if (strstr(line, "_OPERATION ")) {
            extract_define_string(line, " ", inst->operation, sizeof(inst->operation));
        }
    }
    
    fclose(fp);
    return 1;
}

/* Print YAML string with proper formatting and indentation */
void print_yaml_string(FILE* fp, const char* key, const char* value, int indent) {
    for (int i = 0; i < indent; i++) fprintf(fp, "  ");
    
    if (!value || strlen(value) == 0) {
        fprintf(fp, "%s: \"\"\n", key);
        return;
    }
    
    /* Handle multiline strings with block scalar format */
    if (strchr(value, '\n')) {
        fprintf(fp, "%s: |\n", key);
        const char* line_start = value;
        const char* line_end;
        
        while ((line_end = strchr(line_start, '\n')) != NULL) {
            for (int i = 0; i < indent + 1; i++) fprintf(fp, "  ");
            fprintf(fp, "%.*s\n", (int)(line_end - line_start), line_start);
            line_start = line_end + 1;
        }
        
        if (*line_start) {
            for (int i = 0; i < indent + 1; i++) fprintf(fp, "  ");
            fprintf(fp, "%s\n", line_start);
        }
    } else {
        fprintf(fp, "%s: \"%s\"\n", key, value);
    }
}

void print_yaml_bool(FILE* fp, const char* key, int value, int indent) {
    for (int i = 0; i < indent; i++) fprintf(fp, "  ");
    fprintf(fp, "%s: %s\n", key, value ? "true" : "false");
}

/* Write complete YAML instruction file */
void write_instruction_yaml(FILE* fp, const instruction_data_t* inst, const char* output_filename) {
    fprintf(fp, "# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.\n");
    fprintf(fp, "# SPDX-License-Identifier: BSD-3-Clause-Clear\n");
    fprintf(fp, "# Generated from C header by c_to_yaml program\n");
    fprintf(fp, "# Output file: %s\n", output_filename);
    fprintf(fp, "\n");
    
    fprintf(fp, "$schema: \"inst_schema.json#\"\n");
    fprintf(fp, "kind: instruction\n");
    
    print_yaml_string(fp, "name", inst->name, 0);
    print_yaml_string(fp, "long_name", inst->long_name, 0);
    print_yaml_string(fp, "description", inst->description, 0);
    print_yaml_string(fp, "definedBy", inst->defined_by, 0);
    print_yaml_string(fp, "assembly", inst->assembly, 0);
    
    fprintf(fp, "encoding:\n");
    print_yaml_string(fp, "match", inst->match_pattern, 1);
    fprintf(fp, "  variables: []\n");
    
    fprintf(fp, "access:\n");
    print_yaml_string(fp, "s", inst->access_s, 1);
    print_yaml_string(fp, "u", inst->access_u, 1);
    print_yaml_string(fp, "vs", inst->access_vs, 1);
    print_yaml_string(fp, "vu", inst->access_vu, 1);
    
    print_yaml_bool(fp, "data_independent_timing", inst->data_independent_timing, 0);
    
    fprintf(fp, "operation(): ");
    if (strlen(inst->operation) > 0) {
        fprintf(fp, "|\n  %s\n", inst->operation);
    } else {
        fprintf(fp, "|\n\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_header.h> <output.yaml>\n", argv[0]);
        return 1;
    }
    
    const char* header_file = argv[1];
    const char* yaml_file = argv[2];
    
    instruction_data_t inst;
    
    if (!parse_header_file(header_file, &inst)) {
        fprintf(stderr, "Error: Failed to parse header file %s\n", header_file);
        return 1;
    }
    
    FILE* fp = fopen(yaml_file, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open output file %s\n", yaml_file);
        return 1;
    }
    
    printf("Converting C header %s to YAML %s\n", header_file, yaml_file);
    printf("Extracted instruction: %s\n", inst.name);
    
    write_instruction_yaml(fp, &inst, yaml_file);
    
    fclose(fp);
    
    printf("Successfully generated %s\n", yaml_file);
    return 0;
}
