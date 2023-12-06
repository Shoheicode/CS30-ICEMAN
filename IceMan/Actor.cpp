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
    if (!isFacingIceMan(getDirection(), studW)){
        
    }
}
string Actor::is4Away(StudentWorld* world){
//    IceMan* man = world->getIceMan();
//    Protester* proP = world->getProtester(); USE THESE
    if (isAlive()){
        IceMan* man = world->getIceMan();
        int x = man->getX();
        int y = man->getY();
        double radius = sqrt(pow(x - getX(), 2) + pow(y - getY(), 2));
        
        
        for (Actor* a : world->getCharacterList()) {
            //cout << a->getID() << endl;
            if (world->getMap().at(getY()).at(getX()) == nullptr) {
                if (a->getID() ==IID_PLAYER && a->getX() <= getX() + 4 && a->getY() <= getY() + 4){
                    return "IceMan";
                }
                else if (a->getID() ==IID_PROTESTER && a->getX() <= getX() + 3 && a->getY() <= getY() + 3){
                    return "Protester";
                }
                else if (a->getID() ==IID_HARD_CORE_PROTESTER && a->getX() <= getX() + 3 && a->getY() <= getY() + 3){
                    return "Hard Protester";
                }
            }
        }
    }
    return "nothing near";
}

string Actor::is3Away(StudentWorld* world){
    if (isAlive()){
        int x = 3;
        int y = 3;
        //rad <=3
        for (Actor* a : world->getCharacterList()) {
            int xI = 3;
            int yI = 3;
            double radius;
            //x = icemans get(X)
            //y = icemans get(Y)
            
           
            
            if (world->getMap().at(getY() + y).at(getX() + x) == nullptr) {
                radius = sqrt(pow(a->getX() - getX(), 2) + pow(a->getY() - getY(), 2));
                if (a->getID() ==IID_PLAYER && radius <=3){// down and right of gold
                    return "IceMan";
                }
                //else if (a->getID() ==IID_PLAYER && radius <=3 ){//up and left of gold
                //    return "IceMan";
                //}
                else if (a->getID() ==IID_PROTESTER && a->getX() <= getX() + 3 && a->getY() <= getY() + 3){
                    return "Reg Protester";
                }
                else if (a->getID() ==IID_HARD_CORE_PROTESTER && a->getX() <= getX() + 3 && a->getY() <= getY() + 3){
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

void IceMan::doSomething(){
    Boulder* rock = getWorld()->getBoulder();
    if (isAlive()){
        overlap(studW);//dig when overlap with ice
        isInRange(studW);
        int a;
        if (getWorld()->getKey(a) == true) { //player pressed key
            bool blocked = false;
            switch (a){
                case KEY_PRESS_ESCAPE:
                    getWorld()->decLives();
                    setAlive(false);
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
                            if (rock->getX()+3 == getX()-1 && abs(getY() - rock->getY()) < 4) {
                                blocked = true;//if blocked by boulder dont move
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
                            if (rock->getX() == getX() + 4 && abs(getY() - rock->getY()) < 4) {//if blocked by boulder dont move
                                blocked = true;
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
                            if (rock->getY() == getY()+4 && abs(getX() - rock->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
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
                            if (rock->getY()+3 == getY()-1 && abs(getX() - rock->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
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
            if(world->pickUpgetStudGold()){
                gold++;//picks up gold
                return;
            }
            else if(world->pickUpgetStudOil()){
                oil++;//picks up oil
                return;
            }
            else if(world->pickUpgetStudSonar()){
                sonarC++;//picks up sonar
                return;
            }
            else if(world->pickUpgetStudWater()){
                if (waterSq == 5){
                    return;
                }
                waterSq++;//picks up water if not full
                return;
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
    if (isAlive()) {
        cout << "PROTESTOR IS ALIVE BABBBBYYY" << endl;
        if (leave_the_oil_field == true) {
            if (getX() == 60 && getY() == 60) {
                setAlive(false);
                return;
            }
            else if (getX() != 60 && !blockedByIceOrBoulder(getX(), getY(), studW)) {
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
}

bool Protester::blockedByIceOrBoulder(int x, int y, StudentWorld* world){
    Boulder* rock = world->getBoulder();
    Ice* coldRock = world->getIce();
    int xI = coldRock->getX();
    int yI = coldRock->getY();
    int xB = rock->getX();
    int yB = rock->getY();
     if (x + 1 == xB ||x + 1 == xI || x - 1 == xB ||x - 1 == xI){
         delete rock;
         delete coldRock;
         return true;
    }
    else if (y + 1 == xB ||y + 1 == xI ||y - 1 == xB ||y - 1 == xI){
        delete rock;
        delete coldRock;
        return true;
    }
    delete rock;
    delete coldRock;
    return false;
}

bool Protester::iceManisInSight(int x, int y, StudentWorld* world){
    IceMan* man = world->getIceMan();
    int xI = man->getX();
    int yI = man->getY();
    if (x == xI){ //if they are on same row
        delete man;
        return true;
    }
    else if (y == yI){ //if they are on same column
        delete man;
        return true;
    }
    delete man;
    return false;
}

//Hardcore Protestor

void HardcoreProtester::doSomething() {

}

//Prop


//gold

void Gold::doSomething(){
    if(isAlive()){
            if (!isVisible() && is4Away(studW) == "IceMan"){
                setVisible(true);//appear on screen
                return;}
            else if (getState() == icePickUp && is3Away(studW)== "IceMan"){
                setAlive(false);
                getWorld()->playSound(SOUND_GOT_GOODIE);
                studW->increaseScore(10);//increment score by 10
                //increment iceMan's gold  by 1
            }
            
            
           
        }
        //        else if (getState() == proPickUp && is3Away(studW) == "Protester" || is3Away(studW) == "Hard Protester"){
        //            //&& if pick-up BY PROTESTER able
        //            setAlive(false);
        //            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        //            //bribe protester
        //            studW->increaseScore(25);}
        //    }
        return;
    }

//DOES INCREMENT SCORE AND DISAPPEARS IF FULL PNG BUT SOMETIMES DOESNT WORK W PARTIAL




//oil

void Oil::doSomething(){
    if (isAlive()){
        if (!isVisible() && is4Away(studW) == "IceMan"){
            setVisible(true);
            return;
        }
        else if (is3Away(studW) == "IceMan"){//is pick up able by iceMan
            setAlive(false);
            getWorld()->playSound(SOUND_FOUND_OIL);
            studW->increaseScore(1000);
        }
    }
    return;
}

//Squirt
void Squirt::setDirectiontoIceMan(Direction d, StudentWorld* world){
    IceMan* man = world->getIceMan();
        if (d != man->getDirection()){
            Direction iceManDirection = man->getDirection();
            setDirection(iceManDirection);
        }
    delete man;
}


//sonar kit

void SonarKit::doSomething(){
    if (isAlive()){
        if(is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            //increment ice man inventory
            studW->increaseScore(75);
        }
        //if (timeEllapsed){
        //setAlive(false);
        //}
    }
    return;
}

//water pool
void WaterPool::doSomething(){
    if (isAlive()){
        if (is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
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

