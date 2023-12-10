#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
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
                return true;
            }
            break;
        case Actor::right://if faced right, and is moved farther than 60 units it is out of Field
            if(x >= 61){
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
        if(a->getID() ==IID_PLAYER && a->getDirection() != getDirection()){
            break;
            return false;
        }
        break;
        return true;
    }
    return false;
}
void Actor::setFacingIceMan(Direction d,StudentWorld* world){
    //IceMan* man = world->getIceMan();
    if (!isFacingIceMan(getDirection(), studW)){
        setDirection(world->getIceMan()->getDirection());
    }
}
string Actor::is4Away(StudentWorld* world){
//    IceMan* man = world->getIceMan();
//    Protester* proP = world->getProtester(); USE THESE
    if (isAlive()){
        //IceMan* man = world->getIceMan();
        for (Actor* a : world->getCharacterList()) {
            int x = a->getX();
            int y = a->getY();
            double radius = sqrt(pow(x - getX(), 2) + pow(y - getY(), 2));
            if (radius<=4) {
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
            else if (radius > 4){
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
            if (radius<=3) {
                if (a->getID() ==IID_PLAYER){// down and right of gold
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


//NOT DONE WITH BOULDER YET (STILL IN PROGRESS)
void Boulder::doSomething() {
    if (isAlive()) {
        if (getState() == stable) {
            int x = getX();
            int y = getY();
            int numBelow = 0;
            for (int i = 0; i < 4; i++) {
                if (getWorld()->getMap().at(y - 1).at(x + i) == nullptr) {
                    numBelow++;
                }
            }
            if (numBelow == 4) {
                currentState = waiting;
                cout << numBelow << endl;
            }
        }
        else if (getState() == waiting) {
            if (wait != 0) {
                wait--;
                cout << wait << endl;
            }
            else {
                //cout << "DONE WAITING! It is falling time" << endl;
                currentState = falling;
                getWorld()->playSound(SOUND_FALLING_ROCK);
            }
        }
        else if (getState() == falling) {
            if (getY() == 0) {
                setAlive(false);
                //cout << "I HIT THE GROUND" << endl;
            }
            else {
                overlap(getWorld());
                for (int i = 0; i < 4; i++) {
                    if (getWorld()->getMap().at(getY() - 1).at(getX()+i) != nullptr) {
                        setAlive(false);
                        //cout << "I HIT ICE" << endl;
                    }
                }
            }
            moveTo(getX(), getY() - 1);
        }
    }
}

void Boulder::overlap(StudentWorld* world) {
    for (Actor* a : world->getCharacterList()) {
        //Checking if overlaping with characters
        if (a->getID() == IID_BOULDER && a->getY() + 4 == getY()) {
            if (abs(getX() - a->getX()) < 4) {
                setAlive(false);
                //cout << "I HIT THE PLAYER"
            }
        }
        if (a->getID() == IID_PLAYER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                world->decLives();
                //setAlive(false);
                a->setAlive(false);
            }
        }
        if (a->getID() == IID_PROTESTER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                //Create annoyance
                //world->decLives();
                ////setAlive(false);
                //a->setAlive(false);
            }
        }
    }
}

//Iceman
//bool IceMan::isAlive(){
////    if (m_lives == 0){
////        return false;//ends level, terminates game
////    }
//    return true;
//}
bool followingPath = false;
void IceMan::doSomething(){
    Boulder* rock = getWorld()->getBoulder();
    if (isAlive()){
        if (followingPath) {
            //getWorld()->findPath(60, 60, getX(), getY());
            cout << endl;
            cout << endl;
            pair<int, int> coord = getWorld()->getLeadingPathDistance(getX(), getY());
            moveTo(coord.first, coord.second);
            if (coord.first == 60 && coord.second == 60) {
                followingPath = false;
            }
        }
        overlap(studW);//dig when overlap with ice
        isInRange(studW);
        int a;
        if (getWorld()->getKey(a) == true) { //player pressed key
            bool blocked = false;
            switch (a){
                case KEY_PRESS_ESCAPE:
                    cout << "PRESSED ESCAPE " << endl;
                    //getWorld()->decLives();
                    getWorld()->findPath(60, 60, getX(), getY());
                    followingPath = true;
                    //setAlive(false);
                    //getWorld()->increaseScore(-getSc)
                    break;
                case KEY_PRESS_SPACE:
                    getWorld()->useSpray(getX(), getY());
                    break;
                case KEY_PRESS_LEFT:
                    if (getDirection() != left){
                        setDirection(left); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{
                        if (rock != NULL) {
                            if (rock->getX() + 3 == getX() - 1 && abs(getY() - rock->getY()) < 4) {
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
                    else{
                        if (rock != NULL) {
                            if (rock->getX() == getX() + 4 && abs(getY() - rock->getY()) < 4) {//if blocked by boulder dont move
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
                    else{
                        if (rock != NULL) {
                            if (rock->getY() == getY() + 4 && abs(getX() - rock->getX()) < 4) {
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
                    else{
                        if (rock != NULL) {
                            if (rock->getY() + 3 == getY() - 1 && abs(getX() - rock->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
                            }
                        }
                        if (!blocked) {//move if nothing blocks it
                            moveTo(getX(), getY() - 1);
                        }
                    }
                    break;
                    case 'z':
                    case 'Z':
                        getWorld()->useSonar(getX(), getY());
                        break;
                    case KEY_PRESS_TAB:
                        getWorld()->dropGold(getX(), getY());
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
        if (a->getID() == IID_GOLD && a->is3Away(studW)=="IceMan")
        {
            //gold++;//picks up gold
            return;
        }
        else if (a->getID() == IID_BARREL && a->is3Away(studW)=="IceMan")
        {
            oil++;//picks up oil
            return;
        }
        else if (a->getID() == IID_SONAR && a->is3Away(studW)=="IceMan")
        {
            sonarC++;//picks up sonar
            return;
        }
        else if (a->getID() == IID_WATER_POOL && a->is3Away(studW)=="IceMan")
        {
            waterSq++;//picks up water
            return;
        }
    }
           
}

//NOT DONE YET
void IceMan::getAnnoyed(int dAmage){
    //if is shouted at
    setHitpoints(-2);
    if(hitPoints == 0){
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        //loose a life
    }
}

//protester
void Protester::doSomething(){
    if (isAlive()){
        if (ticksToWait > 0){
            ticksToWait--;
            return;
        }
        else if (ticksToWait==0 && !outOfField(getX(), getY(), getDirection())){
            if(leave_the_oil_field == true){
                if (getX()==60 && getY()==60){
                    setAlive(false);
                    //reset ticks to wait
                    return;
                }
                else if(getX()!=60 && !blockedByIceOrBoulder(getX(), getY(), studW)){
                    //turn direction based on Q
                    moveTo(getX() + 1, getY());
                    //reset ticks to wait
                    return;
                }
                else if(getY()!=60 && !blockedByIceOrBoulder(getX(), getY(), studW)){
                    //turn direction based on Q
                    moveTo(getX(), getY() + 1 );
                    //reset ticks to wait
                    return;
                }
            }
            else if(is4Away(studW) == "IceMan" && isFacingIceMan(getDirection(), studW)){
                if (hasShoutedLast15 == true){
                    getWorld()->playSound(SOUND_PROTESTER_YELL);
                    //iceMan -2 hitpoints
                    //reset ticks to wait
                    hasShoutedLast15 = false;
                }
            }
            else if (iceManisInSight(getX(), getY(), studW) && is4Away(studW) == "Greater IceMan"&& !blockedByIceOrBoulder(getX(), getY(), studW) && !isFacingIceMan(getDirection(), studW)){
                    setFacingIceMan(getDirection(), studW);
                    moveOne(getX(), getY(), getDirection());
                    numSquaresToMoveInCurrentDirection = 0;
                    //reset ticks to wait
                    return;
                
            }
            else if(!iceManisInSight(getX(), getY(), studW)){
                numSquaresToMoveInCurrentDirection--;
                if(numSquaresToMoveInCurrentDirection <= 0){
                    //pick random direction that is not blocked by boulders or Ice
                    numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
                    //reset ticks to wait
                    //take 1 step in that direction
                    moveOne(getX(), getY(), getDirection());
                    return;
                }
            }
            //else if isAtFork && canMove1Perpindicular && !madePerpTurn in 200 ticks
            //pick which 2 directions
            //pick whichevr 2 directions if both are good pick one randomly
            //set direction to new direction
            else if (blockedByIceOrBoulder(getX(), getY(), studW)){
                numSquaresToMoveInCurrentDirection = 0;
                return;
                //reset ticks to wait
                //pick new dirction in nonresting tick
            }
            
        }
        
   }
    return;
}

bool Protester::blockedByIceOrBoulder(int x, int y, StudentWorld* world){
    Boulder* rock = world->getBoulder();
    Ice* coldRock = world->getIce();
    if (coldRock != nullptr) {
        int xI = coldRock->getX();
        int yI = coldRock->getY();
        int xB = rock->getX();
        int yB = rock->getY();
        
            if (x + 1 == xB || x + 1 == xI || x - 1 == xB || x - 1 == xI) {
                // delete rock;
                //delete coldRock;
                return true;
            }
            else if (y + 1 == xB || y + 1 == xI || y - 1 == xB || y - 1 == xI) {
                if (!outOfField(getX(), getY(), getDirection())){
                    // delete rock;
                    //delete coldRock;
                    return true;
                }
                
            }
        }
    
    //delete rock;
   // delete coldRock;
    return false;
}

void Protester::moveOne(int x, int y, Direction d){
    switch (d){
        case up:
            if (!outOfField(x, y, d) && !blockedByIceOrBoulder(x, y, studW)){
                moveTo(getX(), getY() + 1);
                break;
            }
            else if (outOfField(x, y + 1, d)){
                setDirection(down);
            }
            break;
        case down:
            if (!outOfField(x, y, d) && !blockedByIceOrBoulder(x, y, studW)){
                moveTo(getX(), getY() - 1);
                break;
            }
            else if (outOfField(x, y - 1, d)){
                setDirection(up);
                break;
            }
            break;
        case right:
            if (!outOfField(x, y, d) && !blockedByIceOrBoulder(x, y, studW)){
                moveTo(getX() + 1, getY());
                break;
            }
                cout << "turning now!!" << endl;
                setDirection(left);
            
            break;
        case left:
            if (!outOfField(x, y, d) && !blockedByIceOrBoulder(x, y, studW)){
                moveTo(getX() - 1, getY());
                break;
            }
            else if (outOfField(x -1, y, d)){
                cout << "turning now!!" << endl;
                setDirection(right);
            }
            break;
        case none:
            return;
    }
}

bool Protester::isAtFork(int x, int y, StudentWorld* world){
    
    //right
    
    //right and left
    
    //right and up
    
    // right and down
    
    //left
    
    //left and up
    
    //left and down
    
    //up and down
    
    //if getX + 1 !blocked && getX -1 !blocked //left or right
    
    //else if getY + 1 !blocked && getY -1 !blocked //up or down
    //else if getY + 1 !blocked && getY -1 !blocked //up or down
    return true;
}

bool Protester::iceManisInSight(int x, int y, StudentWorld* world){
    //IceMan* man = world->getIceMan();
    if (world->getIceMan() != NULL) {
        int xI = world->getIceMan()->getX();
        int yI = world->getIceMan()->getY();
        if (x == xI) { //if they are on same row
            //delete man;
            return true;
        }
        else if (y == yI) { //if they are on same column
            //delete man;
            return true;
        }
    }
    //delete man;
    return false;
}

void Protester::isBribed(){
    //for each gold in character list
        Gold* gP = getWorld()->getWorldGold();
        if (gP->getX() == getX() && gP->getY() == getY()){//set gold to the right nugget?
            gP->setAlive(false);
            leave_the_oil_field = true;
        }
}

//Hardcore Protester

void HardcoreProtester::doSomething(){//NOT COMPLETE
    if (isAlive()){
        //ticksToWaitBetweenMoves = max(0, (3 – getLevel())/4);
        if (ticksToWaitBetweenMoves == 3){
            //wait 3 ticks
            //must wait until the fourth tick to doSomething
        }
        //if in rest state
        //increment ticks
        //return
        else if (leave_the_oil_field == true){
            if (getX() == 60 && getY() == 60){
                setAlive(false);
            }
            else if(getX()!=60 && !blockedByIceOrBoulder(getX(), getY(), studW)){
                //turn direction based on Q
                moveTo(getX() + 1, getY());
                return;
            }
            else if (getY() != 60 && !blockedByIceOrBoulder(getX(), getY(), studW)) {
                //turn direction based on Q
                moveTo(getX(), getY() + 1);
                return;
            }
        }
        else if (is4Away(studW) == "IceMan" && isFacingIceMan(getDirection(), studW) && hasShoutedLast15==true){
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            //IceMan -2 hitpoints
            hasShoutedLast15 = false;
            return;
        }
        //else if (isGreaterThan4Away(studW)== "IceMan"){
            //double M =16 + current_level_number * 2;
        
        }

    }
    //if (isAlive()){
    //    // if in rest state {
    //    //increment ticks;
    //    //return;}
    //    if(leave_the_oil_field == true){
    //        if (getX()==60 && getY()==60){
    //            setAlive(false);
    //            return;
    //        }
    //        else if(getX()!=60 && !blockedByIceOrBoulder(getX(), getY(), studW)){
    //            //turn direction based on Q
    //            moveTo(getX() + 1, getY());
    //            return;
    //        }
    //        else if(getY()!=60 && !blockedByIceOrBoulder(getX(), getY(), studW)){
    //            //turn direction based on Q
    //            moveTo(getX(), getY() + 1 );
    //            return;
    //        }
    //    }
    //    else if(is4Away(studW) == "IceMan" && isFacingIceMan(getDirection(), studW)){
    //        //if hasn't shouted in 15 ticks
    //        getWorld()->playSound(SOUND_PROTESTER_YELL);
    //        //iceMan -2 hitpoints
    //        //update state, wont shout in the next 15 ticks
    //    }
    //    else if (iceManisInSight(getX(), getY(), studW) && is4Away(studW) == "IceMan"&& !blockedByIceOrBoulder(getX(), getY(), studW) && !isFacingIceMan(getDirection(), studW)){
    //            setFacingIceMan(getDirection(), studW);
    //            //            //moveTo(getX(), getY() + 1) OR moveTo(getX() + 1, getY())
    //            //            //moveTo(getX(), getY() - 1) OR moveTo(getX() - 1, getY())
    //            numSquaresToMoveInCurrentDirection = 0;
    //            return;
    //        }
    //    else if(!iceManisInSight(getX(), getY(), studW)){
    //        numSquaresToMoveInCurrentDirection--;
    //        if(numSquaresToMoveInCurrentDirection <= 0){
    //            //pick random direction that is not blocked by boulders or Ice
    //            //pick random number 8 <= numSquaresToMoveInCurrentDirection <= 60
    //            //take 1 step in that direction
    //        }
    //    }
    //    //else if isAtFork && canMove1Perpindicular && !madePerpTurn in 200 ticks
    //        //pick which 2 directions
    //        //pick whichevr 2 directions if both are good pick one randomly
    //        //set direction to new direction
    //    else if (blockedByIceOrBoulder(getX(), getY(), studW)){
    //        numSquaresToMoveInCurrentDirection = 0;
    //        return;
    //        //pick new dirction in nonresting tick
    //    }
    //
    //}
    //return;

//Prop


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
        //        else if (getState() == proPickUp && is3Away(studW) == "Protester" || is3Away(studW) == "Hard Protester"){
        //            //&& if pick-up BY PROTESTER able
        //            setAlive(false);
        //            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        //            //bribe protester
        //            studW->increaseScore(25);}
        //    }

//DOES INCREMENT SCORE AND DISAPPEARS IF FULL PNG BUT SOMETIMES DOESNT WORK W PARTIAL




//oil

void Oil::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        if (!isVisible() && is4Away(studW) == "IceMan"){
            setVisible(true);
            return;
        }
        else if (is3Away(studW) == "IceMan"){//is pick up able by iceMan
            setAlive(false);
            getWorld()->playSound(SOUND_FOUND_OIL);
            man->setOil(1);
            studW->increaseScore(1000);
        }
    }
    return;
}

//Squirt
int x = 0;
void Squirt::doSomething(){
    if (x == 5) {
        x = 0;
        moveTo(getWorld()->getIceMan()->getX()-4, getWorld()->getIceMan()->getY());
    }
    x++;
}


//sonar kit

void SonarKit::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        if(is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            man->setSonar(1);
            studW->increaseScore(75);
        }
        //if (timeEllapsed){
        //setAlive(false);
        //}
        else{
            cout << "hello im alive!" << endl;
        }
    }
    return;
}

//water pool
void WaterPool::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        if (is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
            if (man->getSquirt() >= 5){
                return;
            }
            man->setWater(1);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            //increment 5 water squirts to ice man's inventory
            studW->increaseScore(100);
        }
        //if (timeEllapsed){
        //setAlive(false);
        //}
    }
    return;
}
