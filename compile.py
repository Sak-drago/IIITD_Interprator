#!/usr/bin/env python3
import subprocess
import sys
import argparse
import shutil
import os

parser = argparse.ArgumentParser(description="Build Script")
parser.add_argument("target", nargs='?', choices=["Src","Library", "Example", "Tests", "Doc", "All"], default="All")
parser.add_argument("--mode", choices=["DEBUG", "RELEASE"], default="DEBUG")
parser.add_argument("--clean", action="store_true")
args = parser.parse_args()

build_mode = args.mode.upper()

if args.clean:
  for d in [".build", "bin", "docs"]:
    if os.path.exists(d):
      shutil.rmtree(d)
  if os.path.exists("compile_commands.json"):
    os.remove("compile_commands.json")
  if len(sys.argv) == 2:
    sys.exit(0)

os.makedirs(".build", exist_ok=True)

# - - - Configuration Step
cmake_config = ["cmake", "-S", ".", "-B", ".build", f"-DCMAKE_BUILD_TYPE={build_mode}"]

if os.name == 'nt':
  # Force Ninja on Windows
  cmake_config += ["-G", "Ninja"]
  
  # Only set compilers if not already set via environment variables
  if not os.environ.get('CC') and not os.environ.get('CXX'):
    cmake_config += ["-DCMAKE_C_COMPILER=clang-cl", "-DCMAKE_CXX_COMPILER=clang-cl"]
  # If CC/CXX are set to clang/clang++, we need to ensure they work with Ninja on Windows
  elif os.environ.get('CC') == 'clang' or os.environ.get('CXX') == 'clang++':
    # Override to use the GNU-compatible clang explicitly
    cmake_config += ["-DCMAKE_C_COMPILER=clang", "-DCMAKE_CXX_COMPILER=clang++"]

subprocess.run(cmake_config, check=True)

# - - - LSP support
source_json = os.path.join(".build", "compile_commands.json")
if os.path.exists(source_json):
  shutil.copy(source_json, "compile_commands.json")

# - - - Target Mapping
t_map = {
  "Src"  :  "build_src",
  "Library": "build_virtualMachine",
  "Example": "build_examples",
  "Tests": "build_tests",
  "Doc": "build_docs",
  "All": "all"
}

# - - - Build Step
build_cmd = ["cmake", "--build", ".build", "--target", t_map[args.target]]
if build_mode:
  build_cmd += ["--config", build_mode]

subprocess.run(build_cmd, check=True)
