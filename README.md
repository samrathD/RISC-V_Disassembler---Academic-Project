This is a simple RISC-V Disassembler implemented in the C programming language.

Description 

The RISC-V Disassembler is designed to take RISC-V machine code instructions and convert them into human-readable assembly language instructions. It parses the binary machine code and produces the corresponding assembly language representation. This program was part of a course, CMPT 295 at Simon Fraser University. Various files in been included to test the program.

Features Disassembly 
- Converts RISC-V machine code to assembly language instructions. 
- User-Friendly: Provides a human-readable output for a better understanding of machine code.

How to run the program - 
1) Type the following command in terminal "make riscv"
2) Type "./riscv -d" followed by the path to the input file. Various input file have been included to test the program, check the "code" then "input" folder. There are also reference files included in "code". The final command would look something like this "./riscv -d ./code/input/R/R.input". 