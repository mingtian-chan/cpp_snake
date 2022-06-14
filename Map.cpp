#include <fstream>
#include "Map.h"
#include <string>
using namespace std;

#define LevelDir "./Map/"
#define file "level"

Map::Map() {}

Map::Map(int stage) {
    mapPath = (string)LevelDir + file + to_string(stage) + ".txt";
    loadMap();
}

void Map::loadMap() {
    ifstream in(mapPath);
    in >> mapHeight >> mapWidth;
    map = new int *[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new int [mapWidth];
        for (int j = 0; j < mapWidth; j++) {
            in >> map[i][j];
        }
    }
}

int Map::getMapValue(int y, int x) {
    return map[y][x];
}

void Map::setMapValue(int y, int x, int value) {
    map[y][x] = value;
}

vector<int> Map::getMapPos(int value) {
    vector<int> v;
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (map[i][j] == value) {
                v.push_back(i);
                v.push_back(j);
            }
        }
    }
    return v;
}

void Map::setSnakeZero() {
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0;j < mapWidth; j++) {
            if (map[i][j] == 3) {
                map[i][j] = 0;
            }
            if (map[i][j] == 4) {
                map[i][j] = 0;
            }
        }
    }
}