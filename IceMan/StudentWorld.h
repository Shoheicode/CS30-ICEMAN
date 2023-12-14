#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <stdlib.h>
#include <vector>
#include <cmath>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }

    ~StudentWorld() {
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                delete iceMap.at(i).at(j); //deletes every ice in vector;
            }
        }
        iceMap.clear();
        //Delete all the actors in character list
        for (Actor* a : characterList) {
            Actor* temp = a;
            a = nullptr;
            delete temp;
        }
        characterList.clear();

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                delete leavingPath[i][j]; //deletes every ice in vector;
            }
        }

    }

    /*
    init() method must:

    A. Initialize the data structures used to keep track of your game’s virtual world
    
    B. Construct a new oil field that meets the requirements stated in the section below
        (filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
    
    C.Allocate and insert a valid Iceman object into the game world at the proper
        location
    */

    virtual int init()
    {
        //Number of Boulders
        int bNum = min(static_cast<int>(getLevel())/ 2 + 2, 9);

        //Number of Gold Nuggets
        int gNum = max(5 - static_cast<int>(getLevel()) / 2, 2);

        //Number of Barrels of Oil
        oilLeft = min(static_cast<int>(getLevel()) + 2, 21);
        
        //Number of Sonar Kits and water pools
        int sWNum = max(100, 300 - (10 * static_cast<int>(getLevel())));
                
        int probOfSonarOrWater = static_cast<int>(getLevel()) * 25 + 300;

        ticksToWaitBetweenMoves = max(0, 3 - static_cast<int>(getLevel() / 4));

        //int diceShuff = rand() % 101;
        tickBetween = max(25, 200 - static_cast<int>(getLevel()));
        currentNum = 0;
        pNum = min(15, static_cast<int>(2 + getLevel() * 1.5));
        cout << "NUMBER OF PROSTERS:" << pNum << endl;
        proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
        probabilityOfHardcore = min(90, static_cast<int>(getLevel()) * 10 + 30);
        
        int proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
        
        int proTickWait = max(0, 3 - (static_cast<int>(getLevel())/4));
        ticksSonarWater = max(100, (300 - 10) * static_cast<int>(getLevel()));
        
        srand(time(NULL));

        //Creates the player
        player = new IceMan(30, 60, this);
        characterList.push_back(player);

        //Create Ice
        createIceMap();

        //Spawns Boulders
        spawnBoulders(bNum);
        
        //Spawns Nuggets
        spawnNuggets(gNum);
        
        //Spawns SonarKit
        //spawnSonar(sNum);
        //spawnWater(wNum);
        
        numberOfScanners = player->getSonarCount();
        
        //Spawns Oil
        spawnOil(oilLeft);

        //Adds a protestor
        //spawnProtesters(pNum);
        //characterList.push_back(new Protester(60, 60, IID_PROTESTER, this));
        characterList.push_back(new Protester(60, 60, IID_PROTESTER, proTickStun, ticksToWaitBetweenMoves, this));
        currentNum++;
        //characterList.push_back(new HardcoreProtester(60, 60, proTickStun, this));
       

        findPath(60,60,0,60);
       
       // characterList.push_back(new Protester(60, 60, IID_PROTESTER, proTickStun,this));


        
        return GWSTATUS_CONTINUE_GAME;
    }

    /*
    move method() must:

    A: Update the Status Text on the top of the screen

    B: Must ask all the active actors to do something (like move)
        -If an actor does something that makes Iceman give up, move should return GWSTATUS_PLAYER_DIED
        -If Iceman collects all the oil, move should play the sound  SOUND_FINISHED_LEVEL
        and then return a value of GWSTATUS_FINISHED_LEVEL

    C:It must delete any actors that need be removed from the game and the STL container that tracks them
        -A Protester leaving the upper right hand corner
        -A Boulder hitting the ground
        -A Gold Nugget picked up by Iceman/Protester
        -Water Pool that has dried up
        -ETC
    
    */

    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        // cout << iceMap.size() << endl;
        
        //Updates the Textbox
        updateTextBox();
        
        //protester and hpro spawn
        /*int diceShuff = rand() % 101;
        int tickBetween =  max(25, 200 - static_cast<int>(getLevel()));
        int currentNum = 0;
        int pNum = min(15, static_cast<int>(2 + getLevel() * 1.5));
        int proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
        int probabilityOfHardcore = min(90, static_cast<int>(getLevel()) * 10 + 30);*/
        
        int spawnP = --tickBetween;
        //cout << currentNum << endl;
        //cout << pNum << endl;
        if (spawnP == 0) {
            //int spawnP = rand() % 5 + 1;
            cout << "SpawnP: " << spawnP << endl;
            //if (spawnP == 1 && currentNum != pNum){
            if (currentNum < pNum) {
                if (diceShuff <= probabilityOfHardcore) {
                    characterList.push_back(new HardcoreProtester(60, 60, proTickStun, ticksToWaitBetweenMoves, this));
                    cout << "a wild hardcore protester appeared!" << endl;
                }
                else {
                    characterList.push_back(new Protester(60, 60, IID_PROTESTER, proTickStun, ticksToWaitBetweenMoves, this));
                    cout << "a wild protester appeared!" << endl;
                }
            }
            //}
            currentNum++;
            cout << currentNum << endl;
            tickBetween = max(25, 200 - static_cast<int>(getLevel())); // Reset tick count
            diceShuff = rand() % 101;
        }
    
        //spawn sonar and water
        
        int spawn = --ticksSonarWater;
        cout << spawn << endl;
        if (spawn == 0) {
            
            int spawnS = rand() % 5 + 1;
            cout << "Spawn Number: " << spawnS << endl;
            
            if (spawnS == 1){
                if(numberOfScanners < 2){
                cout << "a wild sonar appeared!" << endl;
                playSound(SOUND_SONAR);
                characterList.push_back(new SonarKit(0, 60, ticksSonarWater, this));
                }
            }
            
                else {
                       int x = rand() % 61;
                       int y = rand() % 57;
                       while (blockedbyRocksOrIce(x, y, Actor::up)) {
                           x = rand() % 61;
                           y = rand() % 57;
                           
                       }
                    //playSound(SOUND_);
                    cout << "a wild water pool appeared!" << endl;
                    cout << "X: " << x << endl;
                    cout << "Y: " <<  y << endl;
                    characterList.push_back(new WaterPool(x, y, ticksSonarWater, this));
                   }
                ticksSonarWater = max(100, (300 - 10) * static_cast<int>(getLevel()));
        }
        
        //Goes through each character and asks if it does something
        for (Actor* a : characterList) {
            if (a->isAlive()) {
                a->doSomething();
                
                if (!player->isAlive()) { //Checks if player dies and returns dies
                    return GWSTATUS_PLAYER_DIED;
                }
                
                if(completeLevel()){ // Checks if player completes level
                    return GWSTATUS_FINISHED_LEVEL;
                }
                
            }
        }

        //Removes all unalive objects are screen
        removeDeadObjects();

        //Asks if player is alive
        if (!player->isAlive()) {
            return GWSTATUS_PLAYER_DIED;
        }

        //if (false) {
        //    decLives();
        //}

        //If the lives equal zero, then return the player has perished
        if (getLives() == 0) {
            return GWSTATUS_PLAYER_DIED;
        }
        //Otherwise return continue game
        return GWSTATUS_CONTINUE_GAME;
    }
    /*
    
    This method is called when Iceman:
        -Loses a life
        -Completes the current level

    */
    virtual void cleanUp()
    {
        //Deletes all the stuff in icemap
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                delete iceMap.at(i).at(j); //deletes every ice in vector;
            }
        }
        //Clears the vector out
        iceMap.clear();
        increaseScore(-getScore());
        //Delete all the actors in character list
        for (Actor* a : characterList) {
            Actor* temp = a;
            a = nullptr;
            delete temp;
        }
        characterList.clear();

        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                delete leavingPath[i][j]; //deletes every ice in vector;
            }
        }
        
        //delete characterList;
    }
    
    bool completeLevel();
    void moveToShortPath(int startX, int startY);
    bool blockedByIce (int x, int y);
    void dropGold(int x, int y);
    bool pickUpGold(int x, int y);
    void sdeT(int t){t--;}
    void useSonar(int x, int y);
    bool useSpray(int x, int y);
    bool noIce(int x, int y);
    bool empty4(int x, int y);
    int countSonar = 0;
    bool blockedbyRocksOrIce(int x, int y, Actor::Direction d);//not done
    bool blockedByRocks(int x, int y);
    double getRadius(int x1, int x2, int y2, int y1){return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));}
    bool checkFacingDirection(int x, int y, Actor::Direction d);
    //Returns the characterlist
    list<Actor*>& getCharacterList() {
        return characterList;
    }

    //Returns the map
    vector<vector<Ice*>>& getMap() {
        return iceMap;
    }

    bool checkSpot(string actorType, int x, int y);
    
    IceMan* getIceMan();
    
    Boulder* getBoulder();
    
    Gold* getWorldGold();
    
    Ice* getIce();
    
    Protester* getProtester();

    void findPath(int x, int y, int objx, int objy);

    string getLeadingPathDistance(int x, int y) {
        string direction = "right";
        //pair<int, int> smallCoord = make_pair(x+1, y);
        int smallest = *(leavingPath[y][x+1]);
        if (*(leavingPath[y+1][x]) < smallest) {
            direction = "up";
            smallest = *(leavingPath[y+1][x]);
            //smallCoord = make_pair(x, y+1);
        }
        if (*leavingPath[y-1][x] < smallest) {
            direction = "down";
            smallest = *(leavingPath[y-1][x]);
            //smallCoord = make_pair(x, y-1);
        }
        if (*leavingPath[y][x-1] < smallest) {
            direction = "left";
            smallest = *(leavingPath[y][x-1]);
            //smallCoord = make_pair(x-1, y);
        }

        //cout << "x:" << smallCoord.first << endl;
        //cout << "y:" << smallCoord.second << endl;
        cout << smallest << endl;
        //pair<pair<int, int>, string> movement= direction;
        return direction;
    }
    
    HardcoreProtester* getHardcoreProtester();

