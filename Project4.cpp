/*
 *  Project4.cpp - code for ECE571 project 4 - Color Image Compression Using Unsupervised Learning (Clustering)
 *
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>
#include "Matrix.h"
#include <cstring> 
#include "Pr.h"

using namespace std;

#define Usage "Usage: ./Project4 [option] [int] [Filename]\n"

// define variables which will be globally used
void kMeansCluster(Matrix& sampleMtx, int k);
void winnerCluster(Matrix& sampleMtx, int k);
void SOMCluster(Matrix& sampleMtx, int k);
void calculateMean(Matrix& sampleMtx, vector<vector<int> >& clusterResult, Matrix* clusterMean);
void constructSOMGrid(int k, int &row, int &col);
double topologicalWeight(int row, int col, int winnerRow, int winnerCol);
void initialize(Matrix& sampleMtx, int count, int* indexes);
void generateNewImg(Matrix& sampleMtx, Matrix* clusterCenter, int* pointCluster);

int k;
char* fileName;
double learningRate;

int main(int argc, char **argv) {
    // check to see if the number of argument is correct
    /*if (argc != 7) {
        cout << Usage;
        exit(1);
    }*/
    k = atoi(argv[2]);//Cluster Number 
    fileName = argv[3];
    int row = 480;
    int col = 480;
    Matrix image = readImage("./flowers.ppm", &row, &col);
//    Matrix image32 = readImage("./som320001.ppm", &row, &col);
//    Matrix image64 = readImage("./som640001.ppm", &row, &col);
//    Matrix image128 = readImage("./som1280001.ppm", &row, &col);
//    Matrix image256 = readImage("./som2560001.ppm", &row, &col);
//    cout << calculateRMSD(image, image32) << endl;
//    cout << calculateRMSD(image, image64) << endl;
//    cout << calculateRMSD(image, image128) << endl;
//    cout << calculateRMSD(image, image256) << endl;
//    cout << (strcmp(argv[1], "1"));
    
    if(strcmp(argv[1], "1") == 0) {
        cout << "K-Mean \nFileName:" << fileName << "\nStart: " << k << endl;
        kMeansCluster(image, k);
    }
    if(strcmp(argv[1], "2") == 0) {        
		cout << "Winner-Take-All \nFilename: " << fileName << "\nStart:" << k << " fileName:" << fileName << endl;
        winnerCluster(image, k);
    }
    if(strcmp(argv[1], "3") == 0) {         
		cout << "SOM \nFilename: "<< fileName << "\nStart: "<< k <<  endl;
        SOMCluster(image, k);
    }
}

/**
 * cluster the sample matrix
 */
void kMeansCluster(Matrix& sampleMtx, int k) {
    int sampleRow = sampleMtx.getRow();
    int sampleCol = sampleMtx.getCol();
    int clusterRow = k;
    Matrix clusterMean[clusterRow];
    vector<vector<int> > clusterResult;
    int sampleCluster[sampleRow];
    int indexes[clusterRow];
    //initialize(sampleMtx, clusterRow, indexes);
    // initialize the clusters with random samples
    for(int i = 0; i < clusterRow; i++) {
        clusterResult.push_back(vector<int>());
        for(int j = i; j < sampleRow; j += sampleRow / clusterRow) {
	    clusterResult[i].push_back(j);
        }
    }

    // traverse the samples to assign samples to clusters until no changes with next iteration
    bool changed = true;
    int iteration = 0;
    while(changed) {
        changed = false;
	cout << ++iteration << endl;
        // calculate the new mean 
        calculateMean(sampleMtx, clusterResult, clusterMean);
        for(int i = 0; i < clusterRow; i++) {
            clusterResult[i].clear();
        }
        for(int i = 0; i < sampleRow; i++) {
            double minDist = numeric_limits<double>::max();
            int minIndex;
            Matrix sample = subMatrix(sampleMtx, i, 0, i, sampleCol - 1);
            for(int j = 0; j < clusterRow; j++) {
                double dist = euc(sample, clusterMean[j]);
                if(dist < minDist) {
                    minDist = dist;
                    minIndex = j;
                }
            }
            // push sample to its corresponding class
            clusterResult[minIndex].push_back(i);
            if(sampleCluster[i] != minIndex) {
                changed = true; 
                sampleCluster[i] = minIndex;
            }
        }
    }
    for(int i = 0; i < k; i++) {
	//cout << "Begin to output cluster mean" << endl;
        cout << clusterMean[i] << endl;
    }
    generateNewImg(sampleMtx, clusterMean, sampleCluster);
}


