#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <future>

//Actor
bool Actor::outOfField(int x, int y, Actor::Direction d){
    switch(d){
        case Actor::up://if faced up, and is moved higher than 60 units it is out of Field
            if(y >= 60){
                return true;
            }
            break;
            
        case Actor::down://if faced down, and is moved lower than 0 units it is out of Field
            if(y <= 0){
                return true;
            }
            break;
        case Actor::left://if faced left, and is moved farther than 0 units it is out of Field
            if(x <= 0){
                //cout << "DO NOT MOVE" << endl;
                return true;
            }
            break;
        case Actor::right://if faced right, and is moved farther than 60 units it is out of Field
            if(x >= 60){
                return true;
            }
            break;
        case Actor::none://if not faced any direction it is in field
            return false;
            break;
    }
    return false;
}

bool Actor::isFacingIceMan(Direction d, StudentWorld* world){
    for (Actor* a : world->getCharacterList()) {
        if(a->getID() ==IID_PLAYER && a->getDirection() != getDirection()){//if not facing iceman return false
            break;
            return false;
        }
        break;
        return true;
    }
    return false;
}
void Actor::setFacingIceMan(Direction d,StudentWorld* world){
    if (!isFacingIceMan(getDirection(), studW)){//if not facing iceman, set direction to his direction
        setDirection(world->getIceMan()->getDirection());
    }
}
string Actor::is4Away(StudentWorld* world){
    if (isAlive()){
        for (Actor* a : world->getCharacterList()) {
            int x = a->getX();
            int y = a->getY();
            double radius = sqrt(pow(x - getX(), 2) + pow(y - getY(), 2));
            if (radius<=4) {//checks if actor is a rad of 4 away
                if (a->getID() ==IID_PLAYER){
                    return "IceMan";
                }
                else if (a->getID() ==IID_PROTESTER){
                    return "Protester";
                }
                else if (a->getID() ==IID_HARD_CORE_PROTESTER){
                    return "Hard Protester";
                }
            }
            else if (radius > 4){//checks if actor is a rad of farther than 4 away
                if (a->getID() ==IID_PLAYER){
                    return "Greater IceMan";
                }
                else if (a->getID() ==IID_PROTESTER){
                    return "Greater Protester";
                }
                else if (a->getID() ==IID_HARD_CORE_PROTESTER){
                    return "Greater Hard Protester";
                }
            }
        }
    }
    return "nothing near";
}

string Actor::is3Away(StudentWorld* world){
    if (isAlive()){
        for (Actor* a : world->getCharacterList()) {
            int x = a->getX();
            int y = a->getY();
            double radius = sqrt(pow(x - getX(), 2) + pow(y - getY(), 2));
            if (radius<=3) {//checks if actor is a rad of 3 away
                if (a->getID() ==IID_PLAYER){
                    return "IceMan";
                }
                else if (a->getID() ==IID_PROTESTER){
                    return "Reg Protester";
                }
                else if (a->getID() ==IID_HARD_CORE_PROTESTER){
                    return "Hard Protester";
                }
            }
        }
    }
    return "nothing near";
}


void Boulder::doSomething() {
    if (isAlive()) {
        if (getState() == stable) {//if stable check if all 4 units below have ice
            int x = getX();
            int y = getY();
            int numBelow = 0;
            for (int i = 0; i < 4; i++) {
                if (getWorld()->getMap().at(y - 1).at(x + i) == nullptr) {
                    numBelow++;
                }
            }
            if (numBelow == 4) {//if so change to waiting
                currentState = waiting;
                //cout << numBelow << endl;
            }
        }
        else if (getState() == waiting) {
            if (wait != 0) {//decrement ticks
                wait--;
                //cout << wait << endl;
            }
            else {
                //cout << "DONE WAITING! It is falling time" << endl;
                currentState = falling;//fall
                getWorld()->playSound(SOUND_FALLING_ROCK);
            }
        }
        else if (getState() == falling) {
            if (getY() == 0) {
                setAlive(false);//hit ground set dead
                //cout << "I HIT THE GROUND" << endl;
            }
            else {
                overlap(getWorld());
                for (int i = 0; i < 4; i++) {
                    if (getWorld()->getMap().at(getY() - 1).at(getX()+i) != nullptr) {
                        setAlive(false);//if not fallen 4 and no ice, keep falling
                        //cout << "I HIT ICE" << endl;
                    }
                }
            }
            moveTo(getX(), getY() - 1);//move down 1
        }
    }
}

