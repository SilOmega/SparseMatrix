#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H


#include <algorithm> // std::swap
#include <iostream>
#include <iterator> // std::forward_iterator_tag

/** Definition of a templated sparse_matrix class
  which gives a simulation of a standard matrix
  where only explicity inserted elements are 
  effectively memorized. In this specific case 
  the matrix has unlimited size.
*/


template <typename T>
class sparse_matrix {

public: 
	///< Definition of usable index within the class
	typedef unsigned int positive; // only positive values 

private:

	struct node {
		positive row;
		positive column;
		T key;
		node *next;
		node *down; // pointer to next row

		node() : row(0), column(0), next(0), down(0) {}

		node(const T &k, int r, int c) : row(r), column(c), key(k), next(0), down(0) {}

	};

	

	node *head; ///< pointer to first element of the matrix
	positive size; ///< number of element in the matrix
	positive rows; ///< rows number of the matrix
	positive columns; ///< columns number of the matrix
	T default_value;

	void clear() {

		node *r = head;
		node *n = head;

		while (r != 0) {
			node *tmp2 = r->down;
			while (n != 0) {
				node *tmp = n->next;
				delete n;
				n = 0;
				n = tmp;
			}
			n = tmp2;
			r = tmp2;
		}


		size = 0;
		rows = 0;
		columns = 0;

	}

public:

	///< Default constructor
	sparse_matrix() : head(0), default_value(0), size(0), columns(0), rows(0) {}

	/**	
		Secondary constructor to create a sparse_matrix with a default value 
		@param def_value default value to assign	
	*/
	explicit sparse_matrix(const T &def_value) : head(0), default_value(def_value), size(0), columns(0), rows(0) {}

	/// Destructor
	~sparse_matrix() {
		
		clear();
		std::cout << "~sparse_matrix called" << std::endl;
	}

		
	/**
		Copy constructor
		@param other matrix to copy
	*/
	sparse_matrix(const sparse_matrix &other) : head(0), default_value(0), size(0), columns(0), rows(0) {
		const node *tmp = other.head;
		const node *rows = other.head;

		try {
			while (rows != 0) {
				while (tmp != 0) {
					add(tmp->row, tmp->column, tmp->key);
					tmp = tmp->next;
				}
				rows = rows->down;
				tmp = rows;
			}
			set_default(other.get_defaultValue());

		}catch (...) {
			clear();
			throw;
		}
	}
	
	/**
		Copy constructor which casts from a type Q to T 
		@param other matrix to copy
	*/
	template <typename Q>
	sparse_matrix(const sparse_matrix<Q> &other) : head(0), default_value(0), size(0), columns(0), rows(0) {
		
		typename sparse_matrix<Q>::iterator i, ie;
		i = other.begin();
		ie = other.end();
		
		try {
			for (; i != ie; i++) {
				add(i->row, i->column, (T) i->key);
			}

			set_default( (T) other.get_defaultValue());
		}
		catch (...) {
			clear();
			throw;
		}
		
	
	}


	/**
		Assignment operator
		@param other matrix to copy
		@return modified matrix
	*/
	sparse_matrix&operator=(const sparse_matrix &other)  {

		if (this != &other) {
			sparse_matrix tmp(other);
			std::swap(size, tmp.size);
			std::swap(head, tmp.head);

		}

		return *this;

	}


	/// Size getter
	positive get_size() const {
		return size;
	}

	/// Columns getter
	positive get_columns() const {
		return columns;
	}

	/// Rows getter
	positive get_rows() const {
		return rows;
	}

	/// Default_value getter
	T get_defaultValue() const{
		return default_value;
	}


