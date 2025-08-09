#!/usr/bin/env python3
"""
RISC-V YAML to C Header Converter
Converts RISC-V instruction YAML files to C header format with #define statements
"""

import yaml
import sys
import os
import re
from typing import Dict, Any, List

def sanitize_c_identifier(name: str) -> str:
    """Convert YAML key to valid C identifier"""
    # Replace dots and other special chars with underscores
    sanitized = re.sub(r'[^a-zA-Z0-9_]', '_', name)
    # Ensure it doesn't start with a number
    if sanitized and sanitized[0].isdigit():
        sanitized = 'inst_' + sanitized
    return sanitized.upper()

def escape_c_string(text: str) -> str:
    """Escape string for C header format"""
    if not text:
        return '""'
    # Replace newlines and quotes
    escaped = text.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')
    return f'"{escaped}"'

def format_encoding_variables(variables: List[Dict]) -> str:
    """Format encoding variables as C struct"""
    if not variables:
        return "    /* No encoding variables */\n"
    
    result = ""
    for var in variables:
        name = var.get('name', 'unknown')
        location = var.get('location', 'unknown')
        result += f"    /* {name}: {location} */\n"
    return result

def yaml_to_c_header(yaml_file: str, output_file: str) -> bool:
    """Convert YAML instruction file to C header"""
    try:
        with open(yaml_file, 'r', encoding='utf-8') as f:
            data = yaml.safe_load(f)
        
        if not data or data.get('kind') != 'instruction':
            print(f"Error: {yaml_file} is not a valid instruction YAML file")
            return False
        
        # Extract key fields
        inst_name = data.get('name', 'unknown')
        long_name = data.get('long_name', '')
        description = data.get('description', '').strip()
        defined_by = data.get('definedBy', '')
        assembly = data.get('assembly', '')
        
        # Encoding information
        encoding = data.get('encoding', {})
        match_pattern = encoding.get('match', '')
        variables = encoding.get('variables', [])
        
        # Access modes
        access = data.get('access', {})
        
        # Operation
        operation = data.get('operation()', '')
        
        # Generate C header
        c_identifier = sanitize_c_identifier(inst_name)
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(f"""/* 
 * Auto-generated from RISC-V UDB YAML: {os.path.basename(yaml_file)}
 * Instruction: {inst_name}
 */

#ifndef RISCV_INST_{c_identifier}_H
#define RISCV_INST_{c_identifier}_H

/* Instruction Information */
#define INST_{c_identifier}_NAME "{inst_name}"
#define INST_{c_identifier}_LONG_NAME {escape_c_string(long_name)}
#define INST_{c_identifier}_DESCRIPTION {escape_c_string(description)}
#define INST_{c_identifier}_DEFINED_BY "{defined_by}"
#define INST_{c_identifier}_ASSEMBLY "{assembly}"

/* Encoding Information */
#define INST_{c_identifier}_MATCH_PATTERN "{match_pattern}"

/* Encoding Variables */
/*
{format_encoding_variables(variables)}*/

/* Access Modes */
#define INST_{c_identifier}_ACCESS_S "{access.get('s', 'unknown')}"
#define INST_{c_identifier}_ACCESS_U "{access.get('u', 'unknown')}"
#define INST_{c_identifier}_ACCESS_VS "{access.get('vs', 'unknown')}"
#define INST_{c_identifier}_ACCESS_VU "{access.get('vu', 'unknown')}"

/* Data Independent Timing */
#define INST_{c_identifier}_DATA_INDEPENDENT_TIMING {str(data.get('data_independent_timing', False)).lower()}

/* Operation */
#define INST_{c_identifier}_OPERATION {escape_c_string(operation)}

/* Instruction Structure */
typedef struct {{
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
}} riscv_instruction_{c_identifier.lower()}_t;

/* Global instance */
extern const riscv_instruction_{c_identifier.lower()}_t riscv_inst_{c_identifier.lower()};

/* Inline getter function */
static inline const riscv_instruction_{c_identifier.lower()}_t* get_riscv_inst_{c_identifier.lower()}(void) {{
    static const riscv_instruction_{c_identifier.lower()}_t inst = {{
        .name = INST_{c_identifier}_NAME,
        .long_name = INST_{c_identifier}_LONG_NAME,
        .description = INST_{c_identifier}_DESCRIPTION,
        .defined_by = INST_{c_identifier}_DEFINED_BY,
        .assembly = INST_{c_identifier}_ASSEMBLY,
        .match_pattern = INST_{c_identifier}_MATCH_PATTERN,
        .access_s = INST_{c_identifier}_ACCESS_S,
        .access_u = INST_{c_identifier}_ACCESS_U,
        .access_vs = INST_{c_identifier}_ACCESS_VS,
        .access_vu = INST_{c_identifier}_ACCESS_VU,
        .data_independent_timing = INST_{c_identifier}_DATA_INDEPENDENT_TIMING,
        .operation = INST_{c_identifier}_OPERATION
    }};
    return &inst;
}}

#endif /* RISCV_INST_{c_identifier}_H */
""")
        
        print(f"Successfully converted {yaml_file} to {output_file}")
        return True
        
    except Exception as e:
        print(f"Error converting {yaml_file}: {e}")
        return False

def main():
    if len(sys.argv) != 3:
        print("Usage: python yaml_to_c.py <input.yaml> <output.h>")
        sys.exit(1)
    
    yaml_file = sys.argv[1]
    output_file = sys.argv[2]
    
    if not os.path.exists(yaml_file):
        print(f"Error: Input file {yaml_file} does not exist")
        sys.exit(1)
    
    success = yaml_to_c_header(yaml_file, output_file)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
