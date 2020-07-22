#include <iostream> 
#include <cstring>

using namespace std ;

bool Update( int *alreadyChosen, int *minset, int num, int ans[][17], int *ansnum ) {
	// 更新已選擇過的線 
	bool end = true ;
	for ( int x = 0 ; x < 16 ; x++ ) {
		if ( alreadyChosen[x] == 0 ) {
			// 避免重複做記號 
			for ( int i = 0 ; i < ansnum[x] ; i++ ) {
				for ( int j = 0 ; j < num ; j++ )
					if ( minset[j] == ans[x][i] )
						alreadyChosen[x] = 1 ;
			}
		}
	}
	for ( int x = 0 ; x < 16 ; x ++ )
	// 是否已全部線皆確認過   
		if ( alreadyChosen[x] == 0 )
			end = false ;
	return end ;
	
}

int GetOutput( int *n, int a ) { //n1 n2 n3 n4 + 要改哪一條線 
	// SA1 or SA0 check
	if ( a != 4 )
		n[4] = !( n[2]&n[3] ) ;
	if ( a != 5 )
		n[5] = !n[1] ;
	if ( a != 6 )
		n[6] = !( n[5]|n[0] ) ;
	if ( a != 7 )
		n[7] = !( n[4]&n[6] ) ;
	return n[7] ; 
} 


int main() {
	
	bool same, end ;
	int n[9], t[9], temp, y0, y1, ans[17][17], ansnum[17] = {0}, max, maxset ;
	int j, sum, minset[10] = {0}, minnum = 0, alreadyChosen[17] = {0}, count[17] = {0} ;	
	
	// a0, a1, a2, a3, n1, n2, n3, y
	for ( int i = 0 ; i < 16 ; i++ ) {
		// i from 0000 to 1111
		temp = i ;
		// transfer int to int array as bit using
		for ( j = 0 ; j < 4 ; j++ ) {
			n[j] = temp%2 ;
			temp /= 2 ;
		}
		
		for ( int x = 0 ; x < 8 ; x++ ) { //sa0 a0 a1 a2 a3 n1 n2 n3 y 
			y0 = GetOutput( n, 0 ) ;// y0=initialize normal output  
			for ( j = 0 ; j < 9 ; j++ )
				t[j] = n[j] ; //copy to test 
			t[x] = 1 ; //測試 是不是sa1 

			y1 = GetOutput( t, x ) ; //改變后的y 
			
			if ( y0 != y1 ) {//如果不一樣代表sa1 
				sum = n[3]*8+n[2]*4+n[1]*2+n[0] ;//2進位轉10進位 
				
				// store nonrepeat int to answer buffer
					ans[x][ansnum[x]++] = sum ;		
			}
		}	
		for ( int x = 8 ; x < 16 ; x++ ) { //sa1 
			y0 = GetOutput( n, 0 ) ; //得到y值 
			for ( j = 0 ; j < 9 ; j++ )
				t[j] = n[j] ;
			t[x-8] = 0 ;//測試 是不是sa0
			
			y1 = GetOutput( t, x-8 ) ;//改變后的y 
			
			if ( y0 != y1 ) {//如果不一樣代表sa0
				sum = n[3]*8+n[2]*4+n[1]*2+n[0] ;//2進位轉10進位 
			
			// store nonrepeat int to answer buffer
					ans[x][ansnum[x]++] = sum ;
			}
		}	
	}
	
	// ( only a answer ) choose first
	for ( int x = 0 ; x < 16 ; x++ ) {
		if ( ansnum[x] == 1 ) {
			same = false ;
			for ( int i = 0 ; i < minnum ; i++ ) 
				if ( minset[i] == ans[x][0] )
					same = true ;
			if ( !same )
				minset[minnum++] = ans[x][0] ;
		}
			// store into minimum set 
	} 
	
	end = Update( alreadyChosen, minset, minnum, ans, ansnum ) ;//確認沒有重複  用already chosen做記號 
	while ( !end ) {
		// 每次都推一個尚未確認的線且統計數據重複最多的set進入minimun set
		// 再重新確認是否已達到目標並做記號(update) 
		memset( count, 0, 16 ) ;//count歸0 
		max = 0 ;
		for ( int x = 0 ; x < 16 ; x++ ) { //算哪一個組合最多 存到max 
			if ( alreadyChosen[x] == 0 ) {
				for ( int i = 0 ; i < ansnum[x] ; i++ ) {
					count[ans[x][i]]++ ;
					if ( count[ans[x][i]] >= max ) {
						max = count[ans[x][i]] ;
						maxset = ans[x][i] ;
					}
				}
			}
		}
		minset[minnum++] = maxset ; //max推入minset 
		end = Update( alreadyChosen, minset, minnum, ans, ansnum ) ;//確認沒有重複  用already chosen做記號 
	}
	cout << "The minimun set" << endl ;
	for ( int x = 0 ; x < minnum ; x++ ) { //印出minimum set 
		// output
		temp = minset[x] ;
		for ( j = 0 ; j < 4 ; j++ ) { //轉2進位 
			n[j] = temp%2 ;
			temp /= 2 ;
		}
		cout << n[3] << n[2] << n[1] << n[0] << endl ;
	}
	/*	// 輸出全部結果 0-7 means web of checking SA0 ( a0, a1, a2, a3, n1, n2, n3, y ) 
						8-15 means web of checking SA1 ( a0, a1, a2, a3, n1, n2, n3, y ) 
	for ( int x = 0 ; x < 16 ; x++ ) {
		if ( x == 0 )
			cout << "SA1" << endl ;
		if ( x == 8 )
			cout << "SA0" << endl ;
		cout << x << endl ;
		for ( int i = 0 ; i < ansnum[x] ; i++ ) {
			temp = ans[x][i] ;
			for ( j = 0 ; j < 4 ; j++ ) {
				n[j] = temp%2 ;
				temp /= 2 ;
			}
			cout << n[3] << n[2] << n[1] << n[0] << " " ;
		}
		cout << endl ;
	}
		//cout << ans[i] << endl ;
	*/

}
