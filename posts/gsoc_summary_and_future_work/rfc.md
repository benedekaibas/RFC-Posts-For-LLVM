# RFC

## Background/Motivation

During the summer I was working on teaching the Clang Static Analyzer to understand lifetime annotations. The purpose of this, is to help out the compiler level lifetime analysis
(https://discourse.llvm.org/t/rfc-intra-procedural-lifetime-analysis-in-clang/86291). In this RFC, I will explain the work I have done, the current standings of the checkers, and the future work.

## The purpose of the lifetime checkers

The purpose of implementing these checkers are:
1. Understand lifeitme annotations: [[clang::lifetimebound]] and [[clang::lifetime_capture_by(this)]]
2. Help out the compiler *in cases* where it lacks information

### What are these cases?

The CSA does a path sensitive analysis while the compiler level analysis performs an intraprocedural analysis. A great example for this:

![Picture One](RFC-Posts-For-LLVM/pictures/func-chain-comparison.png)

If any of the functions are not annotated in the function call chain then the intraprocedural analysis fails.