void Boulder::overlap(StudentWorld* world) {
    for (Actor* a : world->getCharacterList()) {
        //Checking if overlaping with characters
        if (a->getID() == IID_PLAYER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                world->decLives();//if hit player dec lives
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                a->setAlive(false);
            }
        }
        if (a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {//if hit protester set dead
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                studW->increaseScore(500);//inc score
                dynamic_cast<Protester*>(a)->setLeaveOil(true);//prompt pro to leave field
            }
        }
    }
}

void IceMan::doSomething(){
    if (isAlive()){
        overlap(studW);//dig when overlap with ice
        isInRange(studW);
        checkAnnoyed();
        int a;
        if (getWorld()->getKey(a) == true) { //player pressed key
            bool blocked = false;
            switch (a){
                case KEY_PRESS_ESCAPE://escape = decrease lives
                    getWorld()->decLives();
                    setAlive(false);
                    break;
                case KEY_PRESS_SPACE://use water gun
                    
                    getWorld()->useSpray(getX(), getY());
                    break;
                case KEY_PRESS_LEFT://move left
                    if (getDirection() != left){
                        setDirection(left); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else {
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getX() + 3 == getX() - 1 && abs(getY() - a->getY()) < 4) {
                                blocked = true;//if blocked by boulder dont move
                            }
                        }
                        if (!blocked) {//move if nothing blocks it
                            moveTo(getX() - 1, getY());
                        }
                    }
                    break;
                case KEY_PRESS_RIGHT:
                    if (getDirection() != right){
                        setDirection(right); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else {
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getX() == getX() + 4 && abs(getY() - a->getY()) < 4) {//if blocked by boulder dont move
                                blocked = true;
                            }
                        }
                        if (!blocked) {
                            //move if nothing blocks it
                            moveTo(getX() + 1, getY());
                        }

                    }
                    break;
                case KEY_PRESS_UP:
                    if (getDirection() != up){
                        setDirection(up); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else {
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getY() == getY() + 4 && abs(getX() - a->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
                            }
                        }
                        if (!blocked) {//move if nothing blocks it
                            moveTo(getX(), getY() + 1);
                        }
                    }
                    break;
                case KEY_PRESS_DOWN:
                    if (getDirection() != down){
                        setDirection(down); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else {
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getY() + 3 == getY() - 1 && abs(getX() - a->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
                            }
                        }
                        if (!blocked) {//move if nothing blocks it
                            moveTo(getX(), getY() - 1);
                        }
                        //}
                    }
                    break;
                    case 'z':
                    case 'Z':
                        getWorld()->useSonar(getX(), getY());//use sonar if z
                        break;
                    case KEY_PRESS_TAB:
                        getWorld()->dropGold(getX(), getY());//drop gold if tab
                        break;
                    
            }
        }
    }
    else{
        
        return;
    }
    //delete rock;
}

void IceMan::overlap(StudentWorld* world) {
    //ICE
    //sets digging to false
    bool digging = false;
    //Check is the y is less than 60 (aka below the ice)
    if (getY() <= 59) {
        //Create a box around the character to remove the ice around the character
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                //Removes the ice if the ice is within the box of the character
                if (getY()+y <= 59) {
                    //if the x and y position of the ice equals the x and y position within the box around the character
                    if (world->getMap().at(getY() + y).at(getX() + x) != nullptr) {
                        //Delete the ice
                        Ice* temp = world->getMap().at(getY() + y).at(getX() + x);
                        world->getMap().at(getY() + y).at(getX() + x) = nullptr;
                        delete temp;
                        digging = true;
                    }
                }
            }
        }
    }
    //Play sound if digging is true
    if (digging) {
        world->playSound(SOUND_DIG);
    }
}

