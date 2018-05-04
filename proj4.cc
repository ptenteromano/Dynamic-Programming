/*********************************************************************
 *
 * CISC 4080: Computer Algorithms 
 * Project 4: Dynamic Programming 
 * Spring 2018
 *
 * @author Philip Tenteromano
 * @date March 16th, 2018
 * @file proj4.cc
 *
 * Finding Optimal Matrix Multiplication
 *
 **********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
using namespace std;

#include "Matrix.h"
#include "MatrixIO.h"
using namespace Numeric_lib;

/*
 * Function declarations
 */ 

/**
 * Create a vector of ints, whose content comes from an istream
 *
 * @param source the input stream
 * @return the vector
 */
vector<int> fill(istream& source);

/**
 * Input: vector of matrices to be multiplied
 * Output: optimal number of mulitplications  
 */
int opt_mult(const vector<int>& m);

/**
 * Compares and stores the minimum multiplications, used in opt_mult
 */
int subchain_min(unsigned int i, unsigned int j, const vector<int>& m, 
                 Matrix<int,2> c);

/**
 * The usual main() function
 */
int main(int argc, char** argv)
{
   string progname{argv[0]};           // for msgs

   // command-line arg processing
   if (argc > 2) {
      cerr << "Usage: " << progname << " [mfile]\n";
      return 1;
   }

   // get the data
   vector<int> m;
   if (argc == 1) 
      m = fill(cin);
   else {
      ifstream source(argv[1]);
      if (!source) {
         cerr << progname << ": can't open " << argv[1] << endl;
         return 2;
      }
      m = fill(source);
   }
    
   // solve the problem and report results
   int mults = opt_mult(m);
   cout << mults << " multiplications\n";
};

/*
 * Function definitions
 */

vector<int> fill(istream& source)
{
   vector<int> m;
   int datum;
   while (source >> datum)
      m.push_back(datum);
   return m;
}

// finding minimum multiplications
int opt_mult(const vector<int>& m)
{	
	unsigned int n = m.size();
	unsigned int j;	
	// matrix holding costs of subproblems	
	Matrix<int,2> costTable(n,n);

	// Ai x Ai = 0 multiplications
	for(unsigned int i = 0; i < n; i++)	
		costTable(i,i) = 0;
	
	// for every subproblem, store multiplications in matrix
	for(unsigned int s=2; s < n; s++) 
		for(unsigned int i = 0; i < n-s; i++) {
			j = i + s-1;
			// using a dummy value to find min
			costTable(i,j) = INT_MAX;
			costTable(i,j) = subchain_min(i,j,m,costTable);
		}
	// return the final value in the table
	return costTable(0,n-2); 
}

// auxiliary function to find min value through k split
int subchain_min(unsigned int i, unsigned int j, const vector<int>& m, 
                 Matrix<int,2> c)
{
	// find min through possible k values: i <= k < j
	for(unsigned int k = i; k < j; k++) {
		int currCost = c(i,k) + c(k+1,j) + m[i] * m[k+1] * m[j+1];
		if ( currCost < c(i,j) )	 
			c(i,j) = currCost;		// store min of all possiblities 	
	}

   	return c(i,j);
}



