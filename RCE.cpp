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
int kNNTest(int k, int teIndx, Matrix test, Matrix tClass, Matrix pProb);
int kNNTrain(int k,int teIndx,Matrix test,Matrix tClass, Matrix pProb);

int main(int arc,char** argv){
	float eigenSum = 0.0,tempEigenSum = 0.0,curDatum = 0.0;
	int i = 0,j = 0,k = 0;
	int curYesCount = 0,curNoCount = 0;
	int case1TrainCor = 0,case2TrainCor = 0,case3TrainCor = 0;
	int case1TestCor = 0,case2TestCor = 0,case3TestCor = 0;
	int case1TrainPCACor = 0,case2TrainPCACor = 0,case3TrainPCACor = 0;
	int case1TestPCACor = 0,case2TestPCACor = 0,case3TestPCACor = 0;
	int case1TrainFLDCor = 0,case2TrainFLDCor = 0,case3TrainFLDCor = 0;
	int case1TestFLDCor = 0,case2TestFLDCor = 0,case3TestFLDCor = 0;
	int kNormCor = 0,kPCACor = 0,kFLDCor = 0;
	int TruePos = 0,TrueNeg = 0,FalsePos = 0,FalseNeg = 0;
	int kBestNeigh = 0,kCurBestClass = 0;
	double c0Best = 0.0,c1Best = 0.0;
	double curKBest = 0.0;
	double nTrainC1Vary = 0.0,nTrainC2Vary = 0.0,nTrainC3Vary = 0.0;
	double nTestC1Vary = 0.0,nTestC2Vary = 0.0,nTestC3Vary = 0.0;
	double PCATrainC1Vary = 0.0,PCATrainC2Vary = 0.0,PCATrainC3Vary = 0.0;
	double PCATestC1Vary = 0.0,PCATestC2Vary = 0.0,PCATestC3Vary = 0.0;
	double FLDTrainC1Vary = 0.0,FLDTrainC2Vary = 0.0,FLDTrainC3Vary = 0.0;
	double FLDTestC1Vary = 0.0,FLDTestC2Vary = 0.0,FLDTestC3Vary = 0.0;
	double kNNTrainNormVary = 0.0,kNNTrainPCAVary = 0.0,kNNTrainFLDVary = 0.0;
	double kNNTestNormVary = 0.0,kNNTestPCAVary = 0.0,kNNTestFLDVary = 0.0;
	double nTrC1V[100],nTrC2V[100],nTrC3V[100];
	double nTeC1V[100],nTeC2V[100],nTeC3V[100];
	double pTrC1V[100],pTrC2V[100],pTrC3V[100];
	double pTeC1V[100],pTeC2V[100],pTeC3V[100];
	double fTrC1V[100],fTrC2V[100],fTrC3V[100];
	double fTeC1V[100],fTeC2V[100],fTeC3V[100];
	double TruePosRatio = 0.0,TrueNegRatio = 0.0,FalsePosRatio = 0.0,FalseNegRatio = 0.0;
	ifstream fin;
	ofstream fout;
	string train_file = argv[1];
	string test_file = argv[2];
	string junk,curClass;
	Matrix trainSet(200,7);
	Matrix trainClass(200,1);
	Matrix nTrainYes(68,7);
	Matrix nTrainNo(132,7);
	Matrix nTrainCovAll(7,7);
	Matrix nTrainCovYes(7,7);
	Matrix nTrainCovNo(7,7);
	Matrix nTrainMeanYes(7,1);
	Matrix nTrainMeanNo(7,1);
	Matrix nTrainSwitchAll(7,7);
	Matrix nTrainSwitchNo(7,7);
	Matrix nTrainSwitchYes(7,7);
	Matrix testSet(332,7);
	Matrix testClass(332,1);
	Matrix normTrainSet(200,7);
	Matrix normTestSet(332,7);
	Matrix nTrainSetClass(200,8);
	Matrix nTestSetClass(332,8);
	Matrix nTrainPCAClass(200,2);
	Matrix nTrainFLDClass(200,2);
	Matrix nTrainPCA(200,1);
	Matrix nTrainFLD(200,1);
	Matrix nTestPCA(332,1);
	Matrix nTestFLD(332,1);
	Matrix nTrainEigenValAll(1,7);
	Matrix nTrainEigenVecAll(7,7);
	Matrix FLD(7,1);
	Matrix PCA(7,1);
	Matrix nTrainDist(200,200);
	Matrix PCATrainDist(200,200);
	Matrix FLDTrainDist(200,200);
	Matrix nTestDist(332,332);
	Matrix PCATestDist(332,332);
	Matrix FLDTestDist(332,332);
	Matrix priorProb(2,1);
	
	double fMean[7];
	double fVariance[7];
	
	for(i = 0;i < 7;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
	}
	
	priorProb(0,0) = 0.5;
	priorProb(1,0) = 0.5;
	
	// Input training
	fin.open(train_file.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << train_file << "\n";
		exit(1);
	}
	for(i = 0;i < 8;i++){fin >> junk;}
	for(i = 0;i < 200;i++){
		for(j = 0;j < 7;j++){
			fin >> curDatum;
			trainSet(i,j) = curDatum;
		}
		fin >> curClass;
		if(curClass == "Yes"){
			trainClass(i,0) = 1;
		}else{
			trainClass(i,0) = 0;
		}
	}
	fin.close();
	
	// Input testing
	fin.open(test_file.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << test_file << "\n";
		exit(1);
	}
	for(i = 0;i < 8;i++){fin >> junk;}
	for(i = 0;i < 332;i++){
		for(j = 0;j < 7;j++){
			fin >> curDatum;
			testSet(i,j) = curDatum;
		}
		fin >> curClass;
		if(curClass == "Yes"){
			testClass(i,0) = 1;
		}else{
			testClass(i,0) = 0;
		}
	}
	fin.close();
	fout.open("EigenData.txt");
	// Determine feature means
	for(i = 0;i < 7;i++){
		for(j = 0;j < 200;j++){fMean[i] += trainSet(j,i);}
		fMean[i] /= 200;
	}
	
	// Determine feature variances
	for(i = 0;i < 7;i++){
		for(j = 0;j < 200;j++){fVariance[i] += ((trainSet(j,i) - fMean[i]) * (trainSet(j,i) - fMean[i]));}
		fVariance[i] /= 199;
	}
	
	// Normalize training
	for(i = 0;i < 7;i++){
		for(j = 0;j < 200;j++){normTrainSet(j,i) = ((trainSet(j,i) - fMean[i])/fVariance[i]);}
	}
	
	// Normalize testing
	for(i = 0;i < 7;i++){
		for(j = 0;j < 332;j++){normTestSet(j,i) = ((testSet(j,i) - fMean[i])/fVariance[i]);}
	}
	for(i = 0;i < 200;i++){
		for(j = 0;j < 7;j++){
			//cout << normTrainSet(i,j) << "  ";
		}
		//cout << "\n";
	}
	// Normtrain Covariance and Switch Matrix
	nTrainCovAll = cov(normTrainSet,7);
	nTrainSwitchAll = nTrainCovAll * 199;
	
	// Normtrain PCA Eigen val generation
	jacobi(nTrainSwitchAll,nTrainEigenValAll,nTrainEigenVecAll);
	
	// This shows me I can discard all but the top vector and still have
	// an error of only approximately 1.3%
	
	fout << "eigen vals:\n";
	for(i = 0;i < 7;i++){fout << "  " << nTrainEigenValAll(0,i) << "\n";}
	fout << "eigen vectors:\n";
	for(i = 0;i < 7;i++){
		for(j = 0;j < 7;j++){
			fout << setw(4) << left << nTrainEigenVecAll(i,j) << " ";
		}
		fout << "\n\n";
	}
	fout << "total eigen sum: ";
	eigenSum = 0.0;
	for(i = 0;i < 7;i++){eigenSum += nTrainEigenValAll(0,i);}
	fout << setprecision(10) <<  eigenSum << "\n\n";
	
	// Normtrain PCA generation
	for(i = 0;i < 7;i++){
		PCA(i,0) = nTrainEigenVecAll(i,0);
	}
		
	// Class Matrix Separation
	// I counted ahead of time to conclude there are 68 yes cases and 132 no cases
	// This separation made all the future calculations easier
	curYesCount = 0;
	curNoCount = 0;
	for(i = 0;i < 200;i++){
		if(trainClass(i,0) == 0){
			for(j = 0;j < 7;j++){
				nTrainNo(curNoCount,j) = normTrainSet(i,j);
			}
			curNoCount++;
		}else{
			for(j = 0;j < 7;j++){
				nTrainYes(curYesCount,j) = normTrainSet(i,j);
			}
			curYesCount++;
		}
	}
	fout << "yes: " << curYesCount << "   No: " << curNoCount << endl;
	fout.close();
	cout << "Eigen Done\n";
	fout.open("StaticData.txt");
	// Normtrain Mean and Covar Calc
	nTrainMeanNo = mean(nTrainNo,7);
	nTrainMeanYes = mean(nTrainYes,7);
	nTrainCovNo = cov(nTrainNo,7);
	nTrainCovYes = cov(nTrainYes,7);
	nTrainSwitchNo = nTrainCovNo * 131;
	nTrainSwitchYes = nTrainCovYes * 67;
	
	// Normtrain FLD generation
	FLD = inverse((nTrainSwitchNo + nTrainSwitchYes)) ->* (nTrainMeanNo - nTrainMeanYes);
	
	// Generate the PCA transpose on the training data
	for(i = 0;i < 200;i++){
		nTrainPCA(i,0) = mtod(transpose(PCA) ->* transpose(subMatrix(normTrainSet,i,0,i,6)));
	}
	
	// Generate the FLD transpose on the training data
	for(i = 0;i < 200;i++){
		nTrainFLD(i,0) = mtod(transpose(FLD) ->* transpose(subMatrix(normTrainSet,i,0,i,6)));
	}
	
	// Generate the PCA transpose on the testing data
	for(i = 0;i < 322;i++){
		nTestPCA(i,0) = mtod(transpose(PCA) ->* transpose(subMatrix(normTestSet,i,0,i,6)));
	}
	
	// Generate the FLD transpose on the testing data
	for(i = 0;i < 322;i++){
		nTestFLD(i,0) = mtod(transpose(FLD) ->* transpose(subMatrix(normTestSet,i,0,i,6)));
	}
	// KNN
	
	/* Preprocessing: Creating ALL the distances tables */
	for(i = 0;i < 200;i++){
		for(j = 0;j < 200;j++){
			if(i == j){
				nTrainDist(i,j) = DBL_MAX;
				PCATrainDist(i,j) = DBL_MAX;
				FLDTrainDist(i,j) = DBL_MAX;
			}else{
				nTrainDist(i,j) = euc(transpose(subMatrix(normTrainSet,i,0,i,6)),transpose(subMatrix(normTrainSet,j,0,j,6)));
				PCATrainDist(i,j) = euc(transpose(subMatrix(nTrainPCA,i,0,i,0)),transpose(subMatrix(nTrainPCA,j,0,j,0)));
				FLDTrainDist(i,j) = euc(transpose(subMatrix(nTrainFLD,i,0,i,0)),transpose(subMatrix(nTrainFLD,j,0,j,0)));
			}
		}
	}
	
	for(i = 0;i < 332;i++){
		for(j = 0;j < 332;j++){
			if(i == j){
				nTestDist(i,j) = DBL_MAX;
				PCATestDist(i,j) = DBL_MAX;
				FLDTestDist(i,j) = DBL_MAX;
			}else{
				nTestDist(i,j) = euc(transpose(subMatrix(normTestSet,i,0,i,6)),transpose(subMatrix(normTestSet,j,0,j,6)));
				PCATestDist(i,j) = euc(transpose(subMatrix(nTestPCA,i,0,i,0)),transpose(subMatrix(nTestPCA,j,0,j,0)));
				FLDTestDist(i,j) = euc(transpose(subMatrix(nTestFLD,i,0,i,0)),transpose(subMatrix(nTestFLD,j,0,j,0)));
			}
		}
	}

	// CLASS PROCESSING
	
	// Create training matrix with the class added
	for(i = 0;i < 200;i++){
		for(j = 0;j < 7;j++){
			nTrainSetClass(i,j) = normTrainSet(i,j);
		}
		nTrainSetClass(i,7) = trainClass(i,0);
		nTrainPCAClass(i,0) = nTrainPCA(i,0);
		nTrainPCAClass(i,1) = trainClass(i,0);
		nTrainFLDClass(i,0) = nTrainFLD(i,0);
		nTrainFLDClass(i,1) = trainClass(i,0);
	}
	/* STATIC DATA PROCESSING */
	
	// Training cases
	for(i = 0;i < 200;i++){
		if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,1,priorProb) == trainClass(i,0)){case1TrainCor++;}
		if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,2,priorProb) == trainClass(i,0)){case2TrainCor++;}
		if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,3,priorProb) == trainClass(i,0)){case3TrainCor++;}
		
		if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,1,priorProb) == trainClass(i,0)){case1TrainPCACor++;}
		if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,2,priorProb) == trainClass(i,0)){case2TrainPCACor++;}
		if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,3,priorProb) == trainClass(i,0)){case3TrainPCACor++;}
	
		if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,1,priorProb) == trainClass(i,0)){case1TrainFLDCor++;}
		if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,2,priorProb) == trainClass(i,0)){case2TrainFLDCor++;}
		if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,3,priorProb) == trainClass(i,0)){case3TrainFLDCor++;}
		
	}
	
	// Testing cases
	for(i = 0;i < 332;i++){
		if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,1,priorProb) == testClass(i,0)){case1TestCor++;}
		if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,2,priorProb) == testClass(i,0)){case2TestCor++;}
		if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,3,priorProb) == testClass(i,0)){case3TestCor++;}
		
		if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,1,priorProb) == testClass(i,0)){case1TestPCACor++;}
		if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,2,priorProb) == testClass(i,0)){case2TestPCACor++;}
		if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,3,priorProb) == testClass(i,0)){case3TestPCACor++;}
		
		if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,1,priorProb) == testClass(i,0)){case1TestFLDCor++;}
		if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,2,priorProb) == testClass(i,0)){case2TestFLDCor++;}
		if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,3,priorProb) == testClass(i,0)){case3TestFLDCor++;}
	}
	
	fout << "Stable Prior Probability:\n";
	fout << "normTrain case 1 Percent: " << ((double)case1TrainCor/200.0) * 100.0 << "\n";
	fout << "normTrain case 2 Percent: " << ((double)case2TrainCor/200.0) * 100.0 << "\n";
	fout << "normTrain case 3 Percent: " << ((double)case3TrainCor/200.0) * 100.0 << "\n";
	fout << "normTest case 1 Percent: " << ((double)case1TestCor/332.0) * 100.0 << "\n";
	fout << "normTest case 2 Percent: " << ((double)case2TestCor/332.0) * 100.0 << "\n";
	fout << "normTest case 3 Percent: " << ((double)case3TestCor/332.0) * 100.0 << "\n";
	fout << "nTrainPCA case 1 Percent: " << ((double)case1TrainPCACor/200.0) * 100.0 << "\n";
	fout << "nTrainPCA case 2 Percent: " << ((double)case2TrainPCACor/200.0) * 100.0 << "\n";
	fout << "nTrainPCA case 3 Percent: " << ((double)case3TrainPCACor/200.0) * 100.0 << "\n";
	fout << "nTestPCA case 1 Percent: " << ((double)case1TestPCACor/332.0) * 100.0 << "\n";
	fout << "nTestPCA case 2 Percent: " << ((double)case2TestPCACor/332.0) * 100.0 << "\n";
	fout << "nTestPCA case 3 Percent: " << ((double)case3TestPCACor/332.0) * 100.0 << "\n";
	fout << "nTrainFLD case 1 Percent: " << ((double)case1TrainFLDCor/200.0) * 100.0 << "\n";
	fout << "nTrainFLD case 2 Percent: " << ((double)case2TrainFLDCor/200.0) * 100.0 << "\n";
	fout << "nTrainFLD case 3 Percent: " << ((double)case3TrainFLDCor/200.0) * 100.0 << "\n";
	fout << "nTestFLD case 1 Percent: " << ((double)case1TestFLDCor/332.0) * 100.0 << "\n";
	fout << "nTestFLD case 2 Percent: " << ((double)case2TestFLDCor/332.0) * 100.0 << "\n";
	fout << "nTestFLD case 3 Percent: " << ((double)case3TestFLDCor/332.0) * 100.0 << "\n";

	// kNN DATA PROCESSING
	
	// kNN training cases
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "normTrain kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(i,j,nTrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "normTrain BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "nTrainPCA kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(i,j,PCATrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "nTrainPCA BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "nTrainFLD kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(i,j,FLDTrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "nTrainFLD BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	
	//kNN Test cases
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "normTest kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(i,j,nTestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "normTest BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "nTestPCA kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(i,j,PCATestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "nTestPCA BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	
	kBestNeigh = 0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	fout << "nTestFLD kNN:\n";
	for(i = 1;i < 53;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(i,j,FLDTestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "Cur K = " << i << " percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			kBestNeigh = i;
		}
		kNormCor = 0;
	}
	fout << "nTestFLD BEST kNN Percent: " << curKBest << " K = " << kBestNeigh << "\n";
	fout.close();
	cout << "Static Done\n";
	fout.open("DynamicData.txt");
	
	/* DYNAMIC DATA PROCESSING */
		
	// Training cases
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "Dynamic Prior Probability: \n";
	for(k = 0;k < 100;k++){
		case1TrainCor = 0;case2TrainCor = 0;case3TrainCor = 0;
		case1TestCor = 0;case2TestCor = 0;case3TestCor = 0;
		case1TrainPCACor = 0;case2TrainPCACor = 0;case3TrainPCACor = 0;
		case1TestPCACor = 0;case2TestPCACor = 0;case3TestPCACor = 0;
		case1TrainFLDCor = 0;case2TrainFLDCor = 0;case3TrainFLDCor = 0;
		case1TestFLDCor = 0;case2TestFLDCor = 0;case3TestFLDCor = 0;
		for(i = 0;i < 200;i++){
			if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,1,priorProb) == trainClass(i,0)){case1TrainCor++;}
			if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,2,priorProb) == trainClass(i,0)){case2TrainCor++;}
			if(mpp(nTrainSetClass,transpose(subMatrix(normTrainSet,i,0,i,6)),2,3,priorProb) == trainClass(i,0)){case3TrainCor++;}
		
			if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,1,priorProb) == trainClass(i,0)){case1TrainPCACor++;}
			if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,2,priorProb) == trainClass(i,0)){case2TrainPCACor++;}
			if(mpp(nTrainPCAClass,subMatrix(nTrainPCA,i,0,i,0),2,3,priorProb) == trainClass(i,0)){case3TrainPCACor++;}
	
			if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,1,priorProb) == trainClass(i,0)){case1TrainFLDCor++;}
			if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,2,priorProb) == trainClass(i,0)){case2TrainFLDCor++;}
			if(mpp(nTrainFLDClass,subMatrix(nTrainFLD,i,0,i,0),2,3,priorProb) == trainClass(i,0)){case3TrainFLDCor++;}
		}
	
		// Testing cases
		for(i = 0;i < 332;i++){
			if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,1,priorProb) == testClass(i,0)){case1TestCor++;}
			if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,2,priorProb) == testClass(i,0)){case2TestCor++;}
			if(mpp(nTrainSetClass,transpose(subMatrix(normTestSet,i,0,i,6)),2,3,priorProb) == testClass(i,0)){case3TestCor++;}
		
			if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,1,priorProb) == testClass(i,0)){case1TestPCACor++;}
			if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,2,priorProb) == testClass(i,0)){case2TestPCACor++;}
			if(mpp(nTrainPCAClass,subMatrix(nTestPCA,i,0,i,0),2,3,priorProb) == testClass(i,0)){case3TestPCACor++;}
		
			if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,1,priorProb) == testClass(i,0)){case1TestFLDCor++;}
			if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,2,priorProb) == testClass(i,0)){case2TestFLDCor++;}
			if(mpp(nTrainFLDClass,subMatrix(nTestFLD,i,0,i,0),2,3,priorProb) == testClass(i,0)){case3TestFLDCor++;}
		}

		nTrC1V[k] = ((double)case1TrainCor/200.0) * 100.0;
		nTrC2V[k] = ((double)case2TrainCor/200.0) * 100.0;
		nTrC3V[k] = ((double)case3TrainCor/200.0) * 100.0;
		nTeC1V[k] = ((double)case1TestCor/332.0) * 100.0;
		nTeC2V[k] = ((double)case2TestCor/332.0) * 100.0;
		nTeC3V[k] = ((double)case3TestCor/332.0) * 100.0;
		pTrC1V[k] = ((double)case1TrainPCACor/200.0) * 100.0;
		pTrC2V[k] = ((double)case2TrainPCACor/200.0) * 100.0;
		pTrC3V[k] = ((double)case3TrainPCACor/200.0) * 100.0;
		pTeC1V[k] = ((double)case1TestPCACor/332.0) * 100.0;
		pTeC2V[k] = ((double)case2TestPCACor/332.0) * 100.0;
		pTeC3V[k] = ((double)case3TestPCACor/332.0) * 100.0;
		fTrC1V[k] = ((double)case1TrainFLDCor/200.0) * 100.0;
		fTrC2V[k] = ((double)case2TrainFLDCor/200.0) * 100.0;
		fTrC3V[k] = ((double)case3TrainFLDCor/200.0) * 100.0;
		fTeC1V[k] = ((double)case1TestFLDCor/332.0) * 100.0;
		fTeC2V[k] = ((double)case2TestFLDCor/332.0) * 100.0;
		fTeC3V[k] = ((double)case3TestFLDCor/332.0) * 100.0;

		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	nTrainC1Vary = 0.0;
	nTrainC2Vary = 0.0;
	nTrainC3Vary = 0.0;
	nTestC1Vary = 0.0;
	nTestC2Vary = 0.0;
	nTestC3Vary = 0.0;
	PCATrainC1Vary = 0.0;
	PCATrainC2Vary = 0.0;
	PCATrainC3Vary = 0.0;
	PCATestC1Vary = 0.0;
	PCATestC2Vary = 0.0;
	PCATestC3Vary = 0.0;
	FLDTrainC1Vary = 0.0;
	FLDTrainC2Vary = 0.0;
	FLDTrainC3Vary = 0.0;
	FLDTestC1Vary = 0.0;
	FLDTestC2Vary = 0.0;
	FLDTestC3Vary = 0.0;
	
	for(i = 0;i < 100;i++){
		if(nTrC1V[i] > nTrainC1Vary){nTrainC1Vary = nTrC1V[i];}
		if(nTrC2V[i] > nTrainC2Vary){nTrainC2Vary = nTrC2V[i];}
		if(nTrC3V[i] > nTrainC3Vary){nTrainC3Vary = nTrC3V[i];}
		if(nTeC1V[i] > nTestC1Vary){nTestC1Vary = nTeC1V[i];}
		if(nTeC2V[i] > nTestC2Vary){nTestC2Vary = nTeC2V[i];}
		if(nTeC3V[i] > nTestC3Vary){nTestC3Vary = nTeC3V[i];}
		if(pTrC1V[i] > PCATrainC1Vary){PCATrainC1Vary = pTrC1V[i];}
		if(pTrC2V[i] > PCATrainC2Vary){PCATrainC2Vary = pTrC2V[i];}
		if(pTrC3V[i] > PCATrainC3Vary){PCATrainC3Vary = pTrC3V[i];}
		if(pTeC1V[i] > PCATestC1Vary){PCATestC1Vary = pTeC1V[i];}
		if(pTeC2V[i] > PCATestC2Vary){PCATestC2Vary = pTeC2V[i];}
		if(pTeC3V[i] > PCATestC3Vary){PCATestC3Vary = pTeC3V[i];}
		if(fTrC1V[i] > FLDTrainC1Vary){FLDTrainC1Vary = fTrC1V[i];}
		if(fTrC2V[i] > FLDTrainC2Vary){FLDTrainC2Vary = fTrC2V[i];}
		if(fTrC3V[i] > FLDTrainC3Vary){FLDTrainC3Vary = fTrC3V[i];}
		if(fTeC1V[i] > FLDTestC1Vary){FLDTestC1Vary = fTeC1V[i];}
		if(fTeC2V[i] > FLDTestC2Vary){FLDTestC2Vary = fTeC2V[i];}
		if(fTeC3V[i] > FLDTestC3Vary){FLDTestC3Vary = fTeC3V[i];}
	}
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTrC1:\n";
	for(i = 0;i < 100;i++){
		fout << nTrC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTrainC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTrC2:\n";
	for(i = 0;i < 100;i++){
		fout << nTrC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTrainC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTrC3:\n";
	for(i = 0;i < 100;i++){
		fout << nTrC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTrainC3Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTeC1:\n";
	for(i = 0;i < 100;i++){
		fout << nTeC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTestC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTeC2:\n";
	for(i = 0;i < 100;i++){
		fout << nTeC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTestC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTeC3:\n";
	for(i = 0;i < 100;i++){
		fout << nTeC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << nTestC3Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTrC1:\n";
	for(i = 0;i < 100;i++){
		fout << pTrC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATrainC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTrC2:\n";
	for(i = 0;i < 100;i++){
		fout << pTrC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATrainC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTrC3:\n";
	for(i = 0;i < 100;i++){
		fout << pTrC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATrainC3Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTeC1:\n";
	for(i = 0;i < 100;i++){
		fout << pTeC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATestC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTeC2:\n";
	for(i = 0;i < 100;i++){
		fout << pTeC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATestC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "pTeC3:\n";
	for(i = 0;i < 100;i++){
		fout << pTeC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << PCATestC3Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTrC1:\n";
	for(i = 0;i < 100;i++){
		fout << fTrC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTrainC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTrC2:\n";
	for(i = 0;i < 100;i++){
		fout << fTrC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTrainC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTrC3:\n";
	for(i = 0;i < 100;i++){
		fout << fTrC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTrainC3Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTeC1:\n";
	for(i = 0;i < 100;i++){
		fout << fTeC1V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTestC1Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTeC2:\n";
	for(i = 0;i < 100;i++){
		fout << fTeC2V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTestC2Vary << "\n\n";
	
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "fTeC3:\n";
	for(i = 0;i < 100;i++){
		fout << fTeC3V[i] << "  " << priorProb(0,0) << "  " << priorProb(1,0) << "\n";
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "BEST: " << FLDTestC3Vary << "\n\n";
	
	// kNN DATA PROCESSING
	
	// kNN training cases
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "normTrain kNN: k = 33\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(33,j,nTrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "normTrain BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTrainPCA kNN: k = 33\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(33,j,PCATrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "nTrainPCA BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTrainFLD kNN: k = 19\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 200;j++){
			kCurBestClass = kNNTrain(19,j,FLDTrainDist,trainClass,priorProb);
			if(kCurBestClass == trainClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/200.0) * 100.0 << "\n";
		if((((double)kNormCor/200.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/200.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "nTrainFLD BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	
	//kNN Test cases
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "normTest kNN: k = 37\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(37,j,nTestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "normTest BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTestPCA kNN: k = 49\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(49,j,PCATestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "nTestPCA BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	
	c0Best = 0.0;
	c1Best = 0.0;
	curKBest = 0.0;
	kNormCor = 0;
	kCurBestClass = 0;
	priorProb(0,0) = 1.0;
	priorProb(1,0) = 0.0;
	fout << "nTestFLD kNN: k = 11\n";
	for(i = 1;i < 100;i++){
		for(j = 0;j < 332;j++){
			kCurBestClass = kNNTest(11,j,FLDTestDist,testClass,priorProb);
			if(kCurBestClass == testClass(i,0)){kNormCor++;}
		}
		fout << "c0: " << priorProb(0,0) << "  c1: " <<priorProb(1,0) << "  percent: " << ((double)kNormCor/332.0) * 100.0 << "\n";
		if((((double)kNormCor/332.0) * 100.0) > curKBest){
			curKBest = ((double)kNormCor/332.0) * 100.0;
			c0Best = priorProb(0,0);
			c1Best = priorProb(1,0);
		}
		kNormCor = 0;
		priorProb(0,0) -= 0.01;
		priorProb(1,0) += 0.01;
	}
	fout << "nTestFLD BEST kNN Percent: " << "c0: " << c0Best << "  c1: " << c1Best << "\n";
	fout.close();
	cout << "Dynamic Done\n";
}//END OF MAIN

int kNNTest(int k,int teIndx,Matrix test,Matrix tClass, Matrix pProb){
	int i,j = 0;
	int kIndexes[k];
	double heldDist[332];
	double hold = 0.0;
	double c0 = 0.0,c1 = 0.0;
	bool keepSorting = true;
	
	for(i = 0;i < k;i++){
		kIndexes[i] = -1;
	}
	
	for(i = 0;i < 332;i++){
		heldDist[i] = test(teIndx,i);
	}
	while(keepSorting){
		keepSorting = false;
		for(i = 0;i < 331;i++){
			if(heldDist[i] > heldDist[i + 1]){
				hold = heldDist[i];
				heldDist[i] = heldDist[i + 1];
				heldDist[i + 1] = hold;
				keepSorting = true;
			}
		}
	}
	for(i = 0;i < k;i++){
		//cout << heldDist[i] << "\n";
	}
	for(i = 0;i < k;i++){
		for(j = 0;j < 332;j++){
			if(heldDist[i] == test(teIndx,j)){kIndexes[i] = j;break;}
		}
	}
	
	for(i = 0;i < k;i++){if(tClass(kIndexes[i],0) == 0){c0++;}else{c1++;};}
	c0 = (c0/(double)k) * pProb(0,0);
	c1 = (c1/(double)k) * pProb(1,0);
	//cout << "case0: " << c0 << "   case1: " << c1 << "\n";
	if(c0 > c1){return 0;}else{return 1;}
}

int kNNTrain(int k,int teIndx,Matrix test,Matrix tClass, Matrix pProb){
	int i,j = 0;
	int kIndexes[k];
	double heldDist[200];
	double hold = 0.0;
	double c0 = 0.0,c1 = 0.0;
	bool keepSorting = true;
	
	for(i = 0;i < k;i++){
		kIndexes[i] = -1;
	}
	
	for(i = 0;i < 200;i++){
		heldDist[i] = test(teIndx,i);
	}
	while(keepSorting){
		keepSorting = false;
		for(i = 0;i < 199;i++){
			if(heldDist[i] > heldDist[i + 1]){
				hold = heldDist[i];
				heldDist[i] = heldDist[i + 1];
				heldDist[i + 1] = hold;
				keepSorting = true;
			}
		}
	}
	for(i = 0;i < k;i++){
		//cout << heldDist[i] << "\n";
	}
	for(i = 0;i < k;i++){
		for(j = 0;j < 200;j++){
			if(heldDist[i] == test(teIndx,j)){kIndexes[i] = j;break;}
		}
	}
	
	for(i = 0;i < k;i++){if(tClass(kIndexes[i],0) == 0){c0++;}else{c1++;};}
	c0 = (c0/(double)k) * pProb(0,0);
	c1 = (c1/(double)k) * pProb(1,0);
	//cout << "case0: " << c0 << "   case1: " << c1 << "\n";
	if(c0 > c1){return 0;}else{return 1;}
}