void IceMan::isInRange(StudentWorld* world){
    for (Actor* a : world->getCharacterList()) {
        if (a->getID() == IID_GOLD && a->is3Away(studW) == "IceMan")
        {
            //gold++;//picks up gold
            return;
        }
        else if (a->getID() == IID_BARREL && a->is3Away(studW) == "IceMan")
        {
            oil++;//picks up oil
            return;
        }
        else if (a->getID() == IID_SONAR && a->is3Away(studW) == "IceMan")
        {
            sonarC++;//picks up sonar
            getWorld()->increaseScore(75);
            return;
        }
        else if (a->getID() == IID_WATER_POOL && a->is3Away(studW) == "IceMan")
        {
            //waterSq++;//picks up water
            getWorld()->increaseScore(100);
            return;
        }
    }
           
}

//NOT DONE YET
void IceMan::checkAnnoyed(){
    if(getHitpoints() == 0){
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);//if no health set dead
        setAlive(false);
    }
}

string doSomethingYou(StudentWorld* world, int x, int y) {
    return world->getLeadingPathDistance(x, y);
}

//protester
bool followingPath = false;
void Protester::doSomething(){
    vector<Direction> paths;
    if (isAlive()){
        if (getHitpoints() <= 0 && leave_the_oil_field == false) {//check if no health
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            leave_the_oil_field = true;//leave field
        }
        if (ticksToWait > 0){
            ticksToWait--;//decrement ticks
            return;
        }
        if (hasShoutedLast15 > 0){
            hasShoutedLast15--;//decrement ticks
            return;
        }
        if (ticksToWait == 0) {
            if (leave_the_oil_field) {
                getWorld()->findPath(60,60, getX(), getY());//find shortest path to exit
                future<string> a = async(doSomethingYou, getWorld(), getX(), getY());
                string movement = a.get();
                if (movement == "left") {
                    setDirection(left);
                    moveTo(getX() - 1, getY());//move left if x != 60
                }
                else if (movement == "right") {
                    setDirection(right);
                    moveTo(getX() + 1, getY());//move right if x != 60
                }
                else if (movement == "down") {
                    setDirection(down);
                    moveTo(getX(), getY()-1);//move up if y != 60
                }
                else if (movement == "up") {
                    setDirection(up);
                    moveTo(getX(), getY()+1);//move down if y != 60
                }
                if (getX() == 60 && getY() == 60) {
                    setAlive(false);//exit reached, set dead
                }
            }
            else {
                tryGold(getX(), getY());//try to pick up gold every step
                if (is4Away(studW) == "IceMan" && getWorld()->checkFacingDirection(getX(), getY(), getDirection())) {
                    if (hasShoutedLast15 >= 0) {//yell if 4 away from iceman and facing him
                        getWorld()->playSound(SOUND_PROTESTER_YELL);//Playannoyedsound
                        hasShoutedLast15 = 15;
                        getWorld()->getIceMan()->setHitpoints(-2);
                        return;
                    }
                    moveTo(getX(), getY() - 1);
                }
                else if (is4Away(studW) == "Greater IceMan" && iceManisInSight(getX(), getY(), getWorld())) {
                    //cout << "MOVE GUY" << endl;
                    moveOne(getX(), getY(), getDirection());//if farther away move 1
                    numSquaresToMoveInCurrentDirection = 0;
                }
                else {
                    //cout << "RUNNING THE ELSE" << endl;
                    if (numSquaresToMoveInCurrentDirection == 0) {
                        //cout << "changing Directions" << endl;
                        bool chooseDirection = false;
                        int randomDir = rand() % 4;
                        while (!chooseDirection) {
                            cout << randomDir << endl;
                            //change direction to where there is no ice or boulders
                            if (listOfDir[randomDir] == left) {
                                if (!getWorld()->blockedByIce(getX() - 1, getY()) && !getWorld()->checkSpot("Boulder", getX() - 1, getY()) && getX() != 0) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            if (listOfDir[randomDir] == right) {
                                if (!getWorld()->blockedByIce(getX() + 1, getY()) && !getWorld()->checkSpot("Boulder", getX() + 1, getY()) && getX() != 60) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            else if (listOfDir[randomDir] == up) {
                                if (!getWorld()->blockedByIce(getX(), getY() + 1) && !getWorld()->checkSpot("Boulder", getX(), getY() + 1) && getY() != 60) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            else if (listOfDir[randomDir] == down) {
                                if (!getWorld()->blockedByIce(getX(), getY() - 1) && !getWorld()->checkSpot("Boulder", getX(), getY() - 1) && getY() != 0) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            randomDir = rand() % 4;//reset
                        }
                        setDirection(listOfDir[randomDir]);
                        numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);
                    }
                    //cout << "FORK TIME"
                    if (ticksforFork == 0) {//check fork time
                        //cout << "IT IS FORKTIME" << endl;
                        if (isAtFork(getX(), getY(), getWorld(), paths)) {
                            int random = rand() % paths.size();//turn random direction in fork
                            setDirection(paths.at(random));
                            numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);//reset moves
                            ticksforFork = 200;
                            paths.clear();//ticks
                        }
                    }
                    else {
                        ticksforFork--;//decrement ticks
                    }

                    
                    moveOne(getX(), getY(), getDirection());
                    numSquaresToMoveInCurrentDirection--;
                    //cout << "Number of Squares: " << numSquaresToMoveInCurrentDirection << endl;
                    
                    //if out of scope set moves to 0
                    if (getDirection() == left && getX() == 0) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == right && getX() == 60) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == up && getY() == 60) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == down && getX() == 0) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                }
                
            }
            ticksToWait = max(0, 3 - static_cast<int>(getWorld()->getLevel() / 4));//reset ticks
        }
   }
    return;
}

