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

int main(int argc, char** argv){
	Matrix Train(891,8); Matrix nTrain(891,8);
	Matrix Test(418,7); Matrix nTest(418,7);
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
	int cabNum = 0;
	int embark = 0;
	int surv = 0;
	int i =0,j = 0,k = 0,l = 0,m = 0,n = 0,p = 0;
	float nSum = 0.0;
	char nextChar;
	double fMean[7];
	double fVariance[7];
	
	for(i = 0;i < 7;i++){
		fMean[i] = 0.0;
		fVariance[i] = 0.0;
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
		Train(i,0) = pClass;Train(i,1) = sex;Train(i,2) = sibSp;Train(i,3) = pArch;Train(i,4) = fare;Train(i,5) = cabNum;Train(i,6) = embark;
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
	
	// Determine feature means
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){fMean[i] += Train(j,i);}
		fMean[i] /= 891;
	}
	
	// Determine feature variances
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){fVariance[i] += ((Train(j,i) - fMean[i]) * (Train(j,i) - fMean[i]));}
		fVariance[i] /= (891-1);
	}
	
	// Normalize training
	for(i = 1;i < 8;i++){
		for(j = 0;j < 891;j++){nTrain(j,i) = ((Train(j,i) - fMean[i])/fVariance[i]);}
	}
	
	// Normalize testing
	for(i = 0;i < 7;i++){
		for(j = 0;j < 418;j++){nTest(j,i) = ((Test(j,i) - fMean[i])/fVariance[i]);}
	}
	
	// Add classes to nTrain
	for(i = 0;i < 891;i++){
		nTrain(i,0) = Train(i,0);
	}
	
	for(i = 0;i < 891;i++){
		for(j = 0;j < 8;j++){
			cout << nTrain(i,j) << " ";
		}
		cout << "\n";
	}
	
	return 0;
}//END OF MAIN