/**
* cluster samples in winner-take-all approach
*/
void winnerCluster(Matrix& sampleMtx, int k) {
	int sampleRow = sampleMtx.getRow();
    int sampleCol = sampleMtx.getCol();
    int clusterRow = k;
    Matrix clusterCenter[clusterRow];
	vector<vector<int> > clusterResult;
    int sampleCluster[sampleRow];
    int indexes[clusterRow];
    initialize(sampleMtx, clusterRow, indexes);
    // initialize the clusters with random samples
    for(int i = 0; i < clusterRow; i++) {
        clusterCenter[i] = Matrix(1, sampleCol);
        for(int j = 0; j < sampleCol; j++) {
           // clusterCenter[i].setValue(0, j, sampleMtx(indexes[i], j));
           clusterCenter[i](0,j) = sampleMtx(indexes[i], j);
        }
    }
    // traverse the samples to assign samples to clusters until no changes with next iteration
    bool changed = true;
    while(changed) {
        changed = false;
        for(int i = 0; i < sampleRow; i++) {
            double minDist = numeric_limits<double>::max();
            int minIndex;
            Matrix sample = subMatrix(sampleMtx, i, 0, i, sampleCol - 1);
            for(int j = 0; j < clusterRow; j++) {
                double dist = euc(sample, clusterCenter[j]);
                if(dist < minDist) {
                    minDist = dist;
                    minIndex = j;
                }
            }
            // update cluster center
            clusterCenter[minIndex] = clusterCenter[minIndex] + 0.01 * (sample - clusterCenter[minIndex]);
            if(sampleCluster[i] != minIndex) {
                changed = true; 
                sampleCluster[i] = minIndex;
            }
        }
	cout << "ischanged: " << changed << "---" << clusterResult[0].size() << "--" 			<< clusterCenter[0] << endl;
    }
    for(int i = 0; i < k; i++) {
	cout << clusterCenter[i] << endl;
    }
    generateNewImg(sampleMtx, clusterCenter, sampleCluster);	
}

void SOMCluster(Matrix& sampleMtx, int k) {
    int sampleRow = sampleMtx.getRow();
    int sampleCol = sampleMtx.getCol();
    int clusterCount = k;
    int gridRow;
    int gridCol;
    constructSOMGrid(k, gridRow, gridCol);
    Matrix clusterCenter[gridRow][gridCol]; 
    int sampleCluster[sampleRow];
    int indexes[clusterCount];
    initialize(sampleMtx, clusterCount, indexes);
    // initialize the clusters with random samples
    for(int i = 0; i < gridRow; i++) {
        for(int j = 0; j < gridCol; j++) {
            clusterCenter[i][j] = Matrix(1, sampleCol);
            for(int k = 0; k < sampleCol; k++) {
//                clusterCenter[i][j].setValue(0, k, sampleMtx(indexes[i * gridCol + j], k));
                 clusterCenter[i][j](0,k) =  sampleMtx(indexes[i * gridCol + j], k);   
            }
        }
    }
    // traverse the samples to assign samples to clusters until no changes with next iteration
    bool changed = true;
    while(changed) {
        changed = false;
        for(int i = 0; i < sampleRow; i++) {
            double minDist = numeric_limits<double>::max();
            int minRow;
            int minCol;
            Matrix sample = subMatrix(sampleMtx, i, 0, i, sampleCol - 1);
            for(int j = 0; j < gridRow; j++) {
                for(int k = 0; k < gridCol; k++) {
                    double dist = euc(sample, clusterCenter[j][k]);
                    if(dist < minDist) {
                        minDist = dist;
                        minRow = j;
                        minCol = k;
                    }
                }
            }
            // update all the cluster center taking topology into account
            for(int j = 0; j < gridRow; j++) {
                for(int k = 0; k < gridCol; k++) {
                    clusterCenter[j][k] = clusterCenter[j][k] + 0.01 * topologicalWeight(j, k, minRow, minCol) * (sample - clusterCenter[j][k]);                    
                }
            }
            // push sample to its corresponding class
            int minIndex = minRow * gridCol + minCol;
            if(sampleCluster[i] != minIndex) {
                changed = true; 
                sampleCluster[i] = minIndex;
            }
        }
	// round cluster center
	for(int j = 0; j < gridRow; j++) {
		for(int k = 0; k < gridCol; k++) {
			for(int l = 0; l < sampleCol; l++) {
				//clusterCenter[j][k].setValue(0, l, round(clusterCenter[j][k](0, l)));
			}
		}
	}
	cout << "ischanged: " << changed << "--" << clusterCenter[0][0] << endl;
   }
   for(int i = 0; i < gridRow; i++) {
        for(int j = 0; j < gridCol; j++) {
                cout << clusterCenter[i][j] << endl;
        }
   }
   // output the result image
   Matrix clusterCenterArray[clusterCount];
   for(int i = 0; i < gridRow; i++) {
        for(int j = 0; j < gridCol; j++) {
                clusterCenterArray[i * gridCol + j] = clusterCenter[i][j];
        }
   }
   generateNewImg(sampleMtx, clusterCenterArray, sampleCluster);
}

