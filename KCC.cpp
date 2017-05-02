#include <vector>
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <cfloat>
#include <Matrix.h>
#include <Pr.h>

using namespace std;

//KMean function to perform the kMean method
void kMean(Matrix& orgImg,Matrix& modImg,Matrix& means,Matrix& pClass,int k,ofstream& out);
void WTA(Matrix& orgImg,Matrix& modImg,Matrix& means,Matrix& pClass,int k,ofstream& out);

int main(int argv, char** argc){

	ifstream fin;
	ofstream fout;
	ofstream dout;
	
	fin.open("flowers.ppm");
	if(!fin.is_open()){
		cerr << "Error opening file: Matrix .ppm\n";
		exit(1);
	}
	
	Matrix KM256(256,3);
	Matrix WTA256(256,3);
	Matrix KM128(128,3);
	Matrix WTA128(128,3);
	Matrix KM64(64,3);
	Matrix WTA64(64,3);
	Matrix KM32(32,3);
	Matrix WTA32(32,3);
	string magicID = " ";
	char rgbVal = '\0';
	int i = 0,j = 0,k = 0;
	int nrows = 0,ncols = 0,maxColor = 0;
	string creatorInfo = "";
	
	// Read ppm file in
	getline(fin,magicID);
	getline(fin,creatorInfo);
	fin >> nrows >> ncols >> maxColor;
	while(fin.get() != '\n') {/*skip newline if present*/};
	
	// Make image matrices and distance matrices
	Matrix orgFile(nrows*ncols,3);
	Matrix modFile(nrows*ncols,3);
	Matrix pixClass(nrows*ncols,1);
	
	// read in org image
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fin.get(rgbVal);
			orgFile(i,j) = (int)rgbVal;
		}
	}
	fin.close();
	
	// Randomly assign vals to cluster matrices
	srand(time(NULL));
	for(i = 0;i < 256;i++){
		for(j = 0;j < 3;j++){
			KM256(i,j) = rand() % 256;
			WTA256(i,j) = rand() % 256;
			if(i < 128){
				KM128(i,j) = rand() % 256;
				WTA128(i,j) = rand() % 256;
			}
			if(i < 64){
				KM64(i,j) = rand() % 256;
				WTA64(i,j) = rand() % 256;
			}
			if(i < 32){
				KM32(i,j) = rand() % 256;
				WTA32(i,j) = rand() % 256;
			}
		}
	}
	
	dout.open("RunsData.csv");
	if(!dout.is_open()){
		cerr << "Error opening write file with extension .csv, program terminated.\n";
		exit(0);
	}
	
	kMean(orgFile,modFile,KM256,pixClass,256,dout);
	fout.open("flowers256KMean.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "256 KMeans Done\n";
	dout << "225;KMeans Done\n";
	
	
	kMean(orgFile,modFile,KM128,pixClass,128,dout);
	fout.open("flowers128KMean.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "128 KMeans Done\n";
	dout << "128;KMeans Done\n";
	
	
	kMean(orgFile,modFile,KM64,pixClass,64,dout);
	fout.open("flowers64KMean.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "64 KMeans Done\n";
	dout << "64;KMeans Done\n";
	
	
	kMean(orgFile,modFile,KM32,pixClass,32,dout);
		fout.open("flowers32KMean.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "32 KMeans Done\n";
	dout << "32;KMeans Done\n";
	
	WTA(orgFile,modFile,KM256,pixClass,256,dout);
	fout.open("flowers256WTA.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "256 WTA Done\n";
	dout << "256;WTA Done\n";
	
	
	
	WTA(orgFile,modFile,KM128,pixClass,128,dout);
	fout.open("flowers128WTA.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "128 WTA Done\n";
	dout << "128;WTA Done\n";
	
	
	WTA(orgFile,modFile,KM64,pixClass,64,dout);
	fout.open("flowers64WTA.ppm", ofstream::binary);										// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "64 WTA Done\n";
	dout << "64;WTA Done\n";
	
	
	WTA(orgFile,modFile,KM32,pixClass,32,dout);
		fout.open("flowers32WTA.ppm", ofstream::binary);								// Make sure file is open and ready
	if(!fout.is_open()){															// for binary output.
		cerr << "Error opening write file with extension .ppm, program terminated." << endl;
		exit(0);
	}else{
		fout << magicID << endl << creatorInfo << endl << ncols << " " << nrows << endl << maxColor << endl;	// Output header, mind the correct formatting.
	}
	for(i = 0;i < (nrows * ncols);i++){
		for(j = 0;j < 3;j++){
			fout << (unsigned char)modFile(i,j);
		}
	}
	fout.close();
	cout << "32 WTA Done\n";
	dout << "32;WTA Done\n";

}//END OF MAIN

void kMean(Matrix& orgImg,Matrix& modImg,Matrix& means,Matrix& pClass,int k,ofstream& out){
	bool done = false;
	int i = 0,j = 0,m = 0;
	int numPix = orgImg.getRow();
	int minIdx = 0;
	int rMean = 0,gMean = 0,bMean = 0,mTotal = 0;
	int wrongCount = 0,intCount = 0;
	double curDist = 0.0;
	double curMin;
	double sum = 0;
	while(!done){
		done = true;
		wrongCount = 0;
		for(i = 0;i < numPix;i++){
			curMin = LONG_MAX;
			minIdx = pClass(i,0);
			for(j = 0;j < k;j++){
				curDist = euc(subMatrix(orgImg,i,0,i,2),subMatrix(means,j,0,j,2));
				if(curMin != min(curMin,curDist)){
					curMin = min(curMin,curDist);
					minIdx = j;
				}
			}
			if(minIdx != pClass(i,0)){
				wrongCount++;
				done = false;
				pClass(i,0) = minIdx;
			}
		}
		
		// If reclassification has occurred, recalc the means
		if(wrongCount > 0){
			for(i = 0;i < k;i++){
				rMean = 0;gMean = 0;bMean = 0;mTotal = 0;
				for(j = 0;j < numPix;j++){
					if(pClass(j,0) == i){
						rMean += orgImg(j,0);
						gMean += orgImg(j,1);
						bMean += orgImg(j,2);
						mTotal++;
					}
				}
				if(mTotal != 0){
					means(i,0) = (int)(rMean / mTotal);
					means(i,1) = (int)(gMean / mTotal);
					means(i,2) = (int)(bMean / mTotal);
				}
			}
		}
		out << "WCount; " << wrongCount << "\n";
		intCount++;
	}
	out << "hopped out in;" << intCount << "\n";
	 //Once done classifying, create modded image file
	for(i = 0;i < numPix;i++){
		for(j = 0;j < 3;j++){
			modImg(i,j) = means(pClass(i,0),j);
		}
	}
}

void WTA(Matrix& orgImg,Matrix& modImg,Matrix& means,Matrix& pClass,int k,ofstream& out){
	bool done = false;
	int i = 0,j = 0;
	int numPix = orgImg.getRow();
	int minIdx = 0;
	int intCount = 0,wrongCount = 0,prevWrong = numPix;
	int rMean = 0,gMean = 0,bMean = 0,mTotal = 0;
	double curDist = 0.0;
	double curMin;
	double lRate = .5;
	while(!done){
		done = true;
		wrongCount = 0;
		for(i = 0;i < numPix;i++){
			curMin = LONG_MAX;
			minIdx = pClass(i,0);
			for(j = 0;j < k;j++){
				curDist = euc(subMatrix(orgImg,i,0,i,2),subMatrix(means,j,0,j,2));
				if(curMin != min(curMin,curDist)){
					curMin = min(curMin,curDist);
					minIdx = j;
				}
			}
			// modify the mean on the fly based on error distance
			if(minIdx != pClass(i,0)){
				wrongCount++;
				done = false;
				pClass(i,0) = minIdx;
			}
				means(pClass(i,0),0) = (means(pClass(i,0),0) + round((lRate * (orgImg(i,0) - means(pClass(i,0),0)))));
				means(pClass(i,0),1) = (means(pClass(i,0),1) + round((lRate * (orgImg(i,1) - means(pClass(i,0),1)))));
				means(pClass(i,0),2) = (means(pClass(i,0),2) + round((lRate * (orgImg(i,2) - means(pClass(i,0),2)))));
		}
		
		// If reclassification has occurred, recalc the means
		if(!done){
			for(i = 0;i < k;i++){
				rMean = 0;gMean = 0;bMean = 0;mTotal = 0;
				for(j = 0;j < numPix;j++){
					if(pClass(j,0) == i){
						rMean += orgImg(j,0);
						gMean += orgImg(j,1);
						bMean += orgImg(j,2);
						mTotal++;
					}
				}
				if(mTotal != 0){
					means(i,0) = (int)(rMean / mTotal);
					means(i,1) = (int)(gMean / mTotal);
					means(i,2) = (int)(bMean / mTotal);
				}
			}
		}
		intCount++;
		out << "WCount;" << wrongCount << ";lRate;" << lRate << "\n";
		if(wrongCount >= prevWrong){lRate *= 0.5;}
		prevWrong = wrongCount;
	}
	out << "hopped out in;" << intCount << "\n";
	
	// Once done classifying, create modded image file
	for(i = 0;i < numPix;i++){
		for(j = 0;j < 3;j++){
			modImg(i,j) = means(pClass(i,0),j);
		}
	}
}