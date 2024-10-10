# Password Cracker

## Description
This project is a C++ implementation of a password-cracking tool designed to demonstrate understanding of cryptographic concepts, file handling, and efficient algorithm implementation. It supports cracking both MD5 and SHA256 hashes using dictionary attacks.

## Features
- Crack individual MD5 or SHA256 hashes
- Batch process multiple hashes from a file
- Support for custom dictionary files
- Real-time progress monitoring with the option to stop the process
- Performance timing for cracking operations

## Technologies Used
- C++
- OpenSSL library for cryptographic functions
- Standard Template Library (STL)

## How to Compile and Run

Ensure you have a C++ compiler and OpenSSL installed on your system.
### I compile and run the program with Makefile and using MinGW32 Make (Windows)
1. Compile the program:
   ```bash
   mingw32-make.exe
   ```
2. Run the compiled program:
   ```bash
   .\programmet-debug.exe
   ```

### Choose from the available options:
- **Crack a single hash**
- **Crack multiple hashes from a file**
- **Exit the program**

## Ethical Considerations
This tool is designed for educational purposes and to demonstrate programming skills. It should only be used on systems and data that you have explicit permission to test. Unauthorized use of this tool may be illegal and unethical.
