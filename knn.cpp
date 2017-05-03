/***************************************************************************
 ** Routine Name:    knn
 **
 ** Purpose: This routine provides the function performing kNN classifier.
 **    For each testing sample, kNN looks for its k nearest neighbors, and
 **    assigns the testing sample to the class which has the maximum number
 **    of training samples within that k neighbors.
 **
 ** Prototype: int knn(Matrix train, Matrix label, Matrix test, int c, int k)
 **    - train: The training set of m x (n+1) matrix
 **               where m is the number of rows (or samples)
 **               n is the number of features, the last column is the label
 **               which starts at 0
 **    - test:  The testing sample to be classified
 **               whose dimension is 1xn
 **    - nf:    The number of features
 **    - c:     The number of classes
 **    - k:     The number of nearest neighbors
 **
 ** Created by: Xiaoling Wang (xwang1@utk.edu), UTK
 **
 ** Modified by 
 **    - 02/20/2008: modified the parameter list to be consistent with mpp.cpp
 **    - 05/03/2005: separate the data set with the label matrix  
 **    - 05/02/2005 by Hairong Qi: change from Image library to Matrix library
 **    - 01/25/2004 by Hairong Qi
 ****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Pr.h"

using namespace std;

int knn(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k) 
{
  int    nrtr, nctr, nrte, i, j, tmp;
  Matrix pos, dist, sdist, count, scount, cpos, trsample, trsampleT;
      
  nrtr = train.getRow();
  nctr = train.getCol();
  if (nctr < nf)
    cout << "kNN: "
	 << "number of features is too large\n";

  // Calculate the dist. between the test sample and each training sample
  dist.createMatrix(1,nrtr);    // save the distance value
  sdist.createMatrix(1,nrtr);   // save the sorted distance value
  pos.createMatrix(1,nrtr);     // save the sorted index
  for (i=0; i<nrtr; i++) {         // for each training vector
    dist(0,i) = euc(transpose(subMatrix(train,i,0,i,nf-1)), tedata);
  }

  // Sort array dist using insertion sort in the ascending order
  insertsort(dist, sdist, pos);

  // Count the nearest k neighbors of different classes
  count.createMatrix(1,c);
  for (j=0; j<k; j++) {
    tmp = (int)train((int)pos(0,j),nctr-1);
    count(0,tmp)++;
  }
  
  // Pick the majority
  scount.createMatrix(1,c);
  cpos.createMatrix(1,c);
  insertsort(count, scount, cpos);

  return (int)cpos(0,c-1);
}