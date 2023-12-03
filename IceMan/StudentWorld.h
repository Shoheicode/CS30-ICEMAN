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
        for (int i = 0; i <= 59; i++) {
            for (int j = 0; j <= 64; j++) {
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
        int oNum = min(static_cast<int>(getLevel()) + 2, 21);
        
        //Number of Sonar Kits
        int sNum = max(100, 300 - (10 * static_cast<int>(getLevel())));
        
        //Number of Water Pools
        int wNum = max(100, 300 - (10 * static_cast<int>(getLevel())));

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
        
        //Spawns Oil
        spawnOil(oNum);

        //Adds a protestor
        //characterList.push_back(new Protester(40, 60, this));

        
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

        //Goes through each character and asks if it does something
        for (Actor* a : characterList) {
            if (a->isAlive()) {
                a->doSomething();

                if (!player->isAlive()) { //Checks if player dies and returns dies
                    return GWSTATUS_PLAYER_DIED;
                }
                
                if(completeLevel()
                   ){ // Checks if player completes level
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
        for (int i = 0; i <= 59; i++) {
            for (int j = 0; j <= 64; j++) {
                delete iceMap.at(i).at(j); //deletes every ice in vector;
            }
        }
        //Clears the vector out
        iceMap.clear();

        //Delete all the actors in character list
        for (Actor* a : characterList) {
            Actor* temp = a;
            a = nullptr;
            delete temp;
        }
        characterList.clear();
        //delete characterList;
    }

    bool completeLevel();
    void dropGold(int x, int y);
    void useSonar(int x, int y);
    void useSpray(int x, int y);

    //Returns the characterlist
    list<Actor*>& getCharacterList() {
        return characterList;
    }

    //Returns the map
    vector<vector<Ice*>>& getMap() {
        return iceMap;
    }
    bool pickUpgetStudGold();
    
    bool pickUpgetStudOil();
    
    bool pickUpgetStudSonar();
    
    bool pickUpgetStudWater();
    
    IceMan* getIceMan();
    
    Boulder* getBoulder();
    
    Gold* getWorldGold();
    
    Ice* getIce();
    
    Protester* getProtester();
    
    HardcoreProtester* getHardcoreProtester();

private:
    Actor* a;
    vector<vector<Ice*>> iceMap; //Used to keep track of ice on map
    list<Actor*> characterList;
    IceMan* player;

      void removeDeadObjects();

    //Creates the icemap
    void createIceMap();

    void spawnBoulders(int bNum);

    //Spawns the gold nuggets
    void spawnNuggets(int num);
    
    //Spawn Oil
    void spawnOil(int oNum);
    //Checks the distance between 2 objects
    bool checkDistance(Actor* a, int obj1X, int obj1Y, int obj2X, int obj2Y);


    //Updates the text box
    void updateTextBox();

    //Formats the text to look nice
    string formatText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score);
    
};

#endif // STUDENTWORLD_H_
