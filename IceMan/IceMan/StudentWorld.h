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

        //for (int i = 0; i < 64; i++) {
        //    for (int j = 0; j < 64; j++) {
        //        delete leavingPath[i][j]; //deletes every ice in vector;
        //    }
        //}

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
        int bNum = min(static_cast<int>(getLevel()) / 2 + 2, 9);

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
        //cout << "NUMBER OF PROSTERS:" << pNum << endl;
        proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
        probabilityOfHardcore = min(90, static_cast<int>(getLevel()) * 10 + 30);

        int proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));

        int proTickWait = max(0, 3 - (static_cast<int>(getLevel()) / 4));
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

        numberOfScanners = player->getSonarCount();

        //Spawns Oil
        spawnOil(oilLeft);

        //Adds a protestor at start
        characterList.push_back(new Protester(60, 60, IID_PROTESTER, proTickStun, ticksToWaitBetweenMoves, this));
        currentNum++;//increment protesters on map

        return GWSTATUS_CONTINUE_GAME;//once all spawned run game
    }

    virtual int move()
    {

        //Updates the Textbox
        updateTextBox();

        int spawnP = --tickBetween;

        if (spawnP == 0) {
            if (currentNum < pNum) {//check if current pros on map reached max pros on map
                if (diceShuff <= probabilityOfHardcore) {
                    characterList.push_back(new HardcoreProtester(60, 60, proTickStun, ticksToWaitBetweenMoves, this));//spawn hardcore protester
                    cout << "a wild hardcore protester appeared!" << endl;
                }
                else {
                    characterList.push_back(new Protester(60, 60, IID_PROTESTER, proTickStun, ticksToWaitBetweenMoves, this));//spawn protester
                    cout << "a wild protester appeared!" << endl;
                }
            }
            tickBetween = max(25, 200 - static_cast<int>(getLevel())); // reset tick count
            diceShuff = rand() % 101;//get another random num
        }

        //spawn sonar and water
        int spawn = --ticksSonarWater;//decrement ticks
        //cout << spawn << endl;
        if (spawn == 0) {//check if time to spawn
            int spawnS = rand() % 5 + 1;//calculate random 1/5 and 4/5 chance
            cout << "Spawn Number: " << spawnS << endl;
            if (spawnS == 1) {//if 1/5 spawn sonar
                if (numberOfScanners < 2) {
                    //cout << "a wild sonar appeared!" << endl;
                    playSound(SOUND_SONAR);
                    characterList.push_back(new SonarKit(0, 60, ticksSonarWater, this));
                    numberOfScanners++;
                }
            }

            else {//4/5 chance
                int x = rand() % 61;
                int y = rand() % 57;
                while (blockedbyRocksOrIce(x, y, Actor::up)) {
                    x = rand() % 61;//keep getting random nums until no rocks or ice at coord
                    y = rand() % 57;

                }
                cout << "a wild water pool appeared!" << endl;
                cout << "X: " << x << endl;
                cout << "Y: " << y << endl;
                characterList.push_back(new WaterPool(x, y, ticksSonarWater, this));//spawn water
            }
            ticksSonarWater = max(100, (300 - 10) * static_cast<int>(getLevel()));//reset ticks
        }

        //Goes through each character and asks if it does something
        for (Actor* a : characterList) {
            if (a->isAlive()) {
                a->doSomething();

                if (!player->isAlive()) { //Checks if player dies and returns dies
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }

                if (completeLevel()) { // Checks if player completes level
                    return GWSTATUS_FINISHED_LEVEL;
                }

            }
        }

        //Removes all unalive objects are screen
        removeDeadObjects();

        //Asks if player is alive
        if (!player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        //If the lives equal zero, then return the player has perished
        if (getLives() == 0) {
            return GWSTATUS_PLAYER_DIED;
        }
        //Otherwise return continue game
        return GWSTATUS_CONTINUE_GAME;
    }

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

        //for (int i = 0; i < 64; i++) {
        //    for (int j = 0; j < 64; j++) {
        //        delete leavingPath[i][j]; //deletes every ice in vector;
        //    }
        //}

        //delete characterList;
    }

    //public member funcs
    bool completeLevel();
    //void moveToShortPath(int startX, int startY);
    bool blockedByIce(int x, int y);
    void dropGold(int x, int y);
    bool pickUpGold(int x, int y);
    void sdeT(int t) { t--; }
    void useSonar(int x, int y);
    bool useSpray(int x, int y);
    //bool empty4(int x, int y);
    int countSonar = 0;
    bool blockedbyRocksOrIce(int x, int y, Actor::Direction d);//not done
    bool blockedByRocks(int x, int y);
    double getRadius(int x1, int x2, int y2, int y1) { return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); }
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

    Ice* getIce();

    Protester* getProtester();

    void findPath(int x, int y, int objx, int objy);

    bool findPathToIceMan(int x, int y, int maxMoves);

    string getLeadingPathDistance(int x, int y);

    HardcoreProtester* getHardcoreProtester();

    void decScanners() {
        numberOfScanners--;
    }

private:
    Actor* a;
    vector<vector<Ice*>> iceMap; //Used to keep track of ice on map
    list<Actor*> characterList;
    IceMan* player;
    int oilLeft;
    int leavingPath[64][64];

    struct Point {
        int x;
        int y;
        Point(int x, int y) : x(x), y(y) {}
    };

    //set math formulas///
    int diceShuff = rand() % 101;
    int tickBetween = max(25, 200 - static_cast<int>(getLevel()));
    int pNum = min(15, static_cast<int>(2 + getLevel() * 1.5));
    int proTickStun = max(50, 100 - (10 * static_cast<int>(getLevel())));
    int probabilityOfHardcore = min(90, static_cast<int>(getLevel()) * 10 + 30);
    int ticksSonarWater = max(100, (300 - 10) * static_cast<int>(getLevel()));

    int* pathToIceman[64][64];

    int ticksToWaitBetweenMoves;
    int numberOfScanners = 0;
    int currentNum = 0;
    list<Point> goldPos;
    list<Point> rockPos;

    void removeDeadObjects();

    //Creates the icemap
    void createIceMap();

    //Spawns boulders
    void spawnBoulders(int bNum);

    //Spawns the gold nuggets
    void spawnNuggets(int num);

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
