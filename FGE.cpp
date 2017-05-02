#include <vector>
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cfloat>
#include <Matrix.h>
#include <Pr.h>

using namespace std;

float sigmoid(float x);
float sigmund(float x);

// Modified versions of those Dr. Qi gave us in her Matrix Lib. One for Euclidean, one for
// Mahalanobis
int knn2(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k);
int knn1(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k);
int knn3(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k);
int knn4(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k);

int main(int argc,char** argv){

	Matrix rawData(214,10);
	Matrix Te1(15,9);Matrix Tr1((214-15),10);Matrix nTe1(15,9);Matrix nTr1((214-15),10);Matrix gT1(15,1);Matrix Re1(15,1);
	Matrix Te2(21,9);Matrix Tr2((214-21),10);Matrix nTe2(21,9);Matrix nTr2((214-21),10);Matrix gT2(21,1);Matrix Re2(21,1);
	Matrix Te3(28,9);Matrix	Tr3((214-28),10);Matrix nTe3(28,9);Matrix nTr3((214-28),10);Matrix gT3(28,1);Matrix Re3(28,1);
	Matrix Te4(17,9);Matrix Tr4((214-17),10);Matrix nTe4(17,9);Matrix nTr4((214-17),10);Matrix gT4(17,1);Matrix Re4(17,1);
	Matrix Te5(17,9);Matrix Tr5((214-17),10);Matrix nTe5(17,9);Matrix nTr5((214-17),10);Matrix gT5(17,1);Matrix Re5(17,1);
	Matrix Te6(26,9);Matrix Tr6((214-26),10);Matrix nTe6(26,9);Matrix nTr6((214-26),10);Matrix gT6(26,1);Matrix Re6(26,1);
	Matrix Te7(22,9);Matrix Tr7((214-22),10);Matrix nTe7(22,9);Matrix nTr7((214-22),10);Matrix gT7(22,1);Matrix Re7(22,1);
	Matrix Te8(24,9);Matrix Tr8((214-24),10);Matrix nTe8(24,9);Matrix nTr8((214-24),10);Matrix gT8(24,1);Matrix Re8(24,1);
	Matrix Te9(26,9);Matrix Tr9((214-26),10);Matrix nTe9(26,9);Matrix nTr9((214-26),10);Matrix gT9(26,1);Matrix Re9(26,1);
	Matrix Te10(18,9);Matrix Tr10((214-18),10);Matrix nTe10(18,9);Matrix nTr10((214-18),10);Matrix gT10(18,1);Matrix Re10(18,1);
	Matrix curTest(1,9);
	Matrix gTruth(4,1);
	Matrix weights(3,3);
	Matrix bias (1,3);
	Matrix train(4,2);
	Matrix output(4,1);
	Matrix AvgAcc(500,1);
	Matrix outMove(500000,4);
	
	double hid1 = 0.0,hid2 = 0.0,hid3 = 0.0;
	double delta1 = 0.0,delta2 = 0.0,delta3 = 0.0;
	double thres1 = 0.0,thres2 = 0.0;
	double lRate = 0.1;
	float avgOverall = 0.0;
	float curDatum = 0.0;
	int i = 0,j = 0,k = 0,m = 0;
	int rCount = 0;
	int curIndx = 0;
	int curClass = 0;
	int aIndx = 0,bIndx = 0;
	bool check = false;
	bool wCheck = false;
	string junk = "";
	string foldSegs = argv[1];
	string dataIn = argv[2];
	srand(time(NULL));
	ifstream fin;
	ofstream fout,fout2,fout3;
	double fMean[9];
	double fVariance[9];
	
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	fout.open("KNNData.csv");
	if(!fout.is_open()){
		cerr << "Error opening file: KNNData.csv\n";
		exit(1);
	}
	/* ********************10-Fold Section******************** */
	
	fin.open(dataIn.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << dataIn << "\n";
		exit(1);
	}
	for(i = 0;i < 10;i++){fin >> junk;}
	for(i = 0;i < 214;i++){
		for(j = 0;j < 10;j++){
			fin >> curDatum;
			rawData(i,j) = curDatum;
		}
	}
	fin.close();
	fin.open(foldSegs.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << foldSegs << "\n";
		exit(1);
	}
	fin >> curIndx;
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te1(aIndx,j) = rawData(i,j);
			}
			gT1(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr1(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 15;i++){
		gT1(i,0) = rawData(gT1(i,0),9);
		gT1(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-15);j++){fMean[i] += Tr1(j,i);}
		fMean[i] /= (214-15);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-15);j++){fVariance[i] += ((Tr1(j,i) - fMean[i]) * (Tr1(j,i) - fMean[i]));}
		fVariance[i] /= (213-15);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-15);j++){nTr1(j,i) = ((Tr1(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-15);j++){nTr1(j,i) = (Tr1(j,i) - 1);}
		}
	}
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 15;j++){nTe1(j,i) = ((Te1(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te2(aIndx,j) = rawData(i,j);
			}
			gT2(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr2(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 21;i++){
		gT2(i,0) = rawData(gT2(i,0),9);
		gT2(i,0)--;
	}
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-21);j++){fMean[i] += Tr2(j,i);}
		fMean[i] /= (214-21);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-21);j++){fVariance[i] += ((Tr2(j,i) - fMean[i]) * (Tr2(j,i) - fMean[i]));}
		fVariance[i] /= (213-21);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-21);j++){nTr2(j,i) = ((Tr2(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-21);j++){nTr2(j,i) = (Tr2(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 21;j++){nTe2(j,i) = ((Te1(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te3(aIndx,j) = rawData(i,j);
			}
			gT3(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr3(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 28;i++){
		gT3(i,0) = rawData(gT3(i,0),9);
		gT3(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-28);j++){fMean[i] += Tr3(j,i);}
		fMean[i] /= (214-28);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-28);j++){fVariance[i] += ((Tr3(j,i) - fMean[i]) * (Tr3(j,i) - fMean[i]));}
		fVariance[i] /= (213-28);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-28);j++){nTr3(j,i) = ((Tr3(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-28);j++){nTr3(j,i) = (Tr3(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 28;j++){nTe3(j,i) = ((Te3(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te4(aIndx,j) = rawData(i,j);
			}
			gT4(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr4(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 17;i++){
		gT4(i,0) = rawData(gT4(i,0),9);
		gT4(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-17);j++){fMean[i] += Tr4(j,i);}
		fMean[i] /= (214-17);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-17);j++){fVariance[i] += ((Tr4(j,i) - fMean[i]) * (Tr4(j,i) - fMean[i]));}
		fVariance[i] /= (213-17);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-17);j++){nTr4(j,i) = ((Tr4(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-17);j++){nTr4(j,i) = (Tr4(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 17;j++){nTe4(j,i) = ((Te4(j,i) - fMean[i])/fVariance[i]);}
	}
	
		aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te5(aIndx,j) = rawData(i,j);
			}
			gT5(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr5(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 17;i++){
		gT5(i,0) = rawData(gT5(i,0),9);
		gT5(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-17);j++){fMean[i] += Tr5(j,i);}
		fMean[i] /= (214-17);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-17);j++){fVariance[i] += ((Tr5(j,i) - fMean[i]) * (Tr5(j,i) - fMean[i]));}
		fVariance[i] /= (213-17);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-17);j++){nTr5(j,i) = ((Tr5(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-17);j++){nTr5(j,i) = (Tr5(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 17;j++){nTe5(j,i) = ((Te5(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te6(aIndx,j) = rawData(i,j);
			}
			gT6(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr6(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 26;i++){
		gT6(i,0) = rawData(gT6(i,0),9);
		gT6(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-26);j++){fMean[i] += Tr6(j,i);}
		fMean[i] /= (214-26);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-26);j++){fVariance[i] += ((Tr6(j,i) - fMean[i]) * (Tr6(j,i) - fMean[i]));}
		fVariance[i] /= (213-26);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-26);j++){nTr6(j,i) = ((Tr6(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-26);j++){nTr6(j,i) = (Tr6(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 26;j++){nTe6(j,i) = ((Te6(j,i) - fMean[i])/fVariance[i]);}
	}
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te7(aIndx,j) = rawData(i,j);
			}
			gT7(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr7(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 22;i++){
		gT7(i,0) = rawData(gT7(i,0),9);
		gT7(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-22);j++){fMean[i] += Tr7(j,i);}
		fMean[i] /= (214-22);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-22);j++){fVariance[i] += ((Tr7(j,i) - fMean[i]) * (Tr7(j,i) - fMean[i]));}
		fVariance[i] /= (213-22);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-22);j++){nTr7(j,i) = ((Tr7(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-22);j++){nTr7(j,i) = (Tr7(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 22;j++){nTe7(j,i) = ((Te7(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te8(aIndx,j) = rawData(i,j);
			}
			gT8(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr8(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 24;i++){
		gT8(i,0) = rawData(gT8(i,0),9);
		gT8(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-24);j++){fMean[i] += Tr8(j,i);}
		fMean[i] /= (214-24);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-24);j++){fVariance[i] += ((Tr8(j,i) - fMean[i]) * (Tr8(j,i) - fMean[i]));}
		fVariance[i] /= (213-24);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-24);j++){nTr8(j,i) = ((Tr8(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-24);j++){nTr8(j,i) = (Tr8(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 24;j++){nTe8(j,i) = ((Te8(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te9(aIndx,j) = rawData(i,j);
			}
			gT9(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr9(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 26;i++){
		gT9(i,0) = rawData(gT9(i,0),9);
		gT9(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-26);j++){fMean[i] += Tr9(j,i);}
		fMean[i] /= (214-26);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-26);j++){fVariance[i] += ((Tr9(j,i) - fMean[i]) * (Tr9(j,i) - fMean[i]));}
		fVariance[i] /= (213-26);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-26);j++){nTr9(j,i) = ((Tr9(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-26);j++){nTr9(j,i) = (Tr9(j,i) - 1);}
		}
	}
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 26;j++){nTe9(j,i) = ((Te9(j,i) - fMean[i])/fVariance[i]);}
	}
	
	aIndx = 0;
	bIndx = 0;
	for(i = 0;i < 214;i++){
		if(i == (curIndx-1)){
			for(j = 0;j < 9;j++){
				Te10(aIndx,j) = rawData(i,j);
			}
			gT10(aIndx,0) = i;
			aIndx++;
			fin >> curIndx;
		}else{
			for(j = 0;j < 10;j++){
				Tr10(bIndx,j) = rawData(i,j);
			}
			bIndx++;
		}
	}
	for(i = 0;i < 18;i++){
		gT10(i,0) = rawData(gT10(i,0),9);
		gT10(i,0)--;
	}
	
	// Reset Mean and Variance
	for(i = 0;i < 9;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Determine feature means
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-18);j++){fMean[i] += Tr10(j,i);}
		fMean[i] /= (214-18);
	}
	
	// Determine feature variances
	for(i = 0;i < 9;i++){
		for(j = 0;j < (214-18);j++){fVariance[i] += ((Tr10(j,i) - fMean[i]) * (Tr10(j,i) - fMean[i]));}
		fVariance[i] /= (213-18);
	}
	
	// Normalize training
	for(i = 0;i < 10;i++){
		if(i != 9){
			for(j = 0;j < (214-18);j++){nTr10(j,i) = ((Tr10(j,i) - fMean[i])/fVariance[i]);}
		}else{
			for(j = 0;j < (214-18);j++){nTr10(j,i) = (Tr10(j,i) - 1);}
		}
	}
	
	// Normalize testing
	for(i = 0;i < 9;i++){
		for(j = 0;j < 18;j++){nTe10(j,i) = ((Te10(j,i) - fMean[i])/fVariance[i]);}
	}
	
	fin.close();
	for(j = 1;j < 70;j++){
		avgOverall = 0.0;
		for(i = 0;i < 15;i++){
			curClass = knn2(nTr1,subMatrix(nTe1,i,0,i,8),9,7,j);
			if(curClass == gT1(i,0)){avgOverall++;}
		}
		for(i = 0;i < 21;i++){
			curClass = knn2(nTr2,subMatrix(nTe2,i,0,i,8),9,7,j);
			if(curClass == gT2(i,0)){avgOverall++;}
		}
		for(i = 0;i < 28;i++){
			curClass = knn2(nTr3,subMatrix(nTe3,i,0,i,8),9,7,j);
			if(curClass == gT3(i,0)){avgOverall++;}
		}
		for(i = 0;i < 17;i++){
			curClass = knn2(nTr4,subMatrix(nTe4,i,0,i,8),9,7,j);
			if(curClass == gT4(i,0)){avgOverall++;}
		}
		for(i = 0;i < 17;i++){
			curClass = knn2(nTr5,subMatrix(nTe5,i,0,i,8),9,7,j);
			if(curClass == gT5(i,0)){avgOverall++;}
		}
		for(i = 0;i < 26;i++){
			curClass = knn2(nTr6,subMatrix(nTe6,i,0,i,8),9,7,j);
			if(curClass == gT6(i,0)){avgOverall++;}
		}
		for(i = 0;i < 22;i++){
			curClass = knn2(nTr7,subMatrix(nTe7,i,0,i,8),9,7,j);
			if(curClass == gT7(i,0)){avgOverall++;}
		}
		for(i = 0;i < 24;i++){
			curClass = knn2(nTr8,subMatrix(nTe8,i,0,i,8),9,7,j);
			if(curClass == gT8(i,0)){avgOverall++;}
		}
		for(i = 0;i < 26;i++){
			curClass = knn2(nTr9,subMatrix(nTe9,i,0,i,8),9,7,j);
			if(curClass == gT9(i,0)){avgOverall++;}
		}
		for(i = 0;i < 18;i++){
			curClass = knn2(nTr10,subMatrix(nTe10,i,0,i,8),9,7,j);
			if(curClass == gT10(i,0)){avgOverall++;}
		}
		fout << j + 1 <<" KNN Avg: " << (avgOverall / 214.0) * 100.0 << "\n";
	}
	avgOverall = 0.0;
	for(i = 0;i < 15;i++){
		curClass = knn1(nTr1,subMatrix(nTe1,i,0,i,8),9,7,1);
		if(curClass == gT1(i,0)){avgOverall++;}
	}
	for(i = 0;i < 21;i++){
		curClass = knn1(nTr2,subMatrix(nTe2,i,0,i,8),9,7,1);
		if(curClass == gT2(i,0)){avgOverall++;}
	}
	for(i = 0;i < 28;i++){
		curClass = knn1(nTr3,subMatrix(nTe3,i,0,i,8),9,7,1);
		if(curClass == gT3(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn1(nTr4,subMatrix(nTe4,i,0,i,8),9,7,1);
		if(curClass == gT4(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn1(nTr5,subMatrix(nTe5,i,0,i,8),9,7,1);
		if(curClass == gT5(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn1(nTr6,subMatrix(nTe6,i,0,i,8),9,7,1);
		if(curClass == gT6(i,0)){avgOverall++;}
	}
	for(i = 0;i < 22;i++){
		curClass = knn1(nTr7,subMatrix(nTe7,i,0,i,8),9,7,1);
		if(curClass == gT7(i,0)){avgOverall++;}
	}
	for(i = 0;i < 24;i++){
		curClass = knn1(nTr8,subMatrix(nTe8,i,0,i,8),9,7,1);
		if(curClass == gT8(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn1(nTr9,subMatrix(nTe9,i,0,i,8),9,7,1);
		if(curClass == gT9(i,0)){avgOverall++;}
	}
	for(i = 0;i < 18;i++){
		curClass = knn1(nTr10,subMatrix(nTe10,i,0,i,8),9,7,1);
		if(curClass == gT10(i,0)){avgOverall++;}
	}
	fout << "1 KNN Avg: " << (avgOverall / 214.0) * 100.0 << "\n";
	
	avgOverall = 0.0;
	for(i = 0;i < 15;i++){
		curClass = knn3(nTr1,subMatrix(nTe1,i,0,i,8),9,7,1);
		if(curClass == gT1(i,0)){avgOverall++;}
	}
	for(i = 0;i < 21;i++){
		curClass = knn3(nTr2,subMatrix(nTe2,i,0,i,8),9,7,1);
		if(curClass == gT2(i,0)){avgOverall++;}
	}
	for(i = 0;i < 28;i++){
		curClass = knn3(nTr3,subMatrix(nTe3,i,0,i,8),9,7,1);
		if(curClass == gT3(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn3(nTr4,subMatrix(nTe4,i,0,i,8),9,7,1);
		if(curClass == gT4(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn3(nTr5,subMatrix(nTe5,i,0,i,8),9,7,1);
		if(curClass == gT5(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn3(nTr6,subMatrix(nTe6,i,0,i,8),9,7,1);
		if(curClass == gT6(i,0)){avgOverall++;}
	}
	for(i = 0;i < 22;i++){
		curClass = knn3(nTr7,subMatrix(nTe7,i,0,i,8),9,7,1);
		if(curClass == gT7(i,0)){avgOverall++;}
	}
	for(i = 0;i < 24;i++){
		curClass = knn3(nTr8,subMatrix(nTe8,i,0,i,8),9,7,1);
		if(curClass == gT8(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn3(nTr9,subMatrix(nTe9,i,0,i,8),9,7,1);
		if(curClass == gT9(i,0)){avgOverall++;}
	}
	for(i = 0;i < 18;i++){
		curClass = knn3(nTr10,subMatrix(nTe10,i,0,i,8),9,7,1);
		if(curClass == gT10(i,0)){avgOverall++;}
	}
	fout << "3 KNN Avg: " << (avgOverall / 214.0) * 100.0 << "\n";
	
	avgOverall = 0.0;
	for(i = 0;i < 15;i++){
		curClass = knn4(nTr1,subMatrix(nTe1,i,0,i,8),9,7,1);
		if(curClass == gT1(i,0)){avgOverall++;}
	}
	for(i = 0;i < 21;i++){
		curClass = knn4(nTr2,subMatrix(nTe2,i,0,i,8),9,7,1);
		if(curClass == gT2(i,0)){avgOverall++;}
	}
	for(i = 0;i < 28;i++){
		curClass = knn4(nTr3,subMatrix(nTe3,i,0,i,8),9,7,1);
		if(curClass == gT3(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn4(nTr4,subMatrix(nTe4,i,0,i,8),9,7,1);
		if(curClass == gT4(i,0)){avgOverall++;}
	}
	for(i = 0;i < 17;i++){
		curClass = knn4(nTr5,subMatrix(nTe5,i,0,i,8),9,7,1);
		if(curClass == gT5(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn4(nTr6,subMatrix(nTe6,i,0,i,8),9,7,1);
		if(curClass == gT6(i,0)){avgOverall++;}
	}
	for(i = 0;i < 22;i++){
		curClass = knn4(nTr7,subMatrix(nTe7,i,0,i,8),9,7,1);
		if(curClass == gT7(i,0)){avgOverall++;}
	}
	for(i = 0;i < 24;i++){
		curClass = knn4(nTr8,subMatrix(nTe8,i,0,i,8),9,7,1);
		if(curClass == gT8(i,0)){avgOverall++;}
	}
	for(i = 0;i < 26;i++){
		curClass = knn4(nTr9,subMatrix(nTe9,i,0,i,8),9,7,1);
		if(curClass == gT9(i,0)){avgOverall++;}
	}
	for(i = 0;i < 18;i++){
		curClass = knn4(nTr10,subMatrix(nTe10,i,0,i,8),9,7,1);
		if(curClass == gT10(i,0)){avgOverall++;}
	}
	fout << "4 KNN Avg: " << (avgOverall / 214.0) * 100.0 << "\n";
	
	fout.close();
	fout.open("idealNeuralData.csv");
	if(!fout.is_open()){
		cerr << "Error opening file: neuralData.csv\n";
		exit(1);
	}
	fout2.open("convNeuralData.csv");
	if(!fout.is_open()){
		cerr << "Error opening file: ConvData.csv\n";
		exit(1);
	}
	fout3.open("avgNeuralData.csv");
	if(!fout.is_open()){
		cerr << "Error opening file: AvgNeuralData.csv\n";
		exit(1);
	}
	/* ********************BPNN Section******************** */
	
	// Do 500 iterations of the neural network
	wCheck = false;
	for(m = 0;m < 500;m++){
		// Initialize Bias, gTruth, train, and output
		gTruth(0,0) = 0.0;gTruth(1,0) = 1.0;gTruth(2,0) = 1.0;gTruth(3,0) = 0.0;
		bias(0,0) = -1.0;bias(0,1) = -1.0;bias(0,2) = -1.0;
		train(0,0) = 0.0;train(0,1) = 0.0;train(1,0) = 1.0;train(1,1) = 0.0;
		train(2,0) = 0.0;train(2,1) = 1.0;train(3,0) = 1.0;train(3,1) = 1.0;
		output(0,0) = 0.0;output(1,0) = 0.0;output(2,0) = 0.0;output(3,0) = 0.0;
	
		// Initialize weights
		for(i = 0;i < 3;i++){
			for(j = 0;j < 3;j++){
				weights(i,j) = rand() / double(RAND_MAX);
				if((rand() % 2) == 1){weights(i,j) = weights(i,j) * -1.0;}
			}
		}
		// Train Epoch START
		for(i = 0;i < 500000;i++){
			for(j = 0;j < 4;j++){
				// hidden layer
				hid1 = bias(0,0) * weights(0,0) + train(j,0) * weights(0,1) + train(j,1) * weights(0,2);
				hid2 = bias(0,1) * weights(1,0) + train(j,0) * weights(1,1) + train(j,1) * weights(1,2);
				thres1 = sigmoid(hid1);
				thres2 = sigmoid(hid2);
				
				// output layer
				hid3 = bias(0,2) * weights(2,0) + thres1 * weights(1,2) + thres2 * weights(2,2);
				output(j,0) = sigmoid(hid3);
				
				// Calc deltas
				delta3 = output(j,0) * (1 - output(j,0)) * (gTruth(j,0) - output(j,0));
				delta1 = thres1 * (1 - thres1) * weights(2,1) * delta3;
				delta2 = thres2 * (1 - thres2) * weights(2,2) * delta3;
				
				// Adjust weights from delta vals
				for(k = 0;k < 3;k++){
					// bias
					if(k == 0){
						weights(0,k) = weights(0,k) + lRate * bias(0,0) * delta1;
						weights(1,k) = weights(1,k) + lRate * bias(0,1) * delta2;
						weights(2,k) = weights(2,k) + lRate * bias(0,2) * delta3;
					// weights
					}else{
						weights(0,k) = weights(0,k) + lRate * train(j,0) * delta1;
						weights(1,k) = weights(1,k) + lRate * train(j,1) * delta2;
						if(k == 1){
							weights(2,k) = weights(2,k) + lRate * thres1 * delta3;
						}else{
							weights(2,k) = weights(2,k) + lRate * thres2 * delta3;
						}
					}
				}
			}
			check = true;
			if(output(0,0) >= .5){check = false;}
			else if(output(1,0) <= .5){check = false;}
			else if(output(2,0) <= .5){check = false;}
			else if(output(3,0) >= .5){check = false;}
			outMove(i,0) = output(0,0);
			outMove(i,1) = output(1,0);
			outMove(i,2) = output(2,0);
			outMove(i,3) = output(3,0);
			
			if(check == true){
				fout2 << "Round: ," << m + 1 << ", CONVERGED @: ," << i + 1 << ", epochs\n";
				if(wCheck == false){
					fout << "IDEAL BPNN OUTPUTS CONVERGENCE:  round: " << m + 1 << " epochs: " << i + 1 << "\n\n";
					for(j = 0;j < i + 1;j++){
						for(k = 0;k < 4;k++){
							if(outMove(j,k) != 0.0){
								fout << outMove(j,k) << ",";
							}
						}
						fout << "\n";
					}
					wCheck = true;
				}
				break;
			}
		}
		rCount = 0;
		for(j = 0;j < 4;j++){
			if(round(output(j,0)) == gTruth(j,0)){rCount++;}
		}
		AvgAcc(m,0) = ((float)rCount / 4.0) * 100.0;
		
		if((m % 50) == 0){
			cout << m << " rounds complete\n";
		}
		// Train Epoch END
	}
	fout3 << "ROUND AVERAGES:\n\n";
	for( i = 0;i < 500;i++){
		fout3 << i << "," << AvgAcc(i,0) << "\n";
	}
	avgOverall = 0.0;
	fout3 << "AVG ACCURACY: ";
	for(i = 0;i < 500;i++){
		avgOverall += AvgAcc(i,0);
	}
	fout3 << (avgOverall / 500) << "\n";
	fout.close();
	fout2.close();
	fout3.close();
}//END OF MAIN

float sigmoid(float x){
	return (1.0 / ( 1.0 + exp((x * -1.0))));
}
float sigmund(float x){
	return(((2.0 * 1.716) / (1 + exp((-2.0 / 3.0) * x))) - 1.716);
}

int knn1(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k) 
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
    dist(0,i) = sqrt(euc(subMatrix(train,i,0,i,nf-1),tedata));
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

int knn2(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k) 
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
    dist(0,i) = euc(subMatrix(train,i,0,i,nf-1),tedata);
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

int knn3(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k) 
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
    dist(0,i) = sqrt((euc(subMatrix(train,i,0,i,nf-1),tedata) * euc(subMatrix(train,i,0,i,nf-1),tedata) * euc(subMatrix(train,i,0,i,nf-1),tedata)));
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

int knn4(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k) 
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
    dist(0,i) = sqrt((euc(subMatrix(train,i,0,i,nf-1),tedata) * euc(subMatrix(train,i,0,i,nf-1),tedata)
	* euc(subMatrix(train,i,0,i,nf-1),tedata) * euc(subMatrix(train,i,0,i,nf-1),tedata)));
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