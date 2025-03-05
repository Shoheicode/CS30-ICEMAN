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
            if ((*itr)->getID() == IID_PROTESTER || (*itr)->getID() == IID_HARD_CORE_PROTESTER) {
                currentNum--;
            }
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
void StudentWorld::createIceMap() {
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
            }
            else {
                temp.push_back(nullptr); //Otherwise, set empty spots to nullptr
            }
        }
        iceMap.push_back(temp); //Pushes the row into the map.
    }
}

void StudentWorld::spawnOil(int oNum) {
    //Sets the current number of oil to 0
    int currentNum = 0;

    //Creates oil until the number of oil equals the number needed for the level.
    while (currentNum != oNum) {

        //Sets create oil to true
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
    bool createNugget = false;

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


bool StudentWorld::blockedByRocks(int x, int y) {
    for (const Point& p : rockPos) {//search thru all rocks on map
        double radius = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
        if (radius <= 4) {//if rock is within a radius of 4 to actor set true
            return true;
        }
    }
    return false;
}

bool StudentWorld::blockedByIce(int x, int y) {
    if (y == 61 || x == -1 || x == 61 || y == -1) {
        return true; // if is blocked by the wall and can't go outside of the border
    }
    for (int i = y; i < y + 4; i++) {
        for (int j = x; j < x + 4; j++) {
            if (iceMap[i][j] != nullptr) {//check a 4x4 square if has no ice
                return true;
            }
        }
    }
    return false;

}

bool StudentWorld::blockedbyRocksOrIce(int x, int y, Actor::Direction d) {
    if (blockedByIce(x, y) || checkSpot("Boulder", x, y)) {
        return true;//check both in 1 func, to make it easier
    }

    return false;
}

void StudentWorld::dropGold(int x, int y) {
    if (player->getGold() > 0) {
        //set gold to pick up able by protesters
        player->setGold(-1);
        getCharacterList().push_back(new Gold(player->getX(), player->getY(), true, this));//rest handled in constructor
        goldPos.push_back((Point(player->getX(), player->getY())));//add gold to dropped gold pos to search for later
    }
}

bool StudentWorld::pickUpGold(int x, int y) {
    for (const Point& p : goldPos) {//search all dropped gold
        double radius = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
        if (radius <= 4) {//if dropped gold within rad of 4
            for (Actor* a : characterList) {
                if (a->getID() == IID_GOLD && a->getX() == p.x && a->getY() == p.y) {
                    a->setAlive(false);//actor(protester) picks up gold, gold dies
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
    int health = static_cast<int>((player->getHealth() / 10.0) * 100);//percent of health
    int squirts = player->getSquirt();
    int gold = player->getGold();//getPlayerGoldCount();
    int barrelsLeft = oilLeft - player->getOil();
    int sonar = player->getSonarCount();
    int score = getScore();

    string formatedString = formatText(level, lives, health, squirts, gold, barrelsLeft, sonar, score);

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
    if (player->getOil() == oilGone) {//if iceman has all gold on field, go to next level
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(-getScore());//reset icemans score for next level
        return true;
    }
    return false;
}

void StudentWorld::useSonar(int x, int y) {
    if (player->getSonarCount() > 0) {//check if iceman has sonar
        playSound(SOUND_SONAR);
        for (Actor* p : characterList) {
            if (getRadius(x, p->getX(), y, p->getY()) <= 12) {//search all actors in rad of 12
                if (p->isAlive())
                    p->setVisible(true);//make them visible
            }
        }
    }
    player->setSonar(-1);//decrement icemans inventory
}

bool StudentWorld::useSpray(int x, int y) {
    if (player->getSquirt() > 0) {//check if iceman has squirt
        getCharacterList().push_back(new Squirt(x, y, player->getDirection(), this));//spawn squirt
        playSound(SOUND_PLAYER_SQUIRT);
        player->setWater(-1);//decrement icemans inventory
        for (Actor* p : characterList) {
            if ((p->getID() == IID_PROTESTER || p->getID() == IID_HARD_CORE_PROTESTER) && getRadius(x, p->getX(), y, p->getY()) <= 6) {//if hpro or pro in rad of 6 deal 2 damage
                if (p->isAlive()) {
                    p->setHitpoints(-2);
                    cout << "HIT POINTS: " << p->getHitpoints() << endl;
                    playSound(SOUND_PROTESTER_ANNOYED);
                    return true;
                }
            }
        }
    }
    return false;
}

//
IceMan* StudentWorld::getIceMan() {//gets iceman from actor list
    for (Actor* a : characterList) {
        if (a->getID() == IID_PLAYER) {
            return dynamic_cast<IceMan*>(a);
            break;
        }
    }
    return NULL;
}

Boulder* StudentWorld::getBoulder() {//gets boulder from actor list
    for (Actor* a : characterList) {
        if (a->getID() == IID_BOULDER) {
            return dynamic_cast<Boulder*>(a);
            break;
        }
    }
    return NULL;
}

Protester* StudentWorld::getProtester() {//gets protester from actor list
    for (Actor* a : characterList) {
        if (a->getID() == IID_PROTESTER) {
            return dynamic_cast<Protester*>(a);
            break;
        }
    }
    return nullptr;
}

Ice* StudentWorld::getIce() {//gets ice from actor list
    for (Actor* a : characterList) {
        if (a->getID() == IID_ICE) {
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
                //if boulder is in rad of 4 return true
                return true;
            }
        }
    }
    return false;
}

string StudentWorld::getLeadingPathDistance(int x, int y) {
    string direction = "right";//starting dir

    //fine the smallest value in array
    int smallest = (leavingPath[y][x + 1]);
    if ((leavingPath[y + 1][x]) < smallest) {
        direction = "up";
        smallest = (leavingPath[y + 1][x]);
    }
    if (leavingPath[y - 1][x] < smallest) {
        direction = "down";
        smallest = (leavingPath[y - 1][x]);
    }
    if (leavingPath[y][x - 1] < smallest) {
        direction = "left";
        smallest = (leavingPath[y][x - 1]);
    }
    return direction;
}

void StudentWorld::findPath(int x, int y, int objx, int objy) {
    //set up queue and pair
    queue<pair<pair<int, int>, int>> q;
    pair <pair<int, int>, int> temp;

    temp = make_pair(make_pair(x, y), 0);

    int distance = 0;

    q.push(temp);
    int count = 0;

    // set all array to not taken
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            leavingPath[i][j] = 1000;
        }
    }

    leavingPath[y][x] = 0;//ending place

    // Prints path. Good for debugging
    /*for (int i = 63; i >= 0; i--) {
        for (int j = 0; j < 64; j++) {
            if (leavingPath[j][i] == 1000) {
                cout << "X";
            }
            else {
                cout << "O";
            }
        }
        cout << endl;
    }*/
    while (!q.empty()) {
        if (q.front().first.first >= 0) {//if current node is on left
            if (q.front().first.first == 0) {
                leavingPath[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first - 1) == nullptr && !checkSpot("Boulder", objx - 1, objy) && leavingPath[q.front().first.second][q.front().first.first - 1] == 1000) {
                //if no ice at point, no boulders, and point is not taken
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first - 1) != nullptr) {
                        addtopath = false;
                        //check 4x4 square if ice
                        break;
                    }
                }
                if (addtopath) {
                    //add node to pair to make shortest path
                    leavingPath[q.front().first.second][q.front().first.first - 1] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first - 1, q.front().first.second), q.front().second + 1);
                    q.push(a);
                }
            }
        }
        //checks other nodes in pair with same conditions
        if (q.front().first.second >= 0) {//if current node is on left
            cout << "BYBYBYBYE" << endl;
            if (q.front().first.second == 0) {
                leavingPath[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (iceMap.at(q.front().first.second - 1).at(q.front().first.first) == nullptr && !checkSpot("Boulder", objx, objy - 1) && leavingPath[q.front().first.second - 1][q.front().first.first] == 1000) {
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second - 1).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {
                    leavingPath[q.front().first.second - 1][q.front().first.first] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second - 1), q.front().second + 1);
                    q.push(a);
                }
            }
        }
        if (q.front().first.first <= 60) {
            cout << "HEWWOWOWOW" << endl;
            if (q.front().first.first == 60) {//if current node is on right
                leavingPath[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first + 4) == nullptr && !checkSpot("Boulder", objx - 1, objy) && leavingPath[q.front().first.second][q.front().first.first + 1] == 1000) {
                //cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first + 4) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {

                    leavingPath[q.front().first.second][q.front().first.first + 1] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first + 1, q.front().first.second), q.front().second + 1);
                    q.push(a);
                }
            }
        }

        if (q.front().first.second <= 60) {
            cout << "GOOD BYEEEE" << endl;
            if (q.front().first.second == 60) {//if current node is on right
                leavingPath[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (q.front().first.second == 60 && leavingPath[q.front().first.second][q.front().first.first] == 1000) {

            }
            else if (iceMap.at(q.front().first.second + 4).at(q.front().first.first) == nullptr && !checkSpot("Boulder", objx - 1, objy) && leavingPath[q.front().first.second + 1][q.front().first.first] == 1000) {
                //cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + 4).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {

                    leavingPath[q.front().first.second + 1][q.front().first.first] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second + 1), q.front().second + 1);
                    q.push(a);
                }
            }
        }

        if (q.front().first.first == objx && q.front().first.second == objy) {
            cout << "I HAVE ENDED" << endl;
            break;        // exit loop if destination reached
        }
        q.pop();

    }

    cout << "PRINTING PATH TO LEAVE" << endl;
    for (int i = 63; i >= 0; i--) {
        for (int j = 0; j < 64; j++) {
            if (leavingPath[i][j] == 1000) {
                cout << "X";
            }
            else {
                cout << leavingPath[j][i];
            }
        }
        cout << endl;
    }

}

