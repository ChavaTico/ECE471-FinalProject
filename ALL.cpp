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
	Matrix Train(891,8);
	Matrix Test(418,7);
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
	float fare = 0;
	int cabNum = 0;
	int embark = 0;
	int surv = 0;
	int i =0,j = 0,k = 0,l = 0,m = 0,n = 0,p = 0;
	char nextChar;
	
	
	fin.open(teFile.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << teFile << "\n";
		exit(1);
	}
	
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
		cout << pClass << "  " << sex << "  " << sibSp << "  " << pArch << "  " << fare << "  " << cabNum << "  " << embark << "\n";
	}
	fin.close();
	
		fin.open(trFile.c_str());
	if(!fin.is_open()){
		cerr << "Error opening file: " << trFile << "\n";
		exit(1);
	}
	
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
	
	return 0;
}//END OF MAIN