private:
    Actor* a;
    vector<vector<Ice*>> iceMap; //Used to keep track of ice on map
    list<Actor*> characterList;
    IceMan* player;
    int oilLeft;
    int* leavingPath[64][64];
    struct Point {
    int x;
    int y;
        int ticksSonarWater = 0;
    Point(int x, int y) : x(x), y(y) {}
    };

    int diceShuff = rand() % 101;
    int tickBetween = max(25, 200 - static_cast<int>(getLevel()));
    int ticksToWaitBetweenMoves;
    int currentNum = 0;
    int pNum = min(15, static_cast<int>(2 + getLevel() * 1.5));
    int proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
    int probabilityOfHardcore = min(90, static_cast<int>(getLevel()) * 10 + 30);
    int numberOfScanners = 0;

    list<Point> goldPos;
    list<Point> rockPos;
    list<Point> icePos;
    int ticksSonarWater = max(100, (300 - 10) * static_cast<int>(getLevel()));
    struct Node{
    Node* next;
    bool visited;
    int x;
    int y;
    int distance;
    list<Node*> neighbors;
    Node* parent;
    Node(int x, int y) : x(x), y(y), distance(INFINITY), parent(NULL), visited(false), neighbors() {}
    };
    Node* head;
    list<Node*> hProNodes;

    Node* getNode(int x, int y) {
    for (Node* node : hProNodes) {
    if (node->x == x && node->y == y) {
    return node;
    }
    }
    return nullptr; // Node not found
    }

    void addNode(int x, int y);
    void createNodes();
    bool nodeBeenVisited(int x, int y);
    int getNearest();

      void removeDeadObjects();

    //Creates the icemap
    void createIceMap();

    //Spawns boulders
    void spawnBoulders(int bNum);

    //Spawns the gold nuggets
    void spawnNuggets(int num);
    
    //Spawn Water sq
    void spawnWater(int wNum, int tickNum);
    
    void spawnSW(int swNum, int tick);
    
    //Spawn sonar
    void spawnSonar(int sNum, int tickNum);
    
    //Spawn Oil
    void spawnOil(int oNum);
    
    void spawnProtesters(int pNum);
    //Checks the distance between 2 objects
    bool checkDistance(Actor* a, int obj1X, int obj1Y, int obj2X, int obj2Y);

    //Updates the text box
    void updateTextBox();

    //Formats the text to look nice
    string formatText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score);
    
};

#endif // STUDENTWORLD_H_