void Protester::isAnnoyed(){
    //boulder already covered
    //if gets sprayed by iceMan
    if (getHitpoints() <= 0){
        studW->increaseScore(100);//if no health increase score and leave field
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        leave_the_oil_field = true;
        return;
    }
}

void Protester::tryGold(int x, int y){
    if (getWorld()->pickUpGold(getX(), getY())){
        cout << "Im walking back now!!" << endl;//if pick up gold increase score and leave field
        studW->increaseScore(25);
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        leave_the_oil_field = true;
    }
}


void Protester::moveOne(int x, int y, Direction d){
    switch (d){
            //move 1 where there is no ice/boulders and not out of scope
        case up:
            if (!outOfField(x, y, d) && !getWorld()->blockedbyRocksOrIce(x, y + 1, getDirection())){
                moveTo(getX(), getY() + 1);
                break;
            }
            else if (outOfField(x, y + 1, d) || getWorld()->blockedbyRocksOrIce(x, y + 1, getDirection())){
                //setDirection(down);
                return;
            }
            break;
        case down:
            if (!outOfField(x, y, d) && !getWorld()->blockedbyRocksOrIce(x, y - 1, getDirection())){
                moveTo(getX(), getY() - 1);
                break;
            }
            else if (outOfField(x, y - 1, d) || getWorld()->blockedbyRocksOrIce(x, y - 1, getDirection())){
                //setDirection(up);
                return;
                break;
            }
            break;
        case right:
            if (!outOfField(x, y, d) && !getWorld()->blockedbyRocksOrIce(x +1, y, getDirection())){
                moveTo(getX() + 1, getY());
                break;
            }
            else if (outOfField(x + 1, y, d) || getWorld()->blockedbyRocksOrIce(x + 1, y, getDirection())){
                //setDirection(left);
                return;
                break;
            }
            break;
        case left:
            if (!outOfField(x, y, d) && !getWorld()->blockedbyRocksOrIce(x - 1, y, getDirection())){
                //cout << "MOVING LEFT" << endl;
                moveTo(getX() - 1, getY());
                break;
            }
            else if (outOfField(x - 1, y, d) || getWorld()->blockedbyRocksOrIce(x - 1, y, getDirection())){
                //setDirection(right);
                return;
                break;
            }
            break;
        case none:
            return;
    }
}

