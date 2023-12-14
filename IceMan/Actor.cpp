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
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                setAlive(false);
                //cout << "I HIT THE PLAYER"
            }
        }
        if (a->getID() == IID_PLAYER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                world->decLives();
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                a->setAlive(false);
            }
        }
        if (a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER && (abs(getY() - a->getY()) < 4)) {
            if (abs(getX() - a->getX()) < 4) {
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                studW->increaseScore(500);
                a->setAlive(false);
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
            getWorld()->increaseScore(75);
            return;
        }
        else if (a->getID() == IID_WATER_POOL && a->is3Away(studW)=="IceMan")
        {
            waterSq++;//picks up water
            getWorld()->increaseScore(100);
            return;
        }
    }
           
}

//NOT DONE YET
void IceMan::checkAnnoyed(){
    if(getHitpoints() == 0){
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        getWorld()->decLives();
    }
}

//protester

bool followingPath = false;
void Protester::doSomething(){
    if (isAlive()){
        if (ticksToWait > 0){
            ticksToWait--;
            return;
        }
        det15();
        if (ticksToWait==0 && !outOfField(getX(), getY(), getDirection())){
            if(leave_the_oil_field == true){
                if(sqrt(pow(60 - getX(), 2) + pow(60 - getY(), 2) <= 2)){
                    setDirection(right);
                    setAlive(false);
                    return;
                }
                
                else if(getX()!=60 && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                    
                    //turn direction based on Q
                    if (getDirection() != right){
                        setDirection(right);
                    }
                    moveTo(getX() + 1, getY());
                    
                    return;
                    
                }
                else if(getY()!=60 && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                    //turn direction based on Q
                    if (getDirection() != up){
                        setDirection(up);
                    }
                    moveTo(getX(), getY() + 1 );
                    //reset ticks to wait
                    return;
                }
            }
            else if(is4Away(studW) == "IceMan"){// && isFacingIceMan(getDirection(), studW)
                if (shoutLast15 == 0){
                    yell(getX(), getY());
                    ticksToWait = 10;
                    reset15();
                }
                
            }
            else if (iceManisInSight(getX(), getY(), studW) && is4Away(studW) == "Greater IceMan" && !isFacingIceMan(getDirection(), studW)){
                    setFacingIceMan(getDirection(), studW);
                //getWorld()->moveToShortPath();
               moveOne(getX(), getY(), getDirection());
                tryGold(getX(), getY());
                isAnnoyed();
                    numSquaresToMoveInCurrentDirection = 0;
                reset15();
                ticksToWait = 10;
                    return;
                
            }
            else if(!iceManisInSight(getX(), getY(), studW) && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                numSquaresToMoveInCurrentDirection--;
                if(numSquaresToMoveInCurrentDirection <= 0){
                    //pick random direction that is not blocked by boulders or Ice
                    numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
                    ticksToWait = 10;
                    reset15();
                    //take 1 step in that direction
                    moveOne(getX(), getY(), getDirection());
                    tryGold(getX(), getY());
                    isAnnoyed();
                    return;
                }
            }
            //else if isAtFork && canMove1Perpindicular && !madePerpTurn in 200 ticks
            //pick which 2 directions
            //pick whichevr 2 directions if both are good pick one randomly
            //set direction to new direction
            else if (getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                numSquaresToMoveInCurrentDirection = 0;
                reset15();
                ticksToWait = 10;
                isAnnoyed();
                return;
                //reset ticks to wait
                //pick new dirction in nonresting tick
            }
            
        }
        
   }
    return;
}

void Protester::isAnnoyed(){
    //boulder already covered
    //if gets sprayed by iceMan
    if (getHitpoints() <= 0){
        studW->increaseScore(100);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        leave_the_oil_field = true;
        return;
    }
}

bool Protester::yell(int x, int y){//disabled for testingg
    cout << "yell!!!" << endl;
//    getWorld()->playSound(SOUND_PROTESTER_YELL);
//   IceMan* man = getWorld()->getIceMan();
//  man->setHitpoints(-2);
    return true;
}

void Protester::tryGold(int x, int y){
    if (getWorld()->pickUpGold(getX(), getY())){
            cout << "Im walking back now!!" << endl;
            studW->increaseScore(25);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            leave_the_oil_field = true;
    }
    }


