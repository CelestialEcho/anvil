   ‎   ‎   ‎   ‎   ‎   ‎   ‎   ‎   ‎   ‎![logo_image](https://github.com/CelestialEcho/anvil/blob/main/git_assets/logo_with_text.png?raw=true)
---
[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#) [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#)

**Anvil** is a lightweight and fast command-line build system for C/C++ projects. No complex configuration files, no bloated toolchains, only a simple JSON-based project file and a handful of commands.
 
---

## Features
 
- Simple JSON-based project configuration (`anvil.json`)
- Manage source files and compiler flags from the command line
- Fast builds powered directly by your system compiler
- Minimal setup — no dependencies beyond a C++ compiler

---
 
## Installation
 
### Build from source
 
1. Clone the repository:
   ```bash
   git clone https://github.com/CelestialEcho/anvil.git
   cd anvil
   ```
 
2. Run the bootstrap build script:
   ```bat
   build.bat
   ```
 
3. Add the resulting `anvil.exe` to your `PATH`.
> **Requirements:** A C++17-compatible compiler (e.g. `g++`, `clang++`, or MSVC) must be available in your `PATH`.
 
---
 
## Quick Start
 
Initialize a new Anvil project in your directory:
 
```bash
anvil init
```
 
This creates an `anvil.json` file in the current directory. From there, add your source files and configure compiler flags:
 
```bash
anvil include main.cpp
anvil include src/mylib.cpp
anvil add-arg -std=c++17
anvil add-arg -o bin/myapp.exe
anvil build
```
 
---
 
## Project File
 
Anvil stores project configuration in `anvil.json` at the root of your project. You can edit it directly or use the CLI commands to manage it.
 
**Example `anvil.json`:**
 
```json
{
    "ANVIL_VERSION": "1.0.0",
    "BUILDER_INFO": "Anvil CLI C/C++ builder, check out more on github.",
    "PROJECT_NAME": "my_project",
    "flags": [
        {
            "flag": "-std=c++17",
            "value": null
        },
        {
            "flag": "-o",
            "value": "bin/my_project.exe"
        },
        {
            "flag": "-O2",
            "value": null
        }
    ],
    "includes": [
        "main.cpp",
        "src\\mylib.cpp"
    ]
}
```
 
**Fields:**
 
| Field | Description |
|---|---|
| `ANVIL_VERSION` | Version of Anvil used to create this project file |
| `PROJECT_NAME` | Name of the project |
| `flags` | List of compiler flags. Each entry has a `flag` and an optional `value` |
| `includes` | List of source files to compile |
 
---
 
## Commands
 
```
anvil <command> [args]
```
 
| Command | Description |
|---|---|
| `version` | Print the current version of Anvil |
| `init` | Initialize a new Anvil project in the current directory |
| `include <path>` | Add a source file or directory to the build |
| `exclude <path>` | Remove a source file from the build |
| `list-files` | List all currently included source files |
| `clear` | Remove all included source files |
| `add-arg <flag> [value]` | Add a compiler flag, optionally with a value |
| `remove-arg <flag>` | Remove a compiler flag |
| `list-args` | List all currently applied compiler flags |
| `build` | Compile the project |
 
---
 
## Examples
 
### Initialize and build a project
 
```bash
anvil init
anvil include main.cpp
anvil include src/engine.cpp
anvil add-arg -std=c++17
anvil add-arg -O2
anvil add-arg -o bin/app.exe
anvil build
```
 
### Add an include path
 
```bash
anvil add-arg -I include/
```
 
### Link a library
 
```bash
anvil add-arg -l SDL2
```
 
### Remove a flag
 
```bash
anvil remove-arg -O2
```
 
### Check your current configuration
 
```bash
anvil list-files
anvil list-args
```
 
---
 
## License
This project is licensed under the [MIT License](LICENSE).
