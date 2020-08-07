# SparseMatrix

Sparse Matrix is a templated implementation of a sparse matrix data structure. The logic is to simulate a matrix that keeps in memory only data explicitly inserted by user in order to maintain a low memory usage. The class does not uses any STL container to maintain the data and exposes forward iterators for operations that require them. 

The PredicateEvaluator.h contains the implementation of a first order method which applies a predicate to the matrix and returns the number of values that satisfy it.
