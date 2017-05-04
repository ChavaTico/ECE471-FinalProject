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

int knn(const Matrix &train, const Matrix &tedata, const int nf, const int c, const int k);
void kMean(const Matrix& orgImg,const Matrix& means,const Matrix& pClass,int k);
void WTA(const Matrix& orgImg,const Matrix& means,const Matrix& pClass,int k);
int main(int argc, char** argv){
	Matrix Train(891,8); Matrix nTrain(891,8); Matrix nEgnValTrain(1,7); Matrix nEgnVecTrain(7,7); Matrix nTrainPCA(891,6);
	Matrix Test(418,7); Matrix nTest(418,7); Matrix nTestPCA(418,5); Matrix nTestResultKNN(418,1); Matrix nTestResultC3(418,1);
	Matrix finalResult(418,1);
	Matrix priorProb(2,1);
	Matrix KM256(256,5);
	Matrix WTA256(256,5);
	Matrix KM128(128,5);
	Matrix WTA128(128,5);
	Matrix KM64(64,5);
	Matrix WTA64(64,5);
	Matrix KM32(32,5);
	Matrix WTA32(32,5);
	Matrix KM2(2,5);
	Matrix WTA2(2,5);
	
	string trFile = argv[1];
	string teFile = argv[2];
	string junk = "";
	string nextLine = "";
	ofstream fout; 
	ifstream fin;
	int pClass = 0;
	int sex = 0;
	int sibSp = 0;
	int pArch = 0;
	float fare = 0.0;
	float dead = 0.0;
	float alive = 0.0;
	int cabNum = 0;
	int embark = 0;
	int surv = 0;
	int i =0,j = 0,k = 0,l = 0,m = 0,n = 0,p = 0;
	float nSum = 0.0;
	char nextChar;
	float fMean[7];
	float fVariance[7];
	
	for(i = 0;i < 7;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	// Randomly assign vals to cluster matrices
	srand(time(NULL));
	for(i = 0;i < 256;i++){
		for(j = 0;j < 5;j++){
			KM256(i,j) = rand() / RAND_MAX;
			WTA256(i,j) = rand() / RAND_MAX;
			if(i < 128){
				KM128(i,j) = rand() / RAND_MAX;
				WTA128(i,j) = rand() / RAND_MAX;
			}
			if(i < 64){
				KM64(i,j) = rand() / RAND_MAX;
				WTA64(i,j) = rand() / RAND_MAX;
			}
			if(i < 32){
				KM32(i,j) = rand() / RAND_MAX;
				WTA32(i,j) = rand() / RAND_MAX;
			}
			if(i < 2){
				KM2(i,j) = rand() / RAND_MAX;
				WTA2(i,j) = rand() / RAND_MAX;
			}
		}
	}
	
	// Open Testing
	fin.open(teFile.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << teFile << "\n";
		exit(1);
	}
	
	// Read Testing in
	fin >> junk;
	for(i = 0;i < 418;i++){
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		pClass = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		if(nextLine == "male"){sex = 50;}else{sex = 100;}
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		sibSp = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		pArch = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		fare = atof(nextLine.c_str());
		cabNum = 0;
		fin.get(nextChar);
		if(nextChar != ','){cabNum++;}
		while(nextChar != ','){
			if(nextChar == ' '){cabNum++;}
			fin.get(nextChar);
		}
		getline(fin,nextLine,'\n');
		if(nextLine[0] == 'S'){embark = 2;}
		if(nextLine[0] == 'C'){embark = 4;}
		if(nextLine[0] == 'Q'){embark = 6;}
		Test(i,0) = pClass;Test(i,1) = sex;Test(i,2) = sibSp;Test(i,3) = pArch;Test(i,4) = fare;Test(i,5) = cabNum;Test(i,6) = embark;
		//cout << pClass << "  " << sex << "  " << sibSp << "  " << pArch << "  " << fare << "  " << cabNum << "  " << embark << "\n";
	}
	fin.close();
	
	// Open Training
	fin.open(trFile.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << trFile << "\n";
		exit(1);
	}
	
	// Read Training in
	fin >> junk;
	for(i = 0;i < 891;i++){
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		surv = atoi(nextLine.c_str());
		if(surv == 0){dead++;}else{alive++;}
		getline(fin,nextLine,',');
		pClass = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		if(nextLine == "male"){sex = 50;}else{sex = 100;}
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		sibSp = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		pArch = atoi(nextLine.c_str());
		getline(fin,nextLine,',');
		getline(fin,nextLine,',');
		fare = atof(nextLine.c_str());
		cabNum = 0;
		fin.get(nextChar);
		if(nextChar != ','){cabNum++;}
		while(nextChar != ','){
			if(nextChar == ' '){cabNum++;}
			fin.get(nextChar);
		}
		getline(fin,nextLine,'\n');
		if(nextLine[0] == 'S'){embark = 2;}
		if(nextLine[0] == 'C'){embark = 4;}
		if(nextLine[0] == 'Q'){embark = 6;}
		Train(i,0) = surv;Train(i,1) = pClass;Train(i,2) = sex;Train(i,3) = sibSp;Train(i,4) = pArch;Train(i,5) = fare;Train(i,6) = cabNum;Train(i,7) = embark;
		//cout << surv << "  " << pClass << "  " << sex << "  " << sibSp << "  " << pArch << "  " << fare << "  " << cabNum << "  " << embark << "\n";
	}
	fin.close();
	priorProb(0,0) = dead/891.0;
	priorProb(1,0) = alive/891.0;
	
	// Determine feature means
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){fMean[i-1] += Train(j,i);}
		fMean[i-1] /= 891;
	}
	
	// Determine feature variances
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){fVariance[i-1] += ((Train(j,i) - fMean[i-1]) * (Train(j,i) - fMean[i-1]));}
		fVariance[i-1] /= (891-1);
	}
	for(i = 0;i < 7;i++){
		//cout << ":::" << fMean[i] << ":::" << fVariance[i] << "\n";
	}
	
	// Normalize training
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){nTrain(j,i) = ((Train(j,i) - fMean[i-1])/fVariance[i-1]);}
	}
	
	// Normalize testing
	for(i = 0;i < 7;i++){
		for(j = 0;j < 418;j++){nTest(j,i) = ((Test(j,i) - fMean[i])/fVariance[i]);}
	}

	// Add classes to nTrain
	for(i = 0;i < 891;i++){
		nTrain(i,0) = Train(i,0);
	}
	
	for(i = 0;i < 418;i++){
		for(j = 0;j < 7;j++){
			//cout << nTest(i,j) << " ";
		}
		//cout << "\n";
	}
	
	//PCA Training
	Matrix jTrain = cov(subMatrix(nTrain,0,1,890,7),7) * (891 - 1);
	jacobi(jTrain,nEgnValTrain,nEgnVecTrain);
	
	// This shows me I can discard the bottom two vectors
	
	float eigenSum = 0.0;
	for(i = 0;i < 7;i++){eigenSum += nEgnValTrain(0,i);}
	
	cout << "eigen vals:\n";
	for(i = 0;i < 7;i++){cout << "  " << nEgnValTrain(0,i) << "   " << (nEgnValTrain(0,i)/eigenSum) * 100.0 << "%\n";}
	cout << "eigen vectors:\n";
	for(i = 0;i < 7;i++){
		for(j = 0;j < 7;j++){
			cout << setw(4) << left << nEgnVecTrain(i,j) << " ";
		}
		cout << "\n\n";
	}
	cout << "total eigen sum: ";
	cout << setprecision(10) <<  eigenSum << "\n\n";
	
	// Generate the PCA transpose on the training data
	/* Note for later ease of processing the class vector is moved to the end here */
	for(i = 0;i < 891;i++){
		for(j = 0;j < 5;j++){
			nTrainPCA(i,j) = mtod(transpose(subMatrix(nEgnVecTrain,0,j,6,j)) ->* transpose(subMatrix(nTrain,i,1,i,7)));
		}
	}
	for(i = 0;i < 891;i++){
		nTrainPCA(i,5) = nTrain(i,0);
	}
	
	// Generate the PCA transpose on the testing data
	for(i = 0;i < 418;i++){
		for(j = 0;j < 5;j++){
			nTestPCA(i,j) = mtod(transpose(subMatrix(nEgnVecTrain,0,j,6,j)) ->* transpose(subMatrix(nTest,i,0,i,6)));
		}
	}
	
	fout.open("trainMod.txt");
	for(i = 0;i < 891;i++){
		for(j = 0;j < 6;j++){
			fout << nTrainPCA(i,j) << " ";
		}
		fout << endl;
	}
	fout.close();
	
	fout.open("testMod.txt");
	for(i = 0;i < 418;i++){
		for(j = 0;j < 5;j++){
			fout << nTestPCA(i,j) << " ";
		}
		fout << endl;
	}
	fout.close();
	
	// PCA Training MPP Cases
	double corClassOne = 0;
	double corClassTwo = 0;
	double corClassThree = 0;
	for(i = 0;i < 891;i++){
		if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
		if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
		if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
	}
	
	cout << "MPP Training Accuracy\nCase1: " << (corClassOne/891.0) * 100;
	cout << "\nCase2: " << (corClassTwo/891.0) * 100;
	cout << "\nCase3: " << (corClassThree/891.0) * 100 << "\n";
	corClassOne = 0;
	corClassTwo = 0;
	corClassThree = 0;
	for(i = 0;i < 891;i++){
		if(i < 50){
			if(mpp(subMatrix(nTrainPCA,0,0,50,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,50,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,50,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 100){
			if(mpp(subMatrix(nTrainPCA,0,0,100,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,100,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,100,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 150){
			if(mpp(subMatrix(nTrainPCA,0,0,150,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,150,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,150,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 200){
			if(mpp(subMatrix(nTrainPCA,0,0,200,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,200,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,200,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 250){
			if(mpp(subMatrix(nTrainPCA,0,0,250,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,250,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,250,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 300){
			if(mpp(subMatrix(nTrainPCA,0,0,300,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,300,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,300,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 350){
			if(mpp(subMatrix(nTrainPCA,0,0,350,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,350,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,350,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 400){
			if(mpp(subMatrix(nTrainPCA,0,0,400,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,400,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,400,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 450){
			if(mpp(subMatrix(nTrainPCA,0,0,450,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,450,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,450,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 500){
			if(mpp(subMatrix(nTrainPCA,0,0,500,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,500,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,500,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 550){
			if(mpp(subMatrix(nTrainPCA,0,0,550,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,550,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,550,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 600){
			if(mpp(subMatrix(nTrainPCA,0,0,600,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,600,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,600,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 650){
			if(mpp(subMatrix(nTrainPCA,0,0,650,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,650,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,650,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 700){
			if(mpp(subMatrix(nTrainPCA,0,0,700,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,700,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,700,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 750){
			if(mpp(subMatrix(nTrainPCA,0,0,750,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,750,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,750,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 800){
			if(mpp(subMatrix(nTrainPCA,0,0,800,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,800,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,800,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else if(i < 850){
			if(mpp(subMatrix(nTrainPCA,0,0,850,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(subMatrix(nTrainPCA,0,0,850,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(subMatrix(nTrainPCA,0,0,850,5),transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}else{
			if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,1,priorProb) == nTrainPCA(i,5)){corClassOne++;}
			if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,2,priorProb) == nTrainPCA(i,5)){corClassTwo++;}
			if(mpp(nTrainPCA,transpose(subMatrix(nTrainPCA,i,0,i,4)),2,3,priorProb) == nTrainPCA(i,5)){corClassThree++;}
		}
	}
	cout << "\nMPP Training Accuracy 18-Fold Cross Validation\nCase1: " << (corClassOne/891.0) * 100;
	cout << "\nCase2: " << (corClassTwo/891.0) * 100;
	cout << "\nCase3: " << (corClassThree/891.0) * 100 << "\n";
	
	/*
	// This was run to determine what the ideal k value was. After running 450 different
	// possible K values, 1 was determined to be the best. This section was commented out
	// for subsequent runs of the program as it takes a significant amount of time to run
	to completion.
	float corClassK = 0.0;
	int curCorBest = 0.0;
	int curKBest = -1;
	float bestAcc;
	for(i = 0;i < 450;i++){
		for(j = 0;j < 891;j++){
			if(knn(nTrainPCA,subMatrix(nTrainPCA,j,0,j,4),5,2,i) == nTrainPCA(j,5)){corClassK++;}
		}
		if(corClassK > curCorBest){
			curKBest = i;
			curCorBest =  corClassK;
			bestAcc = (curCorBest/891.0) * 100.0;
		}
		corClassK = 0.0;
	}
	
	cout << "\nkNN Training Accuracy\nBest K Val: " << curKBest << "\nBest Acc: " << bestAcc << "\n";
	*/
	
	float curKs = 0.0;
	for(i = 0;i < 891;i++){
		if(i < 50){
			if(knn(subMatrix(nTrainPCA,0,0,50,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 100){
			if(knn(subMatrix(nTrainPCA,0,0,100,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 150){
			if(knn(subMatrix(nTrainPCA,0,0,150,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 200){
			if(knn(subMatrix(nTrainPCA,0,0,200,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 250){
			if(knn(subMatrix(nTrainPCA,0,0,250,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 300){
			if(knn(subMatrix(nTrainPCA,0,0,300,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 350){
			if(knn(subMatrix(nTrainPCA,0,0,350,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 400){
			if(knn(subMatrix(nTrainPCA,0,0,400,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 450){
			if(knn(subMatrix(nTrainPCA,0,0,450,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 500){
			if(knn(subMatrix(nTrainPCA,0,0,500,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 550){
			if(knn(subMatrix(nTrainPCA,0,0,550,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 600){
			if(knn(subMatrix(nTrainPCA,0,0,600,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 650){
			if(knn(subMatrix(nTrainPCA,0,0,650,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 700){
			if(knn(subMatrix(nTrainPCA,0,0,700,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 750){
			if(knn(subMatrix(nTrainPCA,0,0,750,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 800){
			if(knn(subMatrix(nTrainPCA,0,0,800,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else if(i < 850){
			if(knn(subMatrix(nTrainPCA,0,0,850,5),subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}else{
			if(knn(nTrainPCA,subMatrix(nTrainPCA,i,0,i,4),5,2,1) == nTrainPCA(i,5)){curKs++;}
		}
	}
	
	cout << "\nkNN Training Accuracy 18-Fold Cross Validation\nBest K Val: 1\nBest Acc: " << (curKs/891) * 100.0 << "\n";
	
	cout << "\nPossible additional Clusters using KMeans and WTA Clustering:\n";
	kMean(subMatrix(nTrainPCA,0,0,890,4),KM256,subMatrix(nTrainPCA,0,5,890,5),256);
	cout << "256 KMeans Done\n";
		
	kMean(subMatrix(nTrainPCA,0,0,890,4),KM128,subMatrix(nTrainPCA,0,5,890,5),128);
	cout << "128;KMeans Done\n";
	
	kMean(subMatrix(nTrainPCA,0,0,890,4),KM64,subMatrix(nTrainPCA,0,5,890,5),64);
	cout << "64 KMeans Done\n";
	
	kMean(subMatrix(nTrainPCA,0,0,890,4),KM32,subMatrix(nTrainPCA,0,5,890,5),32);
	cout << "32;KMeans Done\n";
	
	kMean(subMatrix(nTrainPCA,0,0,890,4),KM2,subMatrix(nTrainPCA,0,5,890,5),2);
	cout << "2;KMeans Done\n";
	
	WTA(subMatrix(nTrainPCA,0,0,890,4),KM256,subMatrix(nTrainPCA,0,5,890,5),256);
	cout << "256 WTA Done\n";
	
	WTA(subMatrix(nTrainPCA,0,0,890,4),KM128,subMatrix(nTrainPCA,0,5,890,5),128);
	cout << "128;WTA Done\n";
	
	WTA(subMatrix(nTrainPCA,0,0,890,4),KM64,subMatrix(nTrainPCA,0,5,890,5),64);
	cout << "64;WTA Done\n";
	
	WTA(subMatrix(nTrainPCA,0,0,890,4),KM32,subMatrix(nTrainPCA,0,5,890,5),32);
	cout << "32;WTA Done\n";
	
	WTA(subMatrix(nTrainPCA,0,0,890,4),KM2,subMatrix(nTrainPCA,0,5,890,5),2);
	cout << "2;WTA Done\n";
	
	/* ********** Testing data ********** */
	
	// MPP Case 3
	for(i = 0;i < 418;i++){
			nTestResultC3(i,0) = mpp(nTrainPCA,transpose(subMatrix(nTestPCA,i,0,i,4)),2,3,priorProb);
	}
	
	// KNN where K = 1
	
	for(i = 0;i < 418;i++){
			nTestResultKNN(i,0) = knn(nTrainPCA,subMatrix(nTestPCA,i,0,i,4),5,2,1);
	}
	
	cout << "\nClassifier Fusion: KNN and MPP Case 3\n";
	
	// Create true result using Majority Vote and NB
	for(i = 0;i < 418;i++){
		// If the results are equal, MV says take the result
		if(nTestResultC3(i,0) == nTestResultKNN(i,0)){
			finalResult(i,0) = nTestResultC3(i,0);
		
		// If results are not equal, NB says take the result
		// with the higher probability, in this case KNN
		}else{
			finalResult(i,0) = nTestResultKNN(i,0);
		}
	}
	
	// Generate Confusion Matrix
		int b1 = 0,b2 = 0,b3 = 0,b4 = 0;
		for(i = 0;i < 418;i++){
			if((nTestResultC3(i,0) == 0) && (finalResult(i,0) == 0)){b1++;}
			if((nTestResultC3(i,0) == 1) && (finalResult(i,0) == 0)){b2++;}
			if((nTestResultC3(i,0) == 0) && (finalResult(i,0) == 1)){b3++;}
			if((nTestResultC3(i,0) == 1) && (finalResult(i,0) == 1)){b4++;}
		}
		cout << "Confusion Matrices\n";
		cout << "C3;0;1\n";
		cout << "0;" << b1 << ";" << b2 << ";\n";
		cout << "1;" << b3 << ";" << b4 << ";\n";
		cout << "\n\n";
		
		b1 = 0;b2 = 0;b3 = 0;b4 = 0;
		for(i = 0;i < 418;i++){
			if((nTestResultKNN(i,0) == 0) && (finalResult(i,0) == 0)){b1++;}
			if((nTestResultKNN(i,0) == 1) && (finalResult(i,0) == 0)){b2++;}
			if((nTestResultKNN(i,0) == 0) && (finalResult(i,0) == 1)){b3++;}
			if((nTestResultKNN(i,0) == 1) && (finalResult(i,0) == 1)){b4++;}
		}
		cout << "KNN;0;1\n";
		cout << "0;" << b1 << ";" << b2 << ";\n";
		cout << "1;" << b3 << ";" << b4 << ";\n";
		cout << "\n\n";
		
		// Determine metrics of survivors from final data
		int mLive = 0;
		int fLive = 0;
		int c1Live = 0;
		int c2Live = 0;
		int c3Live = 0;
		int totalLive = 0;
		float avgSibLive = 0.0;
		float avgParenLive = 0.0;
		float avgFareLive = 0.0;
		for(i = 0;i < 418;i++){
			if(finalResult(i,0) == 1){
				totalLive++;
				if(Test(i,0) == 1){c1Live++;}
				if(Test(i,0) == 2){c2Live++;}
				if(Test(i,0) == 3){c3Live++;}
				if(Test(i,1) == 50){mLive++;}else{fLive++;}
				avgSibLive += Test(i,2);
				avgParenLive += Test(i,3);
				avgFareLive += Test(i,4);
			}
		}
		
		cout << "METRICS OF SURVIORS:\n";
		cout << "Total Surviors in Test Set: " << totalLive << " out of 418\n";
		cout << "Men: " << mLive << "  Woman: " << fLive << "\n";
		cout << "First Class: " << c1Live << "\n";
		cout << "Second Class: " << c2Live << "\n";
		cout << "Third Class: " << c3Live << "\n";
		cout << "Avg. # of Siblings of Survivor on Board(rounded): " << round(avgSibLive/418.0) << "\n";
		cout << "Avg. # of Parents of Survivor on Board(rounded): " << round(avgParenLive/418.0) << "\n";
		cout << "Avg. Ticket Fare Paid by Survivor: " << setw(5) << avgFareLive/418.0 << "\n";
		cout << "\n";
		
	return 0;
}//END OF MAIN

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

void kMean(const Matrix& orgImg,const Matrix& means,const Matrix& pClass,int k){
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
				curDist = euc(subMatrix(orgImg,i,0,i,4),subMatrix(means,j,0,j,4));
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
		cout << "WCount; " << wrongCount << "\n";
		intCount++;
	}
	cout << "hopped out in;" << intCount << "\n";
	
}

void WTA(const Matrix& orgImg,const Matrix& means,const Matrix& pClass,int k){
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
				curDist = euc(subMatrix(orgImg,i,0,i,4),subMatrix(means,j,0,j,4));
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
				means(pClass(i,0),3) = (means(pClass(i,0),3) + round((lRate * (orgImg(i,3) - means(pClass(i,0),3)))));
				means(pClass(i,0),4) = (means(pClass(i,0),4) + round((lRate * (orgImg(i,4) - means(pClass(i,0),4)))));
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
		cout << "WCount;" << wrongCount << ";lRate;" << lRate << "\n";
		if(wrongCount >= prevWrong){lRate *= 0.5;}
		prevWrong = wrongCount;
	}
	cout << "hopped out in;" << intCount << "\n";
}