bool Protester::isAtFork(int x, int y, StudentWorld* world,vector<Direction>& path){
    
    //right
    int paths = 0;

    if (getDirection() != left && getDirection() != right) {
        //Checking left path if no ice or boulders
        if (!getWorld()->blockedByIce(getX() - 1, getY()) && !getWorld()->checkSpot("Boulder", getX() - 1, getY())) {
            paths++;
            path.push_back(left);
        }
        //Checking right path if no ice or boulders
        if (!getWorld()->blockedByIce(getX() + 1, getY()) && !getWorld()->checkSpot("Boulder", getX() + 1, getY())) {
            paths++;
            path.push_back(right);
        }
    }
    
    if (getDirection() != up && getDirection() != down) {
        //Checking up path if no ice or boulders
        if (!getWorld()->blockedByIce(getX(), getY() + 1) && !getWorld()->checkSpot("Boulder", getX(), getY() - 1)) {
            paths++;
            path.push_back(up);

        }

        //Checking down path if no ice or boulders
        if (!getWorld()->blockedByIce(getX(), getY() - 1) && !getWorld()->checkSpot("Boulder", getX(), getY() - 1)) {
            paths++;
            path.push_back(down);
        }
    }

    //cout << paths << endl;

    return paths != 0 ? true : false;
}

bool Protester::iceManisInSight(int x, int y, StudentWorld* world){
    //IceMan* man = world->getIceMan();
    if (world->getIceMan() != NULL) {
        int xI = world->getIceMan()->getX();
        int yI = world->getIceMan()->getY();
        if (x == xI) { //if they are on same row
            if (y <= yI) {
                for (int j = y; j <= yI; j++) {
                    for (int i = x; i < 4; i++) {
                        if (world->getMap()[j][x + i] != nullptr || world->checkSpot("Boulder", x+i, j)) {
                            return false;//if ice or boulder
                        }
                    }
                }
                cout << "UP" << endl;
                setDirection(up);
                return true;
            }
            else {
                for (int j = y; j >= yI; j--) {
                    for (int i = 0; i < 4; i++) {
                        if (world->getMap()[j][x + i] != nullptr || world->checkSpot("Boulder", x + i, j)) {
                            return false;
                        }
                    }
                }
                cout << "Down" << endl;
                setDirection(down);
                return true;
            }
        }
        else if (y == yI) { //if they are on same column
            if (x <= xI) {
                for (int j = x; j <= xI; j++) {
                    for (int i = 0; i < 4; i++) {
                        if (world->getMap()[y+i][j] != nullptr || world->checkSpot("Boulder", j, y+i)) {
                            return false;
                        }
                    }
                }
                cout << "Right" << endl;
                setDirection(right);
                return true;
            }
            else {
                for ( int j = x; j >= xI; j--) {
                    for (int i = 0; i < 4; i++) {
                        if (world->getMap()[y+i][j] != nullptr || world->checkSpot("Boulder", j, y+i)) {
                            return false;
                        }
                    }
                }
                cout << "left" << endl;
                setDirection(left);
                return true;
            }
        }
    }
    //delete man;
    return false;
}

//Hardcore Protester

