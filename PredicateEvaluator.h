  
#ifndef EVALUATE_H
#define EVAUALTE_H

#include "SparseMatrix.h"

/**
  The method applies a general predicate over a general sparse matrix
  and prints the number of values which satisfy it.
  @param m sparse matrix on which it applies the predicate
  @param predicate
*/

template <typename M, typename P>
int evaluate(const sparse_matrix<M> &m, const P predicato) {

	typename sparse_matrix<M>::iterator i, ie;
	i = m.begin();
	ie = m.end();
	int counter = 0;

	for (; i != ie; i++)
		if (predicato(i->key))
			counter++;

	int rows = m.get_rows();
	int columns = m.get_columns();
	int size = m.get_size();
	int defaults = (rows * columns) - size;

	if (predicato(m.get_defaultValue()))
		counter = counter + defaults;

	return counter;
}

#endif // !EVALUATE_H
