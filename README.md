# NFA to DFA Converter and DFA Minimizer

This repository contains two C++ programs: one for converting a Non-deterministic Finite Automaton (NFA) to a Deterministic Finite Automaton (DFA), and another for minimizing the resulting DFA.

## Files

- `NFAtoDFA.cpp`: Contains the implementation for converting an NFA to a DFA.
- `DFAMinimizer.cpp`: Contains the implementation for minimizing the DFA.

## NFA to DFA Conversion

This program uses subset construction to convert an NFA to a DFA. The key functions include:
- `epsilonClosure()`: Computes the epsilon closure of a set of states.
- `convertNFAtoDFA()`: Converts the given NFA to a DFA using the subset construction method.
- `printNFA()`: Prints the transitions of the NFA.

## DFA Minimizer

This program minimizes a given DFA using the partition refinement technique. The key functions include:
- `addTransition()`: Adds a transition to the DFA.
- `minimize()`: Minimizes the DFA using partition refinement.
- `print()`: Prints the DFA's transitions and states.

## How to Run

1. **Compile the programs**:
   ```bash
   g++ NFAtoDFA.cpp -o NFAtoDFA
   g++ DFAMinimizer.cpp -o DFAMinimizer
