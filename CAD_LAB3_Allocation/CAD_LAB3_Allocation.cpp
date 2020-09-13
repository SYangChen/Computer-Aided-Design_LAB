#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <algorithm>

using namespace std ;
fstream fin ;

struct LifeTime {
	
	string name ;
	int startTime ;
	int endTime ;
	bool done ;
	
};

struct Register {
	
	vector<string> regStream ;
	int lastTime ; 
	
};

bool Compare( LifeTime a, LifeTime b ) {
	
	return a.startTime < b.startTime ;
	
}

void Read( vector<LifeTime> &lifeTimeVec ) {
	// Read file data to buffer
	int num ;
	LifeTime tempLT ;
	fin >> num ;
	for ( int i = 0 ; i < num ; i++ ) {
		fin >> tempLT.name >> tempLT.startTime >> tempLT.endTime ;
		tempLT.done = false ;
		lifeTimeVec.push_back( tempLT ) ;
	}
	
}

bool LoadIn( string file, vector<LifeTime> &lifeTimeVec ) {
	
	fin.open( file.c_str(), ios::in ) ;
	if ( !fin )
		return false ;
	Read( lifeTimeVec ) ;
	fin.close() ;
	return true ;
	
}

int LeftEdgeAllocation( vector<LifeTime> &lifeTimeVec, vector<Register> &regVec ) {
	// move the time to the left most register whether there has no conflict between their life time
	Register curReg ;
	int cnt = lifeTimeVec.size(), regCnt ;
	for ( regCnt = 0 ; cnt > 0 ; regCnt++ ) {
		curReg.lastTime = 0 ;
		curReg.regStream.clear() ;
		for ( int i = 0 ; i < lifeTimeVec.size() ; i++ ) {
			if ( !lifeTimeVec[i].done && lifeTimeVec[i].startTime >= curReg.lastTime ) {
				curReg.regStream.push_back( lifeTimeVec[i].name ) ;
				lifeTimeVec[i].done = true ;
				curReg.lastTime = lifeTimeVec[i].endTime ;
				cnt-- ;
			}
		}
		regVec.push_back( curReg ) ;
	} // end while
	return regCnt ;
	
} 

int main() {
	
	int numRegUse ;
	string file ;
	vector<LifeTime> lifeTimeVec ;
	vector<Register> regVec ;
	cout << "Please enter the file name : " ;
	cin >> file ;
	if ( !LoadIn( file, lifeTimeVec ) )
		cout << "error file" << endl ;
	else {
		sort( &lifeTimeVec[0], &lifeTimeVec[0]+lifeTimeVec.size(), Compare ) ;
		numRegUse = LeftEdgeAllocation( lifeTimeVec, regVec ) ;
		cout << "The minimun register used by left edge allocation : " ;
		cout << numRegUse << endl ;
		for ( int i = 0 ; i < numRegUse ; i++ ) {
			cout << "Reg#" << i << " : " ;
			for ( int j = 0 ; j < regVec[i].regStream.size() ; j++ )
				cout << regVec[i].regStream[j] << "\t" ;
			cout << endl ;
		} // end for
	}
	system( "pause" ) ;
	
} 
