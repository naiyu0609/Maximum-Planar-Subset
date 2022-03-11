#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

//記錄弦 
class Chords{
    public:
        int i;
        int j;  
};

Chords temp;

int main(int argc, char **argv){
	if(argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        system("pause");
        exit(1);
    }

    // open the input file
    fstream fin;
    fin.open(argv[1], fstream::in);
    if(!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        system("pause");
        exit(1);
    }

    // parse the input file
    cout << "======================" << endl;
    cout << "        Parser        " << endl;
    cout << "======================" << endl;

    char buffer[100];

    fin >> buffer;
    int num_points = atoi(buffer);

    cout << "#Chords:" << num_points << endl;//print 總數 
    
    short int endpoint[num_points]; //記錄端點 
    short int chord_map[num_points]; //記錄是input的第幾根弦
    vector <short int> raw(num_points,0);  
    vector <vector <short int> >  M(num_points,raw); //記錄此範圍的最大不相交總數 
    raw.clear();
    vector <short int> depth (0,0);
    vector <vector <short int> >  temp_list(num_points,depth);
    vector<vector<vector<short int> > >  list(num_points,temp_list); //記錄此範圍最大不相交的是哪幾根弦 
    depth.clear();
    temp_list.clear();
    vector <Chords> chords; //記錄輸入的弦  
    
    
	
	for(int n = 0; n < num_points/2; n++){//依次輸入記錄弦 
		fin >> buffer;
		temp.i = atoi(buffer);
		fin >> buffer;
		temp.j = atoi(buffer);
		endpoint[temp.i] = temp.j;
		endpoint[temp.j] = temp.i;
		chord_map[temp.i] = n;
		chord_map[temp.j] = n;
		chords.push_back(temp);
	}
	
	
	/* DP */
	//由相距距離短到長的方式慢慢堆疊到0到最後的方式去解問題 
	for(int step = 1; step < num_points; step++){
		for(int i = 0; i+step < num_points; i++){
			int k = endpoint[i];//起點的端點
			int j = i+step;//範圍最後的點 
			if (k == j){//Case1 :如果起點的端點剛好 = 範圍最後的點
				M[i][j] = M[i+1][j-1] + 1; // 將此點算入 
				list[i][j].assign(list[i+1][j-1].begin(), list[i+1][j-1].end()); //把 i+1~j-1範圍確定加入的弦算入i~j的範圍 
				list[i][j].insert(list[i][j].end(), chord_map[k]); // 把 ij弦算入i~j的範圍 
			}
			else if(k > j || k < i){//Case2 :如果起點的端點剛好不在此範圍
				if (M[i][j-1] >= M[i+1][j]){//取哪邊的範圍當最佳解 
					M[i][j] = M[i][j-1];
					list[i][j].assign(list[i][j-1].begin(), list[i][j-1].end());
				}
				else{
					M[i][j] = M[i+1][j];
					list[i][j].assign(list[i+1][j].begin(), list[i+1][j].end());
				}
				
			}
			else if((k > i) && (k < j)){//Case3 :如果起點的端點剛好在此範圍
				if (((M[i][k] + M[k][j]) >= M[i][j-1]) & ((M[i][k] + M[k][j]) >= M[i+1][j])){//計算k點前後的範圍相加有沒有超過 i~j-1 或是 i+1~j  
					M[i][j] = M[i][k] + M[k][j]; //子區域最佳解相加 
					list[i][j].assign(list[i][k].begin(), list[i][k].end());//子區域最佳解
					list[i][j].insert(list[i][j].end(), list[k][j].begin(), list[k][j].end());//子區域最佳解
					
				}
				else if ((M[i][j-1]) >= (M[i+1][j])){//取哪邊的範圍當最佳解 
					M[i][j] = M[i][j-1];
				    list[i][j].assign(list[i][j-1].begin(), list[i][j-1].end());
				}
				else{
					M[i][j] = M[i+1][j];
				    list[i][j].assign(list[i+1][j].begin(), list[i+1][j].end());
				}
				
			}
			
		}
		
	}
	
	cout << M[0][num_points-1] << endl;//輸出最佳解的數量 
	
	vector <int> arr(M[0][num_points-1],0);


	for(int n = 0; n < list[0][num_points-1].size(); n++){//將最佳解的弦查出實際代表的點 
		arr[n] = chords[list[0][num_points-1][n]].i;
	}
	
	sort(arr.begin(), arr.begin()+M[0][num_points-1]);//排序 
	
	
	
	
	fstream fout;
    fout.open(argv[2],fstream::out);
    if(!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        system("pause");
	    exit(1);
    }
	fout << M[0][num_points-1] << endl;
	for (int i =0; i < M[0][num_points-1]; i++){//輸出弦 
		fout << arr [i] << " " << endpoint[arr [i]]<<endl;
	}
	cout << "Finish"<<endl;
    // output something to the output file
    system("pause");
    return 0;
}
