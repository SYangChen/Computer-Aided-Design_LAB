#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define MAX 200

using namespace std ;
fstream fin ;

struct alu {
	
	int mul ;		// mul resource constaint / mul operating time
	int add ;		// add					  / add
	 
};

struct dfg {
	
	int op ;				// add 1 or mutiply 2 none -1
	int input1 ;			// first input
	int input2 ;			// second input
	int result ;			// output
	int pathPriority ;		// longestpath
	bool last ;				// last operator
	bool ready ;			// ready to operate
	bool pushtoreadylist ;	// already push to ready list
	
};

struct process {
	
	int resultNum ;			// process operator num
	int count ;				// time counter
	
};

bool compare( dfg a, dfg b ) {
	// greater
	return a.pathPriority > b.pathPriority ;
	
} // compare()

bool LoadIn( dfg *buffer, int &cnt ) {
	
	int num ;
	string name ;
	dfg temp ;
	cout << "Please enter input file name ( except\".txt\" ) : " ;
	cin >> name ;
	name += ".txt" ;
	fin.open( name.c_str(), ios::in ) ;
	if ( !fin )
		return false ;
	fin >> num ;
	cnt = num ;
	while ( num-- ) {
		// Read all data to buffer
		fin >> temp.op >> temp.input1 >> temp.input2 >> temp.result ;
		temp.ready = false ;
		temp.pushtoreadylist = false ;
		temp.pathPriority = buffer[temp.result].pathPriority ;
		temp.last = buffer[temp.result].last ;
		buffer[temp.result] = temp ;
		buffer[temp.input1].last = false ;
		buffer[temp.input2].last = false ;
	} // end while
	fin.close() ;
	return true ;
	
} // LoadIn()

void CheckResource( vector<alu> &resource, alu &time ) {
	// set add/mul operator time & add/mul resource constraint
	alu temp ;
	cout << "Please input the ADD operator time : " ;
	cin >> time.add ;
	cout << "Please input the MUL operator time : " ;
	cin >> time.mul ;
	cout << "Please input all resource constraint you want to test ( -1 to end ) : " << endl ;
	cout << "MUL resource / ADD resource ( with white space ): " ;
	cin >> temp.mul ;
	while ( temp.mul != -1 ) {
		cin >> temp.add ;
		resource.push_back( temp ) ;
		cout << "MUL resource / ADD resource ( with white space ): " ;
		cin >> temp.mul ;
	} // end while
	
} // CheckResource()

void CalLength( dfg *buffer, alu time, int result ) {
	// a Recursion function to calculate the longest path with every operator
	int addmul[] = { 0, time.add, time.mul } ;
	if ( buffer[buffer[result].input1].ready && buffer[buffer[result].input2].ready )
		return ;
	if ( !buffer[buffer[result].input1].ready &&\
		 buffer[result].pathPriority+addmul[buffer[buffer[result].input1].op]\
		 > buffer[buffer[result].input1].pathPriority ) {
		buffer[buffer[result].input1].pathPriority = buffer[result].pathPriority+addmul[buffer[buffer[result].input1].op];
		CalLength( buffer, time, buffer[result].input1 ) ;
	} // end if
	if ( !buffer[buffer[result].input2].ready &&\
		 buffer[result].pathPriority+addmul[buffer[buffer[result].input2].op]\
		 > buffer[buffer[result].input2].pathPriority ) {
		buffer[buffer[result].input2].pathPriority = buffer[result].pathPriority+addmul[buffer[buffer[result].input2].op];
		CalLength( buffer, time, buffer[result].input2 ) ;
	} // end if
	
} // CalLength()

void LongestPathCal( dfg *buffer, alu time ) {
	// calculate LongestPath driver
	for ( int i = 0 ; i < MAX ; i++ ) {
		if ( buffer[i].op != -1 && buffer[i].last ) {
			buffer[i].pathPriority = ( buffer[i].op-1 ) ? time.mul : time.add ;
			CalLength( buffer, time, buffer[i].result ) ;
		} // end if
	} // end for
	
} // LongestPathCal()

