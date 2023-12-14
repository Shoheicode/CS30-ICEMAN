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
    //Spawns ices for all 64 y spots
    for (int i = 0; i < 64; i++) {
        vector<Ice*> temp;
        //Spawns Ice for all 64 x spots
        for (int j = 0; j < 64; j++) {
            //Checks to ensure that the game has the gap
            if (i >= 60) {
                temp.push_back(nullptr);
            }
            else if (!(j <= 33 && j >= 30 && i >= 4)) {
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

bool StudentWorld::noIce(int x, int y) {
    if (iceMap[x][y] == nullptr) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (iceMap[x + i][y + j] != nullptr) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        } else {
            return false;
        }
        return false;
}

//bool StudentWorld::empty4(int x, int y){
//    
//}


void StudentWorld::spawnWater(int wNum, int tickNum){
    cout <<"spawnWater was called!" << endl;
    int tickWait = tickNum;
    //wait for 10 seconds to spawn
    //look for a place on map that doesnt have ice
    
    //convert to ice block if not picked up
    
    int currentNum = 0;
    //Creates water until the number of oil equals the number needed for the level.
    while (currentNum != wNum && tickWait !=0) {
        //Sets create boulder to true
        bool createWater = true;

        //Creates the random numbers
        int x = rand() % 58;
        int y = rand() % 37 + 20;
        while (iceMap[x][y] !=NULL){//checks if coords dont have ice
            x = rand() % 58;
            y = rand() % 37 + 20;
        }
        cout << tickWait<< endl;
        tickWait--;
        //Checks to make sure boulder isn't in the gap
        if (x <= 33 && x >= 27) {
            createWater = false;
        }
        //otherwise
        else {
            //For each character in character list
            for (Actor* a : characterList) {
                //Check if the distance is less than 6 and if not, don't create water
                if (!checkDistance(a, a->getX(), a->getY(), x, y)) {
                    createWater = false;
                    break;
                }

            }
        }
        //If the distance between each object is less than 6, create boulder
        if (createWater && tickWait ==0) {
            //Add to character list
            characterList.push_back(new WaterPool(x, y, tickNum, this));
            //Increase the number of current boulders
            currentNum++;
            tickWait = tickNum; //resets time to wait until next drop
            cout << "I spawned A Water Pool at " << x << "," << y << endl;
//                }
            }
                //if x && y point to a NULL pointer on icemap add to character list
            
//            //Add to character list
//            characterList.push_back(new WaterPool(x, y, tickNum, this));
//            //Increase the number of current boulders
//            currentNum++;
            

            
        }
    }

void StudentWorld::spawnSonar(int sNum, int tickNum) {
    int ticWait = tickNum;
    int currentNum = 0;

    while (currentNum != sNum) {
        bool createSonar = true;
        ticWait--;
        cout << ticWait << endl;
        //sdeT(ticWait);
        if (createSonar && ticWait == 0) {
            characterList.push_back(new SonarKit(0, 60, tickNum, this));
            cout << "sonar created get it quick!" << endl;
            currentNum++;
            ticWait = tickNum; // Reset tick wait
        }
    }
    cout << "all sonar created!" << endl;
    cout << sNum << endl;
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
    rockPos.push_back(Point(x, y));
    //rocksOnMap.push_back(new Boulder(x, y, this));
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


bool StudentWorld::blockedByRocks(int x, int y){
for (const Point& p : rockPos){
double radius = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
if (radius <= 4){
return true;
}
}
return false;
}

bool StudentWorld::blockedByIce(int x, int y){
    for (int i = x; i < x+4; i++) {
            for (int j = y; j < y+4; j++) {
                if (iceMap[i][j] != nullptr)
                    return false;
            }
        }
        return true;
   
}

bool StudentWorld::blockedbyRocksOrIce(int x, int y, Actor::Direction d){
    if (blockedByIce(x, y) || blockedByRocks(x, y)){
        return true;
    }
        
//    for (Actor* p : characterList) {
//        Ice* iceP = iceMap[x][y];
//        if (a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER && (abs(iceP->getY() - a->getY()) < 4)) {
//            if (abs(iceP->getX() - a->getX()) < 4) {
//                return true;
//            }
//        }
//    }

return false;
}

void StudentWorld::dropGold(int x, int y){
if(player->getGold() > 0){
//set gold to pick up able by protesters
    player->setGold(-1);
getCharacterList().push_back(new Gold(player->getX(), player->getY(), true, this));//rest handled in constructor
goldPos.push_back((Point(player->getX(), player->getY())));
}
}

bool StudentWorld::pickUpGold(int x, int y){
for (const Point& p : goldPos){
    double radius = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
        if (radius <= 4){
            for (Actor* a : characterList) {
                if (a->getID() == IID_GOLD && a->getX() == p.x && a->getY() == p.y){
                                cout << "protester picked up gold!" << endl;
                                a->setAlive(false);
                                return true;
                }
            }
        }

    }
return false;
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
    int barrelsLeft = oilLeft - player->getOil();
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
    string returnString = "Lvl: " + levelString + " Lives: " + livesString + " Hlth: " + healthString + "% Wtr: " + squirtsString + " Gld: " + goldString + " Oil left: " +
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
            if(getRadius(x, p->getX(), y, p->getY()) <= 12){
                if (p->isAlive())
                    p->setVisible(true);
                }
            }
        }
        player->setSonar(-1);
    }

bool StudentWorld::useSpray(int x, int y){
    if(player->getSquirt() > 0){
        getCharacterList().push_back(new Squirt(x, y, player->getDirection(), this));
        playSound(SOUND_PLAYER_SQUIRT);
        player->setWater(-1);
        for (Actor* p : characterList){
            if((p->getID() == IID_PROTESTER || p->getID() == IID_HARD_CORE_PROTESTER) && getRadius(x, p->getX(), y, p->getY()) <= 6){
                if (p->isAlive())
                    p->setHitpoints(-2);
                playSound(SOUND_PROTESTER_ANNOYED);
                return true;
            }
        }
    }
    return false;
}
   
//
IceMan* StudentWorld::getIceMan(){
    for (Actor* a : characterList) {
        if (a->getID() == IID_PLAYER){
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

bool StudentWorld::checkSpot(string actorType, int x, int y) {
    for (Actor* act : characterList) {
        if (actorType == "Boulder") {
            if (act->getID() == IID_BOULDER && abs(act->getX() - x) < 4 && abs(act->getY() - y) < 4) {
                cout << "TRUE there is something" << endl;
                return true;
            }
        }
    }
    cout << "THERE IS NOTHING TRUE" << endl;
    return false;
}

void StudentWorld::findPath(int x, int y, int objx, int objy) {

    queue<pair<pair<int, int>, int>> q;
    pair <pair<int, int>, int> temp;

    temp = make_pair(make_pair(x, y), 0);

    int distance = 0;

    q.push(temp);
    int count = 0;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            leavingPath[i][j] = new int (100000000);
        }
    }
    leavingPath[y][x] = new int(0);//ending place

    cout << "COUNT: " << count << endl;
    //cout << q.front().first << endl;
    while (!q.empty()) {
        
        if (q.front().first.first >= 0) {
            //cout << "I AM RUNNING " << endl;
            //cout << "hi" << endl;
            cout << q.front().first.second << endl;
            cout << q.front().first.first << endl;
            if (q.front().first.first == 0) {
                leavingPath[q.front().first.second][q.front().first.first] = new int(q.front().second);
                //cout << "HELLO" << endl;
            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first - 1) == nullptr && !checkSpot("Boulder", objx - 1, objy) && *leavingPath[q.front().first.second][q.front().first.first-1] == 100000000) {
                cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first - 1) != nullptr) {
                        addtopath = false;
                        break;
                    }
                    /*else {
                        addtopath = false;
                        break;
                    }*/
                }
                if (addtopath) {
                    //cout << "ADDED TO THE PATH" << endl;
                    //cout << q.front().first << endl;
                    //cout << q.front().second << endl;
                    leavingPath[q.front().first.second][q.front().first.first-1] = new int(q.front().second+1);
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first - 1, q.front().first.second), q.front().second+1);
                    q.push(a);
                }
            }
        }
        if (q.front().first.second >= 0) {
            if (q.front().first.second == 0) {
                leavingPath[q.front().first.second][q.front().first.first] = new int(q.front().second);
            }
            else if (iceMap.at(q.front().first.second - 1).at(q.front().first.first) == nullptr && !checkSpot("Boulder", objx, objy - 1) && *leavingPath[q.front().first.second-1][q.front().first.first] == 100000000) {
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second - 1).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        //cout << "AM BREAKING" << endl;
                        break;
                    }
                    /*else {
                        addtopath = false;
                        break;
                    }*/
                }
                if (addtopath) {
                    //cout << "ADDED TO THE PATH" << endl;
                    cout << "x:" << q.front().first.first << endl;
                    cout << "y:" << q.front().first.second << endl;
                    leavingPath[q.front().first.second-1][q.front().first.first] = new int(q.front().second+1);
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second - 1), q.front().second+1);
                    q.push(a);
                }
            }
        }
        if (q.front().first.first <= 60) {
            cout << "I AM RUNNING " << endl;
            //cout << "hi" << endl;
            cout << q.front().first.second << endl;
            cout << q.front().first.first << endl;
            if (q.front().first.first == 60) {
                leavingPath[q.front().first.second][q.front().first.first] = new int(q.front().second);
                cout << "HELLO" << endl;
            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first + 4) == nullptr && !checkSpot("Boulder", objx - 1, objy) && *leavingPath[q.front().first.second][q.front().first.first + 1] == 100000000) {
                cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first + 4) != nullptr) {
                        addtopath = false;
                        break;
                    }
                    /*else {
                        addtopath = false;
                        break;
                    }*/
                }
                if (addtopath) {
                    cout << "ADDED TO THE PATH" << endl;
                    //cout << q.front().first << endl;
                    //cout << q.front().second << endl;
                    leavingPath[q.front().first.second][q.front().first.first + 1] = new int(q.front().second+1);
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first + 1, q.front().first.second), q.front().second+1);
                    q.push(a);
                }
            }
        }

        if (q.front().first.second <= 60) {
            cout << "I AM RUNNING " << endl;
            //cout << "hi" << endl;
            cout << q.front().first.second << endl;
            cout << q.front().first.first << endl;
            if (q.front().first.second == 60) {
                leavingPath[q.front().first.second][q.front().first.first] = new int(q.front().second);
                cout << "HELLO" << endl;
            }
            else if (q.front().first.second == 60 && *leavingPath[q.front().first.second][q.front().first.first] == 100000000) {

            }
            else if (iceMap.at(q.front().first.second+4).at(q.front().first.first) == nullptr && !checkSpot("Boulder", objx - 1, objy) && *leavingPath[q.front().first.second+1][q.front().first.first] == 100000000) {
                cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + 4).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        break;
                    }
                    /*else {
                        addtopath = false;
                        break;
                    }*/
                }
                if (addtopath) {
                    cout << "ADDED TO THE PATH" << endl;
                    //cout << q.front().first << endl;
                    //cout << q.front().second << endl;
                    leavingPath[q.front().first.second+1][q.front().first.first] = new int(q.front().second + 1);
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second+1), q.front().second + 1);
                    q.push(a);
                }
            }
        }

        if (q.front().first.first == objx && q.front().first.second == objy) {
            break;
        }
        cout << "I AM POPING" << endl;
        q.pop();
        cout << "SIZE: " << q.size() << endl;
    }

    if (q.empty()) {
        cout << "I AM EMPTY" << endl;
    }
    leavingPath[y][x] = new int(0);//ending place

    for (int i = 63; i >= 0; i--) {
        for (int j = 0; j < 64; j++) {
            if (*leavingPath[i][j] == 100000000) {
                cout << "A";
            }
            else {
                cout << *(leavingPath[i][j]);
            }
        }
        cout << endl;
    }

}





// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


