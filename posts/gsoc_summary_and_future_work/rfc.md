# RFC

## Background

During the summer I worked on teaching the Clang Static Analyzer to understand lifetime annotations. In this RFC, I will explain the work I have
done, the current standings of the checkers, and the future work.

In case you are interested in the final report which is a detailed summary of my work that also contains the related patches, please
visit: https://benedekaibas.github.io/2026-GSoC-LLVM-Final-Report/

## Motivation

Clang has an intra-procedural lifetime analysis behind the `-Wlifetime-safety` flag. This is a powerful compile-time analysis, but has its
limitations. By implementing a lifetime checker family in the Clang Static Analyzer users have multiple ways for detecting lifetime issues
in their codebase. Also together `-Wlifetime-safety` and the lifetime checkers in the CSA cover a wider range of lifetime errors.

## How does the lifetime checker family look like?

The lifetime checkers family consits 3 checkers. There is one modeling and two reporting checkers.

`LifetimeModeling`

The modeling checker is responsible for recording the binding between the objects that are referred to by the annotated parameter and the
annotated parameter itself. It does the recording by subscribing to the CFG lifetime end elements through the `check::LifetimeEnd`.
This callback records when a stack object's lifetime end. This information is relevant for the `UseAfterLifetimeEnd` checker. 

## Understand annotations

In order to run the analysis in the checker has to understand the annotations. Currently only `[[clang::lifetimebound]]` is supported, but
I intend to extend the coverage for `[[clang::lifetime_capture_by]]` as well (read the future works section).

The `[[clang::lifetimebound]]` attribute on a function parameter or the implicit object parameter indicates that objects that are referred
to by that parameter may also be referred to by the return value of the annotated function. Clang attaches the lifetimebound annotation to
the annotated `ParmVarDecl`, so the contract is in the AST. The lifetime checker has to store this contract (binding) somehow. To store
the binding I have created a program state map that holds the binding(s).

```{cpp}
REGISTER_SET_FACTORY_WITH_PROGRAMSTATE(LifetimeSourceSet, const MemRegion *)
REGISTER_MAP_WITH_PROGRAMSTATE(LifetimeBoundMap, SVal, LifetimeSourceSet)
```



### What are these cases?

The CSA does a path sensitive analysis while the compiler level analysis performs an intraprocedural analysis. A great example for this:

![Picture One](/pictures/func-chain-comparison.png)

If any of the functions are not annotated in the function call chain then the intraprocedural analysis fails.
