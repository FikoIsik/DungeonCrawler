#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string, std::ofstream;

/**
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream file(fileName);
    if (!file.is_open()) {
    return nullptr;
    }
    file >> maxRow >> maxCol;
    file >> player.row >> player.col;
    if (maxRow > 999999 || maxCol > 999999) {
        return nullptr;
    }
    if (maxRow <= 0 || maxCol <= 0) {
        return nullptr; 
    }
    if (player.row < 0 || player.col < 0 || player.row >= maxRow || player.col >= maxCol) {
        return nullptr;
    }
    if (maxRow > INT32_MAX/maxCol) {
        return nullptr;
    }
    
    char** map = createMap(maxRow, maxCol);
    if (map == nullptr) { 
        return nullptr;
    }

    bool d = false, e = false;
    for (int row = 0; row < maxRow; ++row) {
        for (int col = 0; col < maxCol; ++col) {
             if  (!(file >> map[row][col])) {
                   deleteMap(map, maxRow); 
                    return nullptr;
                }
                
                if (map[row][col] != TILE_OPEN && map[row][col] != TILE_PILLAR && map[row][col] != TILE_MONSTER &&
                map[row][col] != TILE_TREASURE && map[row][col] != TILE_EXIT && map[row][col] != TILE_DOOR &&
                map[row][col] != TILE_AMULET) {
                deleteMap(map, maxRow);
                return nullptr;
            }
            if (map[row][col] == TILE_DOOR){
            d = true;
            }
            if (map[row][col] == TILE_EXIT){
                e = true;
            } 
        }
    }

if (!d && !e) {
        deleteMap(map, maxRow);
        return nullptr;
    }

    if (map[player.row][player.col] == TILE_OPEN) {
    map[player.row][player.col] = TILE_PLAYER;
    }
    else {
        deleteMap(map, maxRow);
        return nullptr;
    }
    char extra;
    if (file >> extra) {  
        deleteMap(map, maxRow);  
        return nullptr;
    }


    file.close();

    return map; 
}

/**
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if (input == MOVE_UP) {
        nextRow--;
    }
    if (input == MOVE_DOWN) {
        nextRow++;
    }
    if (input == MOVE_LEFT) {
        nextCol--;
    }
    if (input == MOVE_RIGHT) {
        nextCol++;
    }
}

/**
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if (maxRow <= 0 || maxCol <= 0) {
        return nullptr;  
    }
    char** map = new char*[maxRow];
    for (int i = 0; i < maxRow; i++) {
        map[i] = new char[maxCol];
    }
    for (int row = 0; row < maxRow; row++) {
        for (int col = 0; col < maxCol; col++) {
            map[row][col] = TILE_OPEN;
        }
    }
return map;
}

/**
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if (map == nullptr) {
        maxRow = 0;
    }
    else {
    for (int i = 0; i < maxRow; i++) {
        delete[] map[i];
    }
    delete[] map;
    map = nullptr;
    maxRow = 0;
    }
}

/**
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if (map == nullptr) {
        return nullptr;
    }
    if (maxCol <= 0 || maxRow <= 0 || maxCol*2 > 999999 || maxRow*2  > 999999) {
        return nullptr;
    }
    if (2*maxRow > INT32_MAX/(2*maxCol)) {
        return nullptr;
    }




    char** newMap = new char*[maxRow*2];
    for (int i = 0; i < maxRow*2; i++) {
        newMap[i] = new char[maxCol*2];
    }


    //A Quadrant
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            newMap[i][j] = map[i][j];
        }
    }

    //B Quadrant
    for (int i = 0; i < maxRow; i++) {
        for (int j = maxCol; j < maxCol*2; j++) {
            if (map[i][j-maxCol] != 'o') {
            newMap[i][j] = map[i][j-maxCol];
            }
            else {
                newMap[i][j] = TILE_OPEN;
            }
        }
    }
    //C Quadrant
    for (int i = maxRow; i < maxRow*2; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (map[i-maxRow][j] != 'o') {
                newMap[i][j] = map[i-maxRow][j];
            }
            else {
                newMap[i][j] = TILE_OPEN;
            }
        }
    }
    //D Quadrant
    for (int i = maxRow; i < maxRow* 2; i++) {
        for (int j = maxCol; j < maxCol*2; j++) {
            if (map[i-maxRow][j-maxCol] != 'o') {
                newMap[i][j] = map[i-maxRow][j-maxCol];
            }
            else {
                newMap[i][j] = TILE_OPEN;
            }
        }
    }

    for (int i = 0; i < maxRow; i++) {
        delete[] map[i];
    }
    delete[] map;
    maxRow *= 2;
    maxCol *= 2;
    return newMap;
}


/**
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if (!(nextRow >= 0 && nextRow < maxRow) || !(nextCol >= 0 && nextCol < maxCol)) {
        nextRow = player.row;
        nextCol = player.col;
        return 0;
    }

    if (map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER) {
        nextRow = player.row;
        nextCol = player.col;
        return 0;
    }
    if (map[nextRow][nextCol] == TILE_OPEN) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return 1;
    }
    if (map[nextRow][nextCol] == TILE_TREASURE) {
        player.treasure += 1;
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return 2;
    }
    if (map[nextRow][nextCol] == TILE_AMULET) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return 3;
    }
    if (map[nextRow][nextCol] == TILE_DOOR) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return 4;
    }
    if (map[nextRow][nextCol] == TILE_EXIT) {
        if (player.treasure >= 1) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return 5;
        }
        else {
        nextRow = player.row;
        nextCol = player.col;
        return 0;
        }
    }
    return STATUS_STAY;

    

}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    //down
    int count = 0;
    for (int i = player.row+1; i < maxRow; i++) {
        if (map[i][player.col] == TILE_OPEN) {
            continue;
        }
        if (map[i][player.col] == TILE_PILLAR) {
            break;
        }
        if (map[i][player.col] == TILE_MONSTER) {
            if (map[i-1][player.col] == map[player.row][player.col]) {
                map[i-1][player.col] = TILE_MONSTER;
                map[i][player.col] = TILE_OPEN;
                count +=1;
            }
            map[i-1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
    }

    //up
    for (int i = player.row-1; i >= 0; i--) {
        if (map[i][player.col] == TILE_OPEN) {
            continue;
        }
        if (map[i][player.col] == TILE_PILLAR) {
            break;
        }
        if (map[i][player.col] == TILE_MONSTER) {
            if (map[i+1][player.col] == map[player.row][player.col]) {
                map[i+1][player.col] = TILE_MONSTER;
                map[i][player.col] = TILE_OPEN;
                count += 1;
            }
            map[i+1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
    }
    //right
    for (int i = player.col+1; i < maxCol; i++) {
        if (map[player.row][i] == TILE_OPEN) {
            continue;
        } 
        if (map[player.row][i] == TILE_PILLAR) {
            break;
        }
        if (map[player.row][i] == TILE_MONSTER) {
            if (map[player.row][i-1] == map[player.row][player.col]) {
                map[player.row][i] = TILE_OPEN;
                map[player.row][i-1] = TILE_MONSTER;
                count += 1;
            }
            map[player.row][i] = TILE_OPEN;
            map[player.row][i-1] = TILE_MONSTER;
        }
    }

    //left
    for (int i = player.col-1; i >= 0; i--) {
        if (map[player.row][i] == TILE_OPEN) {
            continue;
        } 
        if (map[player.row][i] == TILE_PILLAR) {
            break;
        }
        if (map[player.row][i] == TILE_MONSTER) {
            if (map[player.row][i+1] == map[player.row][player.col]) {
                map[player.row][i] = TILE_OPEN;
                map[player.row][i+1] = TILE_MONSTER;
                count +=1;
            }
            map[player.row][i] = TILE_OPEN;
            map[player.row][i+1] = TILE_MONSTER;
        }
    }
    if (count > 0) {
        return true;
    }
    


    return false;
}
