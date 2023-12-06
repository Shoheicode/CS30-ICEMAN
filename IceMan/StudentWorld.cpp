#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

void StudentWorld::removeDeadObjects() {

    //Check through the character list for if the actors are alive
    vector<list<Actor*>::iterator> it1;
    for (auto itr = characterList.begin(); itr != characterList.end(); itr++) {
        if ((*itr)->isAlive() == false) { //If the actors are not alive
            //Delete the Actors and set the point at it to null
            Actor* a = *itr;
            *itr = nullptr;
            delete a;
            it1.push_back(itr);
        }
    }

    //Delete the empty spots
    for (unsigned int i = 0; i < it1.size(); i++) {
        characterList.erase(it1.at(i));
    }
    
}

//Creates the icemap
void StudentWorld::createIceMap(){
    //Spawns ices for all 60 y spots
    for (int i = 0; i <= 59; i++) {
        vector<Ice*> temp;
        //Spawns Ice for all 65 x spots
        for (int j = 0; j <= 64; j++) {
            //Checks to ensure that the game has the gap
            if (!(j <= 33 && j >= 30 && i >= 4)) {
                temp.push_back(new Ice(j, i)); //Create the ice
                    // cout << 1;

            }
            else {
                temp.push_back(nullptr); //Otherwise, set empty spots to nullptr
            }
        }
        iceMap.push_back(temp); //Pushes the row into the map.
    }
}

void StudentWorld::spawnSonar(int sNum){//TICKSPAN IMPLEMENTATION and specified point?
    //must appear in x num of ticks into the game and disappear
    
    int currentNum = 0;

    //Creates oil until the number of oil equals the number needed for the level.
    while (currentNum != sNum) {

        //Sets create boulder to true
        bool createSonar = true;

        //Creates the random numbers
        int x = rand() % 58;
        int y = rand() % 37 + 20;

        //Checks to make sure boulder isn't in the gap
        if (x <= 33 && x >= 27) {
            createSonar = false;
        }
        //otherwise
        else {
            //For each character in character list
            for (Actor* a : characterList) {
                //Check if the distance is less than 6 and if not, don't create boulder
                if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
                    createSonar = false;
                    break;
                }

            }
        }
        //If the distance between each object is less than 6, create boulder
        if (createSonar) {
            //Add to character list
            characterList.push_back(new SonarKit(x, y, false, this));
           
            //Increase the number of current boulders
            currentNum++;
        }
    }
}
void StudentWorld::spawnOil(int oNum){
    //Sets the current number of oil to 0
    int currentNum = 0;

    //Creates oil until the number of oil equals the number needed for the level.
    while (currentNum != oNum) {

        //Sets create boulder to true
        bool createOil = true;

        //Creates the random numbers
        int x = rand() % 58;
        int y = rand() % 37 + 20;

        //Checks to make sure boulder isn't in the gap
        if (x <= 33 && x >= 27) {
            createOil = false;
        }
        //otherwise
        else {
            //For each character in character list
            for (Actor* a : characterList) {
                //Check if the distance is less than 6 and if not, don't create boulder
                if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
                    createOil = false;
                    break;
                }

            }
        }
        //If the distance between each object is less than 6, create boulder
        if (createOil) {
            //Add to character list
            characterList.push_back(new Oil(x, y, this));
           
            //Increase the number of current boulders
            currentNum++;
        }
    }

}

void StudentWorld::spawnBoulders(int bNum) {

    //Sets the current number of boulders to 0
    int currentNum = 0;

    //Creates boulders until the number of boulders equals the number needed for the level.
    while (currentNum != bNum) {

        //Sets create boulder to true
        bool createBoulder = true;

        //Creates the random numbers
        int x = rand() % 58;
        int y = rand() % 35 + 15;

        //Checks to make sure boulder isn't in the gap
        if (x <= 33 && x >= 27) {
            createBoulder = false;
        }
        //otherwise
        else {
            //For each character in character list
            for (Actor* a : characterList) {
                //Check if the distance is less than 6 and if not, don't create boulder
                if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
                    createBoulder = false;
                    break;
                }

            }
        }
        //If the distance between each object is less than 6, create boulder
        if (createBoulder) {
            //Add to character list
            characterList.push_back(new Boulder(x, y, this));

            //Clear out map for boulder
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    Ice* temp = iceMap.at(y + i).at(x + j);
                    iceMap.at(y + i).at(x + j) = nullptr;
                    delete temp;
                }
            }
            //Increase the number of current boulders
            currentNum++;
        }
    }

}

//Spawns the gold nuggets
void StudentWorld::spawnNuggets(int num) {
    //Creates the nuggets
    bool createNugget= false;

    //Sets the current number of nuggets created to 0
    int currentNum = 0;

    while (currentNum != num) {

        createNugget = true;

        //Generate random positions
        int x = rand() % 61;
        int y = rand() % 57;

        //Make sure the hole in the center is empty
        if (x <= 33 && x >= 27) {
            createNugget = false;
        }
        else {
            //For each of the actors in character list
            for (Actor* a : characterList) {
                //Check to ensure the distance between all the characters is not less than 6.
                if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
                    createNugget = false;
                    break;
                }

            }
        }
        //If create nugget is true, create a nugget and increase current number of nuggets
        if (createNugget) {
            characterList.push_back(new Gold(x, y, false, this));
            currentNum++;
        }
    }
}