bool StudentWorld::findPathToIceMan(int x, int y, int maxMoves) {
    queue<pair<pair<int, int>, int>> q;
    pair <pair<int, int>, int> temp;

    temp = make_pair(make_pair(getIceMan()->getX(), getIceMan()->getY()), 0);

    int distance = 0;

    q.push(temp);

    // set all array to not taken
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            pathToIceman[i][j] = int(1000);
        }
    }

    pathToIceman[getIceMan()->getY()][getIceMan()->getX()] = 0;//ending place
    //cout << "X:" << getIceMan()->getX() << endl;
    bool endPath = false;

    while (!q.empty()) {

        if (q.front().first.first >= 0) {//if current node is on left
            if (q.front().first.first == 0) {
                pathToIceman[q.front().first.second][q.front().first.first] = q.front().second;

            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first - 1) == nullptr && !checkSpot("Boulder", q.front().first.first - 1, q.front().first.second) && pathToIceman[q.front().first.second][q.front().first.first - 1] == 1000) {
                //if no ice at point, no boulders, and point is not taken
                //cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first - 1) != nullptr) {
                        addtopath = false;
                        //check 4x4 square if ice
                        break;
                    }

                }
                if (addtopath) {
                    //add node to pair to make shortest path
                    if (q.front().second == maxMoves) {
                        endPath = true;
                    }
                    pathToIceman[q.front().first.second][q.front().first.first - 1] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first - 1, q.front().first.second), q.front().second + 1);
                    q.push(a);
                }
            }
        }
        //checks other nodes in pair with same conditions
        if (q.front().first.second >= 0) {//if current node is on left
            if (q.front().first.second == 0) {
                pathToIceman[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (iceMap.at(q.front().first.second - 1).at(q.front().first.first) == nullptr && !checkSpot("Boulder", q.front().first.first, q.front().first.second - 1) && pathToIceman[q.front().first.second - 1][q.front().first.first] == 1000) {
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second - 1).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {
                    if (q.front().second == maxMoves) {
                        endPath = true;
                    }
                    pathToIceman[q.front().first.second - 1][q.front().first.first] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second - 1), q.front().second + 1);
                    q.push(a);
                }
            }
        }
        if (q.front().first.first <= 60) {
            if (q.front().first.first == 60) {//if current node is on right
                pathToIceman[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (iceMap.at(q.front().first.second).at(q.front().first.first + 4) == nullptr && !checkSpot("Boulder", q.front().first.first - 1, q.front().first.second) && pathToIceman[q.front().first.second][q.front().first.first + 1] == 1000) {
                //cout << "I" << endl;
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + i).at(q.front().first.first + 4) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {
                    if (q.front().second == maxMoves) {
                        endPath = true;
                    }
                    pathToIceman[q.front().first.second][q.front().first.first + 1] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first + 1, q.front().first.second), q.front().second + 1);
                    q.push(a);
                }
            }
        }

        if (q.front().first.second <= 60) {

            if (q.front().first.second == 60) {//if current node is on right
                pathToIceman[q.front().first.second][q.front().first.first] = q.front().second;
            }
            else if (q.front().first.second == 60 && pathToIceman[q.front().first.second][q.front().first.first] == 1000) {

            }
            else if (iceMap.at(q.front().first.second + 4).at(q.front().first.first) == nullptr && !checkSpot("Boulder", q.front().first.first - 1, q.front().first.second) && pathToIceman[q.front().first.second + 1][q.front().first.first] == 1000) {
                bool addtopath = true;
                for (int i = 0; i < 4; i++) {
                    if (iceMap.at(q.front().first.second + 4).at(q.front().first.first + i) != nullptr) {
                        addtopath = false;
                        break;
                    }

                }
                if (addtopath) {
                    if (q.front().second == maxMoves) {
                        endPath = true;
                    }
                    pathToIceman[q.front().first.second + 1][q.front().first.first] = q.front().second + 1;
                    pair<pair<int, int>, int> a = make_pair(make_pair(q.front().first.first, q.front().first.second + 1), q.front().second + 1);
                    q.push(a);
                }
            }
        }

        if ((q.front().first.first == x && q.front().first.second == y)) {
            return true;
            break;        // exit loop if destination reached
        }
        if (endPath) {
            return false;
            break;
        }
        q.pop();
        //count++;
    }
    return false;
}

bool StudentWorld::checkFacingDirection(int x, int y, Actor::Direction d) {
    // Check if the player is in a specific direction from given coordinates (x, y)
    if (player->getX() < x && d == Actor::Direction::left) {
        return true;
    }
    else if (player->getY() < y && d == Actor::Direction::down) {
        return true;
    }
    else if (player->getY() > y && d == Actor::Direction::up) {
        return true;
    }
    else if (player->getX() > x && d == Actor::Direction::right) {
        return true;
    }
    else {
        return false;
    }
    //true if the player is in that direction
}




// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

