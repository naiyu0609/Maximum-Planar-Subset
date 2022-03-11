#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

//�O���� 
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

    cout << "#Chords:" << num_points << endl;//print �`�� 
    
    short int endpoint[num_points]; //�O�����I 
    short int chord_map[num_points]; //�O���Oinput���ĴX�ک�
    vector <short int> raw(num_points,0);  
    vector <vector <short int> >  M(num_points,raw); //�O�����d�򪺳̤j���ۥ��`�� 
    raw.clear();
    vector <short int> depth (0,0);
    vector <vector <short int> >  temp_list(num_points,depth);
    vector<vector<vector<short int> > >  list(num_points,temp_list); //�O�����d��̤j���ۥ檺�O���X�ک� 
    depth.clear();
    temp_list.clear();
    vector <Chords> chords; //�O����J����  
    
    
	
	for(int n = 0; n < num_points/2; n++){//�̦���J�O���� 
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
	//�Ѭ۶Z�Z���u������覡�C�C���|��0��̫᪺�覡�h�Ѱ��D 
	for(int step = 1; step < num_points; step++){
		for(int i = 0; i+step < num_points; i++){
			int k = endpoint[i];//�_�I�����I
			int j = i+step;//�d��̫᪺�I 
			if (k == j){//Case1 :�p�G�_�I�����I��n = �d��̫᪺�I
				M[i][j] = M[i+1][j-1] + 1; // �N���I��J 
				list[i][j].assign(list[i+1][j-1].begin(), list[i+1][j-1].end()); //�� i+1~j-1�d��T�w�[�J������Ji~j���d�� 
				list[i][j].insert(list[i][j].end(), chord_map[k]); // �� ij����Ji~j���d�� 
			}
			else if(k > j || k < i){//Case2 :�p�G�_�I�����I��n���b���d��
				if (M[i][j-1] >= M[i+1][j]){//�����䪺�d���̨θ� 
					M[i][j] = M[i][j-1];
					list[i][j].assign(list[i][j-1].begin(), list[i][j-1].end());
				}
				else{
					M[i][j] = M[i+1][j];
					list[i][j].assign(list[i+1][j].begin(), list[i+1][j].end());
				}
				
			}
			else if((k > i) && (k < j)){//Case3 :�p�G�_�I�����I��n�b���d��
				if (((M[i][k] + M[k][j]) >= M[i][j-1]) & ((M[i][k] + M[k][j]) >= M[i+1][j])){//�p��k�I�e�᪺�d��ۥ[���S���W�L i~j-1 �άO i+1~j  
					M[i][j] = M[i][k] + M[k][j]; //�l�ϰ�̨θѬۥ[ 
					list[i][j].assign(list[i][k].begin(), list[i][k].end());//�l�ϰ�̨θ�
					list[i][j].insert(list[i][j].end(), list[k][j].begin(), list[k][j].end());//�l�ϰ�̨θ�
					
				}
				else if ((M[i][j-1]) >= (M[i+1][j])){//�����䪺�d���̨θ� 
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
	
	cout << M[0][num_points-1] << endl;//��X�̨θѪ��ƶq 
	
	vector <int> arr(M[0][num_points-1],0);


	for(int n = 0; n < list[0][num_points-1].size(); n++){//�N�̨θѪ����d�X��ڥN���I 
		arr[n] = chords[list[0][num_points-1][n]].i;
	}
	
	sort(arr.begin(), arr.begin()+M[0][num_points-1]);//�Ƨ� 
	
	
	
	
	fstream fout;
    fout.open(argv[2],fstream::out);
    if(!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        system("pause");
	    exit(1);
    }
	fout << M[0][num_points-1] << endl;
	for (int i =0; i < M[0][num_points-1]; i++){//��X�� 
		fout << arr [i] << " " << endpoint[arr [i]]<<endl;
	}
	cout << "Finish"<<endl;
    // output something to the output file
    system("pause");
    return 0;
}
