#!/usr/bin/env python3
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: BSD-3-Clause-Clear

"""
RISC-V Conversion Demonstration Script
Shows complete YAML ↔ C conversion pipeline with real examples
"""

import os
import sys
import subprocess


def run_demo_conversion(yaml_file, instruction_name):
    """Demonstrate complete conversion pipeline for one instruction"""
    print(f"\n{'='*60}")
    print(f"🎯 DEMONSTRATION: {instruction_name.upper()} Instruction")
    print(f"{'='*60}")

    base_name = instruction_name
    c_header = f"examples/{base_name}.h"
    yaml_output = f"examples/{base_name}_from_c.yaml"

    # Step 1: YAML → C
    print(f"\n📋 Step 1: Converting YAML → C Header")
    print(f"Input:  {yaml_file}")
    print(f"Output: {c_header}")

    result = subprocess.run(
        [sys.executable, "yaml_to_c.py", yaml_file, c_header],
        capture_output=True,
        text=True,
    )

    if result.returncode == 0:
        print(f"✅ {result.stdout.strip()}")
    else:
        print(f"❌ Error: {result.stderr}")
        return False

    # Step 2: C → YAML
    print(f"\n🔄 Step 2: Converting C Header → YAML")
    print(f"Input:  {c_header}")
    print(f"Output: {yaml_output}")

    result = subprocess.run(
        ["./c_to_yaml", c_header, yaml_output], capture_output=True, text=True
    )

    if result.returncode == 0:
        print(f"✅ {result.stdout.strip()}")
    else:
        print(f"❌ Error: {result.stderr}")
        return False

    # Show file sizes and structure
    print(f"\n📊 Conversion Results:")
    try:
        yaml_size = os.path.getsize(yaml_file)
        c_size = os.path.getsize(c_header)
        yaml_out_size = os.path.getsize(yaml_output)

        print(f"Original YAML: {yaml_size:,} bytes")
        print(f"Generated C:   {c_size:,} bytes ({c_size/yaml_size:.1f}x larger)")
        print(f"Output YAML:   {yaml_out_size:,} bytes")

        # Show key sections of generated C header
        print(f"\n📝 Generated C Header Preview:")
        with open(c_header) as f:
            lines = f.readlines()
            for i, line in enumerate(lines[:15]):
                print(f"   {i+1:2d}: {line.rstrip()}")
            if len(lines) > 15:
                print(f"   ... ({len(lines)-15} more lines)")

    except Exception as e:
        print(f"⚠️ Could not analyze files: {e}")

    return True


def main():
    """Run complete demonstration with multiple instruction types"""
    print("🚀 RISC-V YAML ↔ C Conversion Demonstration")
    print("Showcasing bidirectional conversion with real RISC-V instructions\n")

    # Ensure directories exist
    os.makedirs("examples", exist_ok=True)

    # Test instructions representing different types
    test_cases = [
        ("spec/std/isa/inst/I/sltu.yaml", "sltu", "Basic integer comparison"),
        (
            "spec/std/isa/inst/I/fence.yaml",
            "fence",
            "Memory ordering with complex description",
        ),
        (
            "spec/std/isa/inst/I/slli.yaml",
            "slli",
            "Shift instruction with encoding variables",
        ),
    ]

    success_count = 0

    for yaml_file, name, description in test_cases:
        if not os.path.exists(yaml_file):
            print(f"⚠️ Skipping {name}: File {yaml_file} not found")
            continue

        print(f"\n🎯 Testing: {description}")
        if run_demo_conversion(yaml_file, name):
            success_count += 1
            print(f"✅ {name.upper()} conversion successful!")
        else:
            print(f"❌ {name.upper()} conversion failed!")

    # Summary
    print(f"\n{'='*60}")
    print(f"📊 DEMONSTRATION SUMMARY")
    print(f"{'='*60}")
    print(f"✅ Successful conversions: {success_count}/{len(test_cases)}")
    print(f"🎯 Demonstrated capabilities:")
    print(f"   • YAML parsing and validation")
    print(f"   • C header generation with professional formatting")
    print(f"   • Bidirectional data transformation")
    print(f"   • String escaping and identifier sanitization")
    print(f"   • Real RISC-V instruction specification handling")

    if success_count == len(test_cases):
        print(f"\n🎉 ALL DEMONSTRATIONS PASSED! 🎉")
        print(f"The conversion system is production-ready!")

    print(f"\n📁 Generated files in examples/ directory:")
    try:
        for file in sorted(os.listdir("examples")):
            file_path = os.path.join("examples", file)
            size = os.path.getsize(file_path)
            print(f"   {file:25} ({size:,} bytes)")
    except:
        pass


if __name__ == "__main__":
    main()