//Checks the distance between 2 objects
bool StudentWorld::checkDistance(Actor* a, int obj1X, int obj1Y, int obj2X, int obj2Y) {
    //Checks the distnace between 2 objects using distance formula
    double distance = pow(pow(obj1X - obj2X, 2) + pow(obj1Y - obj2Y, 2), 0.5);
    
    //Returns false if the distance is less than 6
    if (distance < 6) {
        return false;
    }

    //return true if the distance is greater than or equal to 6
    return true;

}



//Updates the text box
void StudentWorld::updateTextBox() {
    
    int level = getLevel(); // Gets the current level
    int lives = getLives(); //Gets the current lives
    int health = static_cast<int>((player->getHealth()/10.0) * 100);//percent of health
    int squirts = player->getSquirt();
    int gold = player->getGold();//getPlayerGoldCount();
    int barrelsLeft = player->getOil();
    int sonar = player->getSonarCount();
    int score = getScore();

    string formatedString = formatText(level, lives,health,squirts,gold,barrelsLeft, sonar, score);

    //Sets the game text to the formated text.
    setGameStatText(formatedString);
}

//Formats the text to look nice
string StudentWorld::formatText(int level, int lives, int health, int squirts, int gold, int barrels, int sonar, int score) {
    //Converts all values into strings
    string levelString = to_string(level);
    string livesString = to_string(lives);
    string healthString = to_string(health);
    string squirtsString = to_string(squirts);
    string goldString = to_string(gold);
    string barrelsString = to_string(barrels);
    string sonarString = to_string(sonar);
    string scoreString = to_string(score);

    //Creates the format for the string
    string returnString = "Lvl: " + levelString + " Lives: " + livesString + " Hlth: " + healthString + "% Wtr: " + squirtsString + " Gld: " + goldString + " Oil: " +
        barrelsString + " Sonar: " + sonarString + " Scr: " + scoreString;

    //Returns the string
    return returnString;
}

bool StudentWorld::completeLevel() {
    int oilGone = min(static_cast<int>(getLevel()) + 2, 21);
    if(player->getOil() == oilGone){
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(-getScore());
        return true;
    }
    return false;
}

void StudentWorld::useSonar(int x, int y){
    if(player->getSonarCount() > 0){
        playSound(SOUND_SONAR);
        for (Actor* p : characterList){
            if(sqrt(pow(p->getX() - x, 2) + pow(p->getY() - y, 2) <= 12)){               //im not sure if this works please check
                p->setVisible(true);
            }
        }       player->setSonar(-1);
    }
    }
    
void StudentWorld::dropGold(int x, int y){
    if(player->getGold() > 0){
        //set gold to pick up able by protesters
        //player->gold--;  //setGold-1
        getCharacterList().push_back(new Gold(player->getX(), player->getY(), true, this));//rest handled in constructor
    }
}
void StudentWorld::useSpray(int x, int y){
    Protester* proP = getProtester();
    if(player->getSquirt() > 0){
        //proP->setHitpoints(-2);
        playSound(SOUND_PLAYER_SQUIRT);
        player->setWater(-1);
    }
    delete proP;
}

//
IceMan* StudentWorld::getIceMan(){
    for (Actor* a : characterList) {
        if (a->getID() == IID_PLAYER){
            //cout << "AM RUNNING BOIIIII" << endl;
            return dynamic_cast<IceMan*>(a);
            break;
        }
    }
    return NULL;
}

Boulder* StudentWorld::getBoulder(){   
    for (Actor* a : characterList) {
        if (a->getID() == IID_BOULDER){
            return dynamic_cast<Boulder*>(a);
            break;
        }
    }
    return NULL;}

Gold* StudentWorld::getWorldGold(){
    for (Actor* a : characterList) {
        if (a->getID() == IID_GOLD){
            return dynamic_cast<Gold*>(a);
            break;
        }
    }
    return NULL;
}

Protester* StudentWorld::getProtester(){
    for (Actor* a : characterList) {
        if (a->getID() == IID_PROTESTER){
            return dynamic_cast<Protester*>(a);
            break;
        }
    }
    return nullptr;
}

Ice* StudentWorld::getIce(){
    for (Actor* a : characterList) {
        if (a->getID() == IID_ICE){
            return dynamic_cast<Ice*>(a);
            break;
        }
    }
    return nullptr;
}
//maybe causes error cuz hardpro has no doSomething...?
//HardcoreProtester* StudentWorld::getHardcoreProtester(){   for (auto a : characterList) {       if (a->getID() == IID_HARD_CORE_PROTESTER){           return dynamic_cast<HardcoreProtester*>(a);           break;       }   }   return nullptr;}
//


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

//// while (The player has lives left)// {// Prompt_the_user_to_start_playing(); // “press a key to start” Initialize_the_game_world(); // you’re going to write this func// while (The player is still alive)// {// // each pass through this loop is a tick (1/20th of a sec)// // you’re going to write code to do the following// Ask_all_actors_to_do_something();// If_any_actors_died_then_delete_them_from_the_world();// // we write this code to handle the animation for// you Animate_all_of_the_alive_actors_to_the_screen();// Sleep_for_50ms_to_give_the_user_time_to_react();// }// // the player died – you’re going to write this code// Cleanup_all_game_world_objects(); // you’re going to write this  if (The player has more lives)// Prompt_the_player_to_continue();// }// Tell_the_user_the_game_is_over(); // “game over!”; we provide this
