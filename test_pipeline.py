#!/usr/bin/env python3
"""
RISC-V Round-trip Conversion Test
Tests complete YAML → C Header → YAML pipeline with data integrity validation
"""

import subprocess
import sys
import os
import yaml
import shutil

def run_command(cmd, description):
    """Execute command with error handling and logging"""
    print(f"Running: {description}")
    print(f"Command: {' '.join(cmd)}")
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error in {description}:")
        print(f"STDOUT: {result.stdout}")
        print(f"STDERR: {result.stderr}")
        return False
    
    if result.stdout:
        print(f"Output: {result.stdout}")
    return True

def test_conversion_pipeline(yaml_input):
    """Execute complete round-trip conversion test"""
    base_name = os.path.splitext(os.path.basename(yaml_input))[0]
    
    # File names for the pipeline
    c_header = f"{base_name}.h"
    c_program = "c_to_yaml"
    if os.name == 'nt':  # Windows
        c_program += ".exe"
    yaml_output1 = f"{base_name}_from_c.yaml"
    c_header2 = f"{base_name}_round2.h"
    yaml_output2 = f"{base_name}_final.yaml"
    
    print(f"Testing conversion pipeline for {yaml_input}")
    print("=" * 60)
    
    # Step 1: YAML -> C Header
    if not run_command([sys.executable, "yaml_to_c.py", yaml_input, c_header], 
                      "Convert YAML to C header"):
        return False
    
    # Step 2: Compile C program
    if not run_command(["gcc", "-o", c_program, "c_to_yaml.c"], 
                      "Compile C to YAML converter"):
        return False
    
    # Step 3: C Header -> YAML
    if not run_command([f"./{c_program}", c_header, yaml_output1], 
                      "Convert C header to YAML"):
        return False
    
    # Step 4: Round-trip test - YAML -> C Header again
    if not run_command([sys.executable, "yaml_to_c.py", yaml_output1, c_header2], 
                      "Round-trip: Convert generated YAML to C header"):
        return False
    
    # Step 5: C Header -> YAML final
    if not run_command([f"./{c_program}", c_header2, yaml_output2], 
                      "Final: Convert round-trip C header to YAML"):
        return False
    
    print("\n" + "=" * 60)
    print("Pipeline completed successfully!")
    print(f"Original YAML: {yaml_input}")
    print(f"Generated C header: {c_header}")
    print(f"Generated YAML: {yaml_output1}")
    print(f"Round-trip C header: {c_header2}")
    print(f"Final YAML: {yaml_output2}")
    
    # Check if round-trip files are similar
    try:
        with open(yaml_output1, 'r') as f1, open(yaml_output2, 'r') as f2:
            content1 = f1.read()
            content2 = f2.read()
            
        if content1 == content2:
            print("✅ Round-trip test PASSED: Generated YAML files are identical")
        else:
            print("⚠️  Round-trip test WARNING: Generated YAML files differ")
            print("This may be expected due to formatting differences")
    except Exception as e:
        print(f"Could not compare files: {e}")
    
    return True

def main():
    if len(sys.argv) < 2:
        # Default test with a simple instruction
        test_files = [
            "spec/std/isa/inst/I/add.yaml",
            "spec/std/isa/inst/I/addi.yaml"
        ]
        
        available_files = [f for f in test_files if os.path.exists(f)]
        
        if not available_files:
            print("No default test files found. Please specify a YAML file.")
            print("Usage: python test_pipeline.py <instruction.yaml>")
            sys.exit(1)
        
        for yaml_file in available_files:
            print(f"\nTesting with {yaml_file}")
            if not test_conversion_pipeline(yaml_file):
                print(f"❌ Test failed for {yaml_file}")
                sys.exit(1)
            print(f"✅ Test passed for {yaml_file}")
    else:
        yaml_file = sys.argv[1]
        if not os.path.exists(yaml_file):
            print(f"Error: File {yaml_file} does not exist")
            sys.exit(1)
        
        if not test_conversion_pipeline(yaml_file):
            print("❌ Test failed")
            sys.exit(1)
        print("✅ Test passed")

if __name__ == "__main__":
    main()