/**
* calculate the new cluster mean
*/
void calculateMean(Matrix& sampleMtx, vector<vector<int> >& clusterResult, Matrix* clusterMean) {
    int sampleCol = sampleMtx.getCol();
    for(int i = 0; i < clusterResult.size(); i++) {
        vector<int> clusterIVec = clusterResult[i];
        int clusterISize = clusterIVec.size();
        // there is no sample in the current cluster
        if(clusterISize == 0) {
            continue;
        }
        Matrix imatrix(clusterISize, sampleCol);
        for(int j = 0; j < clusterISize; j++) {
            int sampleIndex = clusterIVec[j];
            for(int k = 0; k < sampleCol; k++) {
             //   imatrix.setValue(j, k, sampleMtx(sampleIndex, k));
                imatrix(j,k) = sampleMtx(sampleIndex, k);
            }
        }
        clusterMean[i] = transpose(mean(imatrix, sampleCol));
    }
}

/**
 * construct a SOM which is as square as possible 
 */
void constructSOMGrid(int k, int &row, int &col) {
    int temp = sqrt(k);
    while(k % temp != 0) {
        temp--;
    }
    int temp2 = k / temp;
    row = max(temp, temp2);
    col = min(temp, temp2);
}

/**
 * calculate the topolocial weight between one cluster center and the winner cluster center
 */
double topologicalWeight(int row, int col, int winnerRow, int winnerCol) {
    Matrix cluster(1, 2);
    //cluster.setValue(0, 0, row);
    //cluster.setValue(0, 1, col);
    cluster(0,0) = row;
    cluster(0,1) = col; 
    Matrix winner(1, 2);
    //winner.setValue(0, 0, winnerRow);
    //winner.setValue(0, 1, winnerCol);
    winner(0,0) = winnerRow;
    winner(0,1) = winnerCol;
    return exp(-0.5 * euc(cluster, winner));
}

void initialize(Matrix& sampleMtx, int count, int* indexes) {
    int sampleRow = sampleMtx.getRow();
    for(int i = 0; i < count; i++) {
        int newIndex;
        while(true) {
            bool duplicate = false;
            newIndex = rand() % sampleRow;
            for(int j = 0; j < i; j++) {
                if(newIndex == indexes[j]) {
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate) {
                break;
            }
        }
        indexes[i] = newIndex;
    }
}

void generateNewImg(Matrix& sampleMtx, Matrix* clusterCenter, int* pointCluster) {
    int sampleRow = sampleMtx.getRow();
    int sampleCol = sampleMtx.getCol();
    Matrix result(sampleRow, sampleCol);
    for(int i = 0; i < sampleRow; i++) {
        Matrix transformed = clusterCenter[pointCluster[i]];
        for(int j = 0; j < sampleCol; j++) {
           // result.setValue(i, j, transformed(0, j));
            result(i,j) = transformed(0, j);
        }
    }
    cout << result.getRow() << "----" << sampleRow << "----" << sampleCol << endl;
    writeImage("./output.ppm", result, sqrt(sampleRow), sqrt(sampleRow));
}

