/*
ID: tictoci1
LANG: C++
TASK: castle
*/


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> findAdjacent(int i, int j, int col, int row, char direction){
    vector<int> returnList;
    if(direction=='E'){
        if(j!=col-1){
            returnList.push_back(i);
            returnList.push_back(j+1);
            return returnList;
        }
    }
    else if(direction=='N'){
        if(i!=0){
            returnList.push_back(i-1);
            returnList.push_back(j);
            return returnList;
        }
    }
    else if(direction=='S'){
        if(i!=row-1){
            returnList.push_back(i+1);
            returnList.push_back(j);
            return returnList;
        }
    }
    else if(direction=='W'){
        if(j!=0){
            returnList.push_back(i);
            returnList.push_back(j-1);
            return returnList;
        }
    }
    return returnList;
}

int findSize(vector<vector<vector<vector<int> > > >& map, vector<vector<int> >& rooms, vector<vector<bool> >& checked, int i, int j, int room){
    int count = 0;
    int size = map[i][j].size();
    //vector<vector<int> >* path1 = &map[i][j];
    //vector<vector<int> >& path = *path1;
    if(size==0 || (size<2 && checked[map.at(i).at(j).at(0).at(0)][map.at(i).at(j).at(0).at(1)])){
        return 1;
    }
    else{
        for(int k = 0; k < size; k++){
            int y = map.at(i).at(j).at(k).at(0);//[k][0];
            int x = map.at(i).at(j).at(k).at(1);//path[k][1];
            if(checked[y][x]){
                continue;
            }
            else{
                checked[y][x] = true;
                rooms[y][x] = room;
                count += findSize(map, rooms, checked, y, x, room);
            }
        }
        return count+1;
    }
}

bool sortcol( const vector<int>& v1, 
               const vector<int>& v2 ) { 
 return v1[1] < v2[1]; 
} 

int main(){
    vector<string> key;
    key.push_back("NESW");
    key.push_back("NES");
    key.push_back("WES");
    key.push_back("ES");
    key.push_back("WNS");
    key.push_back("NS");
    key.push_back("WS");
    key.push_back("S");
    key.push_back("WNE");
    key.push_back("NE");
    key.push_back("EW");
    key.push_back("E");
    key.push_back("WN");
    key.push_back("N");
    key.push_back("W");
    key.push_back(" ");
    ifstream input_file;
    input_file.open("castle.in");
    int col;
    int row;
    int num;
    input_file >> col;
    input_file >> row;
    vector<vector<vector<vector<int> > > > paths(row,vector<vector<vector<int> > >(col));
    vector<vector<vector<vector<int> > > > walls(row,vector<vector<vector<int> > >(col));
    vector<vector<int> >rooms(row, vector<int>(col,0));
    vector<vector<bool> > checked(row, vector<bool>(col,false));
    for(int i = 0; i < row;i++){
        
        for(int j = 0; j < col; j++){
            input_file >> num;
            //string directions = key[num];
            //string wallDirs = key[15-num];
            
            for(char &c: key[num]){
                vector<int> coord = findAdjacent(i,j,col,row,c);
                if(coord.size()>0){
                    paths[i][j].push_back(coord);
                }
            }
            for(char &c: key[15-num]){
                vector<int> coord = findAdjacent(i,j,col,row,c);
                if(coord.size()>0){
                    walls[i][j].push_back(coord);
                }
            }
        }
    }
    vector<int> sizes;
    int room = 0;
    int size = 0;
    int maxSize = 0;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            /*vector<vector<int> > nums = paths[i][j];
            for(int k = 0; k< nums.size(); k++){
                cout << "space "<<nums[k][0] << " " << nums[k][1] << " ";
            }
            cout << endl;
            nums = walls[i][j];
            for(int k = 0; k< nums.size(); k++){
                cout << "wall "<< nums[k][0] << " " << nums[k][1] << " ";
            }*/
            if(!checked[i][j]){
                checked[i][j] = true;
                rooms[i][j] = room;
                size = findSize(paths, rooms, checked, i,j,room);
                sizes.push_back(size);
                if(size > maxSize){
                    maxSize = size;
                }
                room++;
            }
        }
    }
    
    int numRooms = sizes.size();
    /*
    for(int i = 0; i<numRooms; i++){
        if(sizes[i] > maxSize){
            maxSize = sizes[i];
        }
    }
    */
    int newMaxSize = 0;
    vector<int> wallBetween;
    for(int j = 0; j < col; j++){
        for(int i = row-1; i >= 0; i--){
            if(walls[i][j].size()>0){
                //vector<vector<int> > adjacents = walls[i][j];
                //sort(adjacents.begin(),adjacents.end(),sortcol);
                sort(walls[i][j].begin(),walls[i][j].end(),sortcol);
                for(int k = 0; k<walls[i][j].size();k++){
                    //vector<int> adj = adjacents[k];
                    //int curRoom = rooms[i][j];
                    //int newi = walls.at(i).at(j).at(k).at(0);//adj[0];
                    //int newj = walls.at(i).at(j).at(k).at(1);//adj[1];
                    //int otherRoom = rooms[newi][newj];
                    if(rooms[i][j] != rooms[walls.at(i).at(j).at(k).at(0)][walls.at(i).at(j).at(k).at(1)]){
                        int newSize = sizes[rooms[i][j]] + sizes[rooms[walls.at(i).at(j).at(k).at(0)][walls.at(i).at(j).at(k).at(1)]];
                        if(newSize > newMaxSize){
                            wallBetween.clear();
                            newMaxSize = newSize;
                            wallBetween.push_back(i+1);
                            wallBetween.push_back(j+1);
                            if(walls.at(i).at(j).at(k).at(0)<i){
                                wallBetween.push_back(13);
                            }
                            else{
                                wallBetween.push_back(11);
                            }
                        }
                    }
                }
            }
        }
    }
    ofstream output_file;
    output_file.open("castle.out");
    output_file << numRooms <<endl;
    output_file << maxSize << endl;
    output_file << newMaxSize << endl;
    output_file << wallBetween[0] << " " << wallBetween[1] << " "<< key[wallBetween[2]]<<endl;
}