void Protester::moveOne(int x, int y, Direction d){
    switch (d){
        case up:
            if (!outOfField(x, y + 1, d) && !getWorld()->blockedbyRocksOrIce(x, y + 1, getDirection())){
                moveTo(getX(), getY() + 1);
                break;
            }
            else if (outOfField(x, y + 1, d) || getWorld()->blockedbyRocksOrIce(x, y + 1, getDirection())){
                setDirection(down);
                return;
            }
            break;
        case down:
            if (!outOfField(x, y -1, d) && !getWorld()->blockedbyRocksOrIce(x, y - 1, getDirection())){
                moveTo(getX(), getY() - 1);
                break;
            }
            else if (outOfField(x, y - 1, d) || getWorld()->blockedbyRocksOrIce(x, y - 1, getDirection())){
                setDirection(up);
                return;
                break;
            }
            break;
        case right:
            if (!outOfField(x + 1, y, d) && !getWorld()->blockedbyRocksOrIce(x +1, y, getDirection())){
                moveTo(getX() + 1, getY());
                break;
            }
            else if (outOfField(x + 1, y, d) || getWorld()->blockedbyRocksOrIce(x + 1, y, getDirection())){
                setDirection(left);
                return;
                break;
            }
            break;
        case left:
            if (!outOfField(x - 1, y, d) && !getWorld()->blockedbyRocksOrIce(x - 1, y, getDirection())){
                moveTo(getX() - 1, getY());
                break;
            }
            else if (outOfField(x - 1, y, d) || getWorld()->blockedbyRocksOrIce(x - 1, y, getDirection())){
                setDirection(right);
                return;
                break;
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
            if (yI < y){ //check if ice in way!
                //delete man;
                return true;
            }
            
        }
        else if (y == yI) { //if they are on same column
            if (xI < x || xI > x){
                //delete man;
                return true;
            }
        }
    }
    //delete man;
    return false;
}

//Hardcore Protester

void HardcoreProtester::doSomething(){//NOT COMPLETE
    if (isAlive()){
        if (ticksToWait > 0){
            ticksToWait--;
            return;
        }
        det15();
        if (ticksToWait==0 && !outOfField(getX(), getY(), getDirection())){
            if(leave_the_oil_field == true){
                if(sqrt(pow(60 - getX(), 2) + pow(60 - getY(), 2) <= 2)){
                    setDirection(right);
                    setAlive(false);
                    return;
                }
                
                else if(getX()!=60 && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                    
                    //turn direction based on Q
                    if (getDirection() != right){
                        setDirection(right);
                    }
                    moveTo(getX() + 1, getY());
                    
                    return;
                    
                }
                else if(getY()!=60 && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                    //turn direction based on Q
                    if (getDirection() != up){
                        setDirection(up);
                    }
                    moveTo(getX(), getY() + 1 );
                    //reset ticks to wait
                    return;
                }
            }
            else if(is4Away(studW) == "IceMan"){// && isFacingIceMan(getDirection(), studW)
                if (shoutLast15 == 0){
                    yell(getX(), getY());
                    ticksToWait = 10;
                    reset15();
                }
            }
            else if (iceManisInSight(getX(), getY(), studW) && is4Away(studW) == "Greater IceMan" && !isFacingIceMan(getDirection(), studW)){
                    setFacingIceMan(getDirection(), studW);
                //getWorld()->moveToShortPath();
               moveOne(getX(), getY(), getDirection());
                tryGold(getX(), getY());
                isAnnoyed();
                    numSquaresToMoveInCurrentDirection = 0;
                reset15();
                ticksToWait = 10;
                    return;
                
            }
            else if(!iceManisInSight(getX(), getY(), studW) && !getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                numSquaresToMoveInCurrentDirection--;
                if(numSquaresToMoveInCurrentDirection <= 0){
                    //pick random direction that is not blocked by boulders or Ice
                    numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
                    ticksToWait = 10;
                    reset15();
                    //take 1 step in that direction
                    moveOne(getX(), getY(), getDirection());
                    tryGold(getX(), getY());
                    isAnnoyed();
                    return;
                }
            }
            //else if isAtFork && canMove1Perpindicular && !madePerpTurn in 200 ticks
            //pick which 2 directions
            //pick whichevr 2 directions if both are good pick one randomly
            //set direction to new direction
            else if (getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())){
                numSquaresToMoveInCurrentDirection = 0;
                reset15();
                ticksToWait = 10;
                isAnnoyed();
                return;
                //reset ticks to wait
                //pick new dirction in nonresting tick
            }
            
        }
        
   }
    return;
    }

void HardcoreProtester::tryGold(int x, int y){
    if (getWorld()->pickUpGold(getX(), getY())){
        if (getGold() == 2){
            cout << "Im MEGA RICHHH!!" << endl;
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            studW->increaseScore(50);
            leave_the_oil_field = true;
        }
        else{
            cout << "youre too cheap" << endl;
            setGoldInv(1);
        }
    }
}



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
    if (getWorld()->blockedbyRocksOrIce(getX(), getY(), getDirection())) {
        setAlive(false);
        }
    if (distance == 0){
        setAlive(false);
    }
    //if protester in the way
    //hurt then set to dead
    if (distance > 0){
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
        detD();
    }
}


//sonar kit

void SonarKit::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        deT();
        if(is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            man->setSonar(1);
            studW->increaseScore(75);
        }
        else if (ticksToWait == 0){
            setAlive(false);
        }
    }
    return;
}

//water pool
void WaterPool::doSomething(){
    IceMan* man = getWorld()->getIceMan();
    if (isAlive()){
        deT();
        if (is3Away(studW) == "IceMan"){//&& !timeEllapsed
            setAlive(false);
            if (man->getSquirt() >= 5){
                return;
            }
            man->setWater(5);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            studW->increaseScore(100);
        }
        else if (ticksToWait == 0){
        setAlive(false);
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