	/**
		Method to add a value at given coordinates
		@param value value to add
		@param i row index 
		@param j column index
	*/
	void add(positive i, positive j, const T &value) {
		node *current;

		current = new node(value, i ,j);
		
		// update matrix dimensions
		if (i > rows)
			rows = i;
		if (j > columns)
			columns = j;

		size++;

		if (head == 0) {
			head = current;
			return;
		}

		node *n = head, *prev = head;

		// Scans rows
		while (n != 0 && i > n->row) {
			prev = n;
			n = n->down;
		}

		if (n == 0) // Last row addition case
			prev->down = current;
		else if (n == head) { //  Addition case if next element is the head 
			if (n->row > i) {
				current->down = head;
				head = current;
			} else {
				if (n->column > j) {
					current->next = head;
					head = current;
				}
				else {
					while (n != 0 && j > n->column) {
						prev = n;
						n = n->next;
					}
					current->next = prev->next;
					prev->next = current;
				}
			}
		}
		else { // Other insertion cases
			if (n->row > i) {
				current->down = prev->down;
				prev->down = current;
			}
			else {
				while (n != 0 && j > n->column) {
					prev = n;
					n = n->next;
				}

				
				if (prev->row < i) {
					current->next = prev->down;
					if (n->down != 0) {
						current->down = n->down;
						n->down = 0;
					}
					prev->down = current;
					
				} else {
					current->next = prev->next;
					prev->next = current;
				}
			}
		}

	}


	/**
    Retrieve operator of an element at given coordinates
		@param i row index 
		@param j column index 
		@return value at specified position
	*/
	T &operator()(unsigned int i, unsigned int j) {
		node *n = head;

		while (n != 0 && n->row < i) {
			n = n->down;
		}

		while (n != 0 && n->column < j) {
			n = n->next;
		}

		
		if (n != 0 && n->row == i && n->column == j)
			return n->key;
		else
			return default_value;
	}

	/**
		Method to set default value
		@param value new default value
	*/
	void set_default(const T &value) {
		default_value = value;
	}


	class iterator {
		node *r; // node of current row
		node *c; // node of current column
		
		// Struct of element returned by iterator
		struct element {
			const positive row;
			const positive column;
			const T key;

			element(const int r, const int c, const T &k) : row(r), column(c), key(k) {}
	
			~element() {
				row = 0;
				column = 0;
				key = 0;
			}
			
		};

			

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element*                        pointer;
		typedef element&                        reference;


		iterator() : r(0), c(0) {}

		iterator(const iterator &other) : r(other.r), c(other.c) {}

		iterator& operator=(const iterator &other) {
			c = other.c;
			r = other.r;
			return *this;
		}

		~iterator() {}

		// Return datum referred by iterator (Dereference)
		reference operator*() {
			element e = element(c->row, c->column, c->key);
			return e;
		}

		// Return pointer to datum referred by iterator 
		pointer operator->() {
			element *e = new element(c->row, c->column, c->key);
			return e;
		}

		// Post-increment iteration operator 
		iterator operator++(int) {
			iterator tmp(*this);
			if (c->next != 0)
				c = c->next;
			else {
				r = r->down;
				c = r;
			}
			return tmp;
		}

		// Pre-increment iteration operator
		iterator& operator++() {
			if (c->next != 0)
				c = c->next;
			else {
				r = r->down;
				c = r;
			}
			return *this;
		}

		// Equality
		bool operator==(const iterator &other) const {
			return (r == other.r && c == other.c);
		}

		// Diversity
		bool operator!=(const iterator &other) const {
			return (r != other.r || c != other.c);
		}

	private:

		friend class sparse_matrix;

		iterator(node *rr) : r(rr), c(rr) {}


	}; 


	iterator begin() const {
		return iterator(head);
	}

	iterator end() const {
		return iterator(0);
	}

	

};


template <typename T>
std::ostream &operator<<(std::ostream &os, const sparse_matrix<T> &sp) {
	typename sparse_matrix<T>::iterator i, ie;
	i = sp.begin();
	ie = sp.end();

	for (unsigned int a = 0; a <= sp.get_rows(); a++) {
		for (unsigned int b = 0; b <= sp.get_columns(); b++) {
			if (i != ie && i->row == a && b == i->column) {
				os << "[" << i->key << "]	";
				i++;
			}
			else
				os << "[" << sp.get_defaultValue() << "]	";
		}
		os << std::endl;
	}
	return os;
}


#endif