void HardcoreProtester::doSomething() {//NOT COMPLETE
    vector<Direction> paths;
    if (isAlive()){
        if (getHitpoints() <= 0 && leave_the_oil_field == false) {//check if no health
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            leave_the_oil_field = true;//leave field
        }
        if (ticksToWait > 0){
            ticksToWait--;//decrement ticks
            return;
        }
        if (hasShoutedLast15 > 0){
            hasShoutedLast15--;//decrement ticks
            return;
        }
        if (ticksToWait == 0) {
            if (leave_the_oil_field) {
                getWorld()->findPath(60,60, getX(), getY());//find shortest path to exit
                string movement = getWorld()->getLeadingPathDistance(getX(),getY());
                if (movement == "left") {
                    setDirection(left);
                    moveTo(getX() - 1, getY());//move left if x != 60
                }
                else if (movement == "right") {
                    setDirection(right);
                    moveTo(getX() + 1, getY());//move right if x != 60
                }
                else if (movement == "down") {
                    setDirection(down);
                    moveTo(getX(), getY()-1);//move up if y != 60
                }
                else if (movement == "up") {
                    setDirection(up);
                    moveTo(getX(), getY()+1);//move down if y != 60
                }
                if (getX() == 60 && getY() == 60) {
                    setAlive(false);//exit reached, set dead
                }

            }
            else {
                tryGold(getX(), getY());//try to pick up gold every step
                if (is4Away(studW) == "IceMan" && getWorld()->checkFacingDirection(getX(), getY(), getDirection())) {
                    if (hasShoutedLast15 >= 0) {//yell if 4 away from iceman and facing him
                        getWorld()->playSound(SOUND_PROTESTER_YELL);//Playannoyedsound
                        hasShoutedLast15 = 15;
                        getWorld()->getIceMan()->setHitpoints(-2);
                        return;
                    }
                    moveTo(getX(), getY() - 1);
                }
                else if (is4Away(studW) == "Greater IceMan" && iceManisInSight(getX(), getY(), getWorld())) {
                    //cout << "MOVE GUY" << endl;
                    moveOne(getX(), getY(), getDirection());//if farther away move 1
                    numSquaresToMoveInCurrentDirection = 0;
                }
                else {
                    //cout << "RUNNING THE ELSE" << endl;
                    if (numSquaresToMoveInCurrentDirection == 0) {
                        //cout << "changing Directions" << endl;
                        bool chooseDirection = false;
                        int randomDir = rand() % 4;
                        while (!chooseDirection) {
                            cout << randomDir << endl;
                            //change direction to where there is no ice or boulders
                            if (listOfDir[randomDir] == left) {
                                if (!getWorld()->blockedByIce(getX() - 1, getY()) && !getWorld()->checkSpot("Boulder", getX() - 1, getY()) && getX() != 0) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            if (listOfDir[randomDir] == right) {
                                if (!getWorld()->blockedByIce(getX() + 1, getY()) && !getWorld()->checkSpot("Boulder", getX() + 1, getY()) && getX() != 60) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            else if (listOfDir[randomDir] == up) {
                                if (!getWorld()->blockedByIce(getX(), getY() + 1) && !getWorld()->checkSpot("Boulder", getX(), getY() + 1) && getY() != 60) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            else if (listOfDir[randomDir] == down) {
                                if (!getWorld()->blockedByIce(getX(), getY() - 1) && !getWorld()->checkSpot("Boulder", getX(), getY() - 1) && getY() != 0) {
                                    chooseDirection = true;
                                    break;
                                }
                            }
                            randomDir = rand() % 4;//reset
                        }
                        setDirection(listOfDir[randomDir]);
                        numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);
                    }
                    //cout << "FORK TIME"
                    if (ticksforFork == 0) {//check fork time
                        //cout << "IT IS FORKTIME" << endl;
                        if (isAtFork(getX(), getY(), getWorld(), paths)) {
                            int random = rand() % paths.size();//turn random direction in fork
                            setDirection(paths.at(random));
                            numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);//reset moves
                            ticksforFork = 200;
                            paths.clear();//ticks
                        }
                    }
                    else {
                        ticksforFork--;//decrement ticks
                    }

                    
                    moveOne(getX(), getY(), getDirection());
                    numSquaresToMoveInCurrentDirection--;
                    //cout << "Number of Squares: " << numSquaresToMoveInCurrentDirection << endl;
                    
                    //if out of scope set moves to 0
                    if (getDirection() == left && getX() == 0) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == right && getX() == 60) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == up && getY() == 60) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                    else if (getDirection() == down && getX() == 0) {
                        numSquaresToMoveInCurrentDirection = 0;
                    }
                }
                
            }
            ticksToWait = max(0, 3 - static_cast<int>(getWorld()->getLevel() / 4));//reset ticks
        }
   }
    return;
}