int ListScheduling( dfg *buffer, int cnt, alu resCons, alu time ) {
	// list scheduling with longest path piority
	int step ;				// step count
	vector<dfg> addReady ;	// add operator ready piority queue
	vector<dfg> mulReady ;	// mul operator ready piority queue
	process *addcntr = new process[resCons.add] ;	// add resource time counter
	process *mulcntr = new process[resCons.mul] ;	// mul resource time counter
	for ( int i = 0 ; i < resCons.add ; i++ )
		addcntr[i].count = 0 ;						// initiate/unuse, set count to 0
	for ( int i = 0 ; i < resCons.mul ; i++ )
		mulcntr[i].count = 0 ;						// initiate/unuse, set count to 0
		
	for( step = 0 ; cnt != 0 || addReady.size() != 0 || mulReady.size() != 0 ; step++ ) {
		// step count with resource constraint and piority
		for ( int i = 0 ; i < MAX ; i++ ) {
			// for loop to push the ready operator into ready list
			if ( !buffer[i].ready && !buffer[i].pushtoreadylist &&\
				 buffer[buffer[i].input1].ready && buffer[buffer[i].input2].ready ) {
				buffer[i].pushtoreadylist = true ;
				if ( buffer[i].op == 1 ) 
					addReady.push_back( buffer[i] ) ;
				else 
					mulReady.push_back( buffer[i] ) ;
				cnt-- ;
			}
		} // end for
		
		// sort the ready list with priority
		if ( addReady.size() > 1 )
			sort( &addReady[0], &addReady[0]+addReady.size(), compare ) ;
		if ( mulReady.size() > 1 )
			sort( &mulReady[0], &mulReady[0]+mulReady.size(), compare ) ;
		// sort the ready list with priority
		
		for ( int i = 0 ; i < resCons.add ; i++ ) {
			// check if there is a resource is in idle( counter = 0 ) ADD
			if ( addcntr[i].count == 0 && addReady.size() != 0 ) {
				// start to operate
				addcntr[i].resultNum = addReady[0].result ;
				addcntr[i].count = time.add ;
				addReady.erase( addReady.begin() ) ;
				// remove from ready list
			}
		} // end for
		for ( int i = 0 ; i < resCons.mul ; i++ ) {
			// check if there is a resource is in idle( counter = 0 ) MUL
			if ( mulcntr[i].count == 0 && mulReady.size() != 0 ) {
				// start to operate
				mulcntr[i].resultNum = mulReady[0].result ;
				mulcntr[i].count = time.mul ;
				mulReady.erase( mulReady.begin() ) ;
				// remove from ready list
			}
		} // end for
		for ( int i = 0 ; i < resCons.add ; i++ ) {
			// time count with operator time ADD
			if ( addcntr[i].count != 0 ) {
				addcntr[i].count-- ;
				if ( addcntr[i].count == 0 )
					// operator done, set this operator output to ready
					buffer[addcntr[i].resultNum].ready = true ;
			} // end if
		} // end for
		for ( int i = 0 ; i < resCons.mul ; i++ ) {
			// time count with operator time MUL
			if ( mulcntr[i].count != 0 ) {
				mulcntr[i].count-- ;
				if ( mulcntr[i].count == 0 )
					// operator done, set this operator output to ready
					buffer[mulcntr[i].resultNum].ready = true ;
			} // end if
		} // end for
	} // end for
	return step ;
	
} // ListScheduling()

int main() {
	
	int cntOP, step ;
	alu time ;
	dfg *buffer = new dfg[MAX] ;
	vector<alu> resource ;
	for ( int i = 0 ; i < MAX ; i++ ) {
		buffer[i].op = -1 ;
		buffer[i].pathPriority = 0 ;
		buffer[i].ready = true ;
		buffer[i].last = true ;
	} // end for
	if ( !LoadIn( buffer, cntOP ) ) {
		cout << "unknown file" << endl ; 
		return 0 ;
	} // end if
	CheckResource( resource, time ) ;
	LongestPathCal( buffer, time ) ;
	/*
	for ( int i = 0 ; i < MAX ; i++ ) {
		cout << buffer[i].op << " " << buffer[i].input1 << " " << buffer[i].input2 << " " ;
		cout << buffer[i].result << " Path:" << buffer[i].pathPriority << " " ;
		cout << buffer[i].ready << " last :" << buffer[i].last << endl ;
	} // end for
	*/
	for ( int i = 0 ; i < resource.size() ; i++ ) {
		step = ListScheduling( buffer, cntOP, resource[i], time ) ;
		cout << "MUL, ADD ( " << resource[i].mul << ", " << resource[i].add << " ) : " ;
		cout << step << " steps ( counter start from step 1 )" << endl ;
		for( int i = 0 ; i < MAX ; i++ ) {
			// flag initialization
			if ( buffer[i].op != -1 ) {
				buffer[i].ready = false ;
				buffer[i].pushtoreadylist = false ;
			} // end if
		} // end for
	} // end for
	
	system( "pause" ) ;
	return 0 ;
} // main()
