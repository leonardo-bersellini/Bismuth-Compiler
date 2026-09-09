## ChangeLog

 This file keeps track of all the main changes for every version of the bismuth compiler project.


### Convention

Each entry starts with the version, formatted as `V x.x.x`, followed by the
committer in `<@username>` format and the date of the change.

Below the version header, each entry lists a set of fields in column form,
each written as `field_name:` followed by its value.         

---
 
 ### Bismuth Versions

 ---

 **V 0.8.5** &ensp; <@leonardo-bersellini> &emsp; 07 . 09 . 2026

 `commit:` Stack templates *-o- (baf291b)*
 <br>
 `scope:` Utils, Codegen, Semantics
 <br>
 `features:` refactored stacks inside semanticAnalyzer and Codegen

 ---

 **V 0.8.4** &ensp; <@leonardo-bersellini> &emsp; 06 . 09 . 2026

 `commit:` Removed Strings *-o- (1781c12)*
 <br>
 `scope:` Types, Semantics
 <br>
 `features:` removed strings as a type

 ---
 
 **V 0.8.3**  &ensp; <@leonardo-bersellini> &emsp; 05 . 09 . 2026

 `commit:` Added Changelog *-o- (fb6df77)*
 <br>
 `scope:` All
 <br>
 `features:` implements this changelog to the project

 ---

 **V 0.8.2**  &ensp; <@leonardo-bersellini> &emsp; 05 . 09 . 2026

 `commit:` Tests for Bismuth *-o- (0a0a738)*
 <br>
 `scope:` Tests
 <br>
 `features:` added tests for all bismuth's components.

 ---

 **V 0.8.1**  &ensp; <@leonardo-bersellini> &emsp; 05 . 09 . 2026

 `commit:` Fixed void keyword *-o- (a771412)*
 <br>
 `scope:` Parser, Codegen
 <br>
 `features:` fixed an old bug preventing the void keyword from being used.

 ---

 **V 0.8.0**  &ensp; <@leonardo-bersellini> &emsp; 04 . 09 . 2026

 `commit:` Arrays: indexed access and parameters *-o- (9ba36eb)*
 <br>
 `scope:` Parser, Semantics, Codegen
 <br>
 `features:` finished arrays implementation adding access to a value inside an array.

 ---

 **V 0.7.7**  &ensp; <@leonardo-bersellini> &emsp; 04 . 09 . 2026

 `commit:` Expression-based Assignments *-o- (3283880)*
 <br>
 `scope:` Parser, Semantics, Codegen
 <br>
 `features:` refactored expression. now an expression works on an lvalue and rvalue.

 ---

 **V 0.7.5**  &ensp; <@leonardo-bersellini> &emsp; 02 . 09 . 2026

 `commit:` Refactored type structure *-o- (2b490b2)*
 <br>
 `scope:` Types
 <br>
 `features:` refator of types structure for complex types such as arrays, made implementing a variadic overloaded visitor.

 ---

 **V 0.7.4**  &ensp; <@leonardo-bersellini> &emsp; 01 . 09 . 2026

 `commit:` Arrays and new Primitive Types *-o- (a943c2c)*
 <br>
 `scope:` All
 <br>
 `features:` added first part of static-arrays implementation

 ---

 **V 0.7.2**  &ensp; <@leonardo-bersellini> &emsp; 31 . 08 . 2026

 `commit:` Fixed CliParser bug *-o- (fc596e8)*
 <br>
 `scope:` CLI
 <br>
 `features:` fixed bug inside the cli parser affecting the execution

 ---

 **V 0.7.1**  &ensp; <@leonardo-bersellini> &emsp; 31 . 08 . 2026

 `commit:` Implemented short utility flags *-o- (bba17b1)*
 <br>
 `scope:` Compiler Driver
 <br>
 `features:` added short flags as identifier for utility flags, updated cliparser logic

 ---

 **V 0.7.0**  &ensp; <@leonardo-bersellini> &emsp; 30 . 08 . 2026

 `commit:` Error Recovery *-o- (1ef402d)*
 <br>
 `scope:` Parser
 <br>
 `features:` recovery strategies for errors collected during parser execution

 ---
 
 **V 0.6.0**  &ensp; <@leonardo-bersellini> &emsp; 29 . 08 . 2026

 `commit:` Const keyword *-o- (2504a39)*
 <br>
 `scope:` All
 <br>
 `features:` const variables and declarations

 ---
 
 **V 0.5.0**  &ensp; <@leonardo-bersellini> &emsp; 28 . 08 . 2026

 `commit:` Switch Instruction *-o- (3de8bca)*
 <br>
 `scope:` All
 <br>
 `features:` added a new instrcution for bismuth language: switch

 ---
 
 **V 0.4.3**  &ensp; <@leonardo-bersellini> &emsp; 26 . 08 . 2026

 `commit:` Added new option: no generation *-o- (6ba99e0)*
 <br>
 `scope:` CLI
 <br>
 `features:` new flag: --no-generation

 ---
 
 **V 0.4.2**  &ensp; <@leonardo-bersellini> &emsp; 26 . 08 . 2026

 `commit:` Semantics: organized analyzer structure *-o- (0475a32)*
 <br>
 `scope:` Semantics
 <br>
 `features:` reworked function code structure for clarity

 ---
 
 **V 0.4.1**  &ensp; <@leonardo-bersellini> &emsp; 25 . 08 . 2026

 `commit:` Fixed codegen bugs *-o- (3f12f19)*
 <br>
 `scope:` Codegen
 <br>
 `features:` minor bugs fixed

 ---
 
 **V 0.4.0**  &ensp; <@leonardo-bersellini> &emsp; 25 . 08 . 2026

 `commit:` Codegen: scoped symbol table *-o- (8823719)*
 <br>
 `scope:` Codegen
 <br>
 `features:` added a symbol table for correct generation of shadowed variables

 ---

 **V 0.3.1**  &ensp; <@leonardo-bersellini> &emsp; 25 . 08 . 2026

 `commit:` Implemented top-level statements control *-o- (f685aaa)*
 <br>
 `scope:` Semantics
 <br>
 `features:` added top-level rules 

 ---

 **V 0.3.0**  &ensp; <@leonardo-bersellini> &emsp; 24 . 08 . 2026

 `commit:` Ansi console and messages *-o- (6d0d178)*
 <br>
 `scope:` Utils
 <br>
 `features:` compilation messages written using ansi colors

 ---

 **V 0.2.4**  &ensp; <@leonardo-bersellini> &emsp; 21 . 08 . 2026

 `commit:` Fixed codegen bug: linker include path *-o- (e60013a)*
 <br>
 `scope:` Codegen
 <br>
 `features:` updated arguments passed to lld-link execution 
 
 ---

 **V 0.2.3**  &ensp; <@leonardo-bersellini> &emsp; 20 . 08 . 2026

 `commit:` Fixed Lexer bug: ignored newline code *-o- (4828172)*
 <br>
 `scope:` Lexer
 <br>
 `features:` fixed lexer bug, the lexer was ingoring the \n sequence 
 
 ---

 **V 0.2.2**  &ensp; <@leonardo-bersellini> &emsp; 20 . 08 . 2026

 `commit:` Bug Fixes *-o- (52ae3d4)*
 <br>
 `scope:` All
 <br>
 `features:` fixed minor bugs
 
 ---

 **V 0.2.0**  &ensp; <@leonardo-bersellini> &emsp; 20 . 08 . 2026

 `commit:` refactor: migrate from Qt to standard C++ *-o- (84dff47)*
 <br>
 `scope:` All
 <br>
 `features:` all the code is written using std c++
 
 ---

 **V 0.1.0**  &ensp; <@leonardo-bersellini> &emsp; 20 . 07 . 2026
 
 `commit:` Linker *-o- (09a75da)*
 <br>
 `scope:` Codegen
 <br>
 `features:` implemented a linker for executable files generation