void HardcoreProtester::tryGold(int x, int y){
    if (getWorld()->pickUpGold(getX(), getY())){
        if (getGold() == 2){//if hpro gets 2 gold leave field
            cout << "Im MEGA RICHHH!!" << endl;
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            studW->increaseScore(50);
            leave_the_oil_field = true;
        }
        else{//other wise hpro continues persuit
            cout << "youre too cheap" << endl;
            setGoldInv(1);//increment inventory
        }
    }
}


//gold

void Gold::doSomething(){
    if(isAlive()){
        IceMan* man = getWorld()->getIceMan();
            if (!isVisible() && getState() == icePickUp && is4Away(studW) == "IceMan") {
                setVisible(true);//appear on screen
                return;
            }
            else if (getState() == icePickUp && is3Away(studW)== "IceMan"){
                setAlive(false);
                getWorld()->playSound(SOUND_GOT_GOODIE);
                man->setGold(1);

                studW->increaseScore(10);//increment score by 10
                //increment iceMan's gold  by 1
            }

            if(getState() == proPickUp){
                //cout << "HIII" << endl;
          
                setVisible(true);//appears on screen
                wait--;
                cout << wait << endl;

            }

            if (wait == 0) {
                setAlive(false);
            }
    }
            
           
 }

//oil

void Oil::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        if (!isVisible() && is4Away(studW) == "IceMan"){
            setVisible(true);//appear if 4 away
            return;
        }
        else if (is3Away(studW) == "IceMan"){//is pick up able by iceMan
            setAlive(false);
            getWorld()->playSound(SOUND_FOUND_OIL);
            man->setOil(1);//inc score and inventory set dead
            studW->increaseScore(1000);
        }
    }
    return;
}

//Squirt
int x = 0;
void Squirt::doSomething(){
    if (getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())) {
        setAlive(false);//set dead if spawned on ice or rocks
        return;
        }
    if (distance == 0){
        setAlive(false);//distance reached set dead
        return;
    }
    if (distance > 0){
        //distance not reached move 3 in set direction then set dead
        switch (getDirection()) {
            case up:
                if (getWorld()->blockedbyRocksOrIce(getX(), getY() + 3,getDirection())){
                    setAlive(false);
                }
                moveTo(getX(), getY() + 1);
                break;
            case down:
                if (getWorld()->blockedbyRocksOrIce(getX(), getY() - 3,getDirection())){
                    setAlive(false);
                }
                moveTo(getX(), getY() - 1);
                break;
            case left:
                if (getWorld()->blockedbyRocksOrIce(getX() - 3, getY(),getDirection())){
                    setAlive(false);
                }
                moveTo(getX() - 1, getY());
                break;
            case right:
                if (getWorld()->blockedbyRocksOrIce(getX() + 3, getY(),getDirection())){
                    setAlive(false);
                }
                moveTo(getX() + 1, getY());
                break;
            default:
                break;
        }
        detD();//decrease distance
    }
}


//sonar kit

void SonarKit::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        deT();//decrease time
        if(is3Away(studW) == "IceMan"){
            setAlive(false);//3 away from iceman inc score and inventory set dead
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->decScanners();
            man->setSonar(1);
            studW->increaseScore(75);
        }
        else if (ticksToWait == 0){
            setAlive(false);//if time ellapsed set dead
        }
    }
    return;
}

//water pool
void WaterPool::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        deT();//decrease time
        if (is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);//3 away from iceman inc score and inventory if less than 5 set dead
            if (man->getSquirt() >= 5){
                return;
            }
            man->setWater(5);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            studW->increaseScore(100);
        }
        else if (ticksToWait == 0){
        setAlive(false);//if time ellapsed set dead
        }
    }
    return;
}

//ice
bool Ice::overlap(StudentWorld* world) {
    for (Actor* a : world->getCharacterList()) {
        //Checking if overlaping with characters
        if (a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                return true;
            }
        }
    }
    return false;
}
