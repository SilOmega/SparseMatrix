# SparseMatrix

The Sparse Matrix folder contains two headers: Sparse Matrix and Predicate Evaluator. The first is the implementation of the data structure class. The logic used is to simulate a matrix but it keeps in memory only data explicitly inserted by user in order to maintain a low memory usage. It also exposes random access iterators for operations that require them. The second one implements a first order method which applies a predicate to the matrix and returns the number of values that satisfy it.
