#include <fstream>
#include <iostream>
#include <string>
#define LevelDir "../levels/"
#define file "level"

using namespace std;

#define Mapdir "./Map"

int main(){
    int mapHeight;
    int mapWidth;

    int **map;

    string mapPath;

    mapPath = "./Map/level0.txt";


    
    ifstream in(mapPath);
    in >> mapHeight >> mapWidth;
    map = new int *[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new int [mapWidth];
        for (int j = 0; j < mapWidth; j++) {
            in >> map[i][j];
        }
    }
    
    cout << " 출력 : " << endl;

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }

    cout << " 배열 위치 확인" << endl;

    cout << "map[0][0] : " << map[0][0] << endl;

    cout << "map[5][5] : " << map[5][5] << endl;
}