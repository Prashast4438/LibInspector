# LibInspector

## Overview

**LibInspector** is a C++ project designed to inspect and verify the libraries included in C++ source code files. The tool scans through the code, identifies the libraries that are included via `#include` statements, and checks if any untrusted libraries are used. This helps developers ensure the use of safe and trusted libraries in their code, preventing security risks and potential vulnerabilities due to untrusted dependencies.

The program supports recursive scanning of C++ files in a given directory, providing an organized table output that includes the file name, folder name, and any untrusted libraries found.

## Features

- **Library Inspection**: Scans C++ source files for `#include` statements and extracts the included libraries.
- **Trusted Libraries**: Uses a predefined list of trusted libraries to cross-check against the included libraries.
- **Untrusted Library Detection**: Flags any libraries that are not part of the trusted list and displays them.
- **Directory Scanning**: Recursively scans all `.cpp` files in the specified directory.
- **Formatted Output**: Displays results in a clean and easy-to-read table format, with proper wrapping of long library names.

## How It Works

1. The program reads C++ files in the specified directory.
2. It extracts the list of libraries included using the `#include` directive.
3. It compares the libraries found in the code with a list of trusted libraries.
4. It flags any libraries that are not in the trusted list as "untrusted" and displays them.
5. Results are shown in a neatly formatted table with columns for the file name, folder name, and untrusted libraries.

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/LibInspector.git
    cd LibInspector
    ```

2. Build the project using a C++ compiler (such as `g++`):

    ```bash
    g++ -o checker checker.cpp
    ```

3. Run the program by specifying the directory path you want to scan:

    ```bash
    ./checker /path/to/your/directory
    ```

## Sample File

To test the program, you can use a sample C++ file. Below is an example of a sample file named `example.cpp` that includes some libraries:

```cpp
#include <iostream>
#include <cmath>
#include <vector>
#include <nonexistentlib.h> // This is an untrusted library

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
