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


//Iceman
//bool IceMan::isAlive(){
////    if (m_lives == 0){
////        return false;//ends level, terminates game
////    }
//    return true;
//}

void IceMan::doSomething(){
    if (isAlive()){
        overlap(studW);//dig when overlap with ice
        int a;
        if (getWorld()->getKey(a) == true) { //player pressed key
            bool blocked = false;
            switch (a){
                case KEY_PRESS_ESCAPE:
                    //isDead();
                    break;
                case KEY_PRESS_SPACE:
                    //shoot water
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);//play shoot water sound
                case KEY_PRESS_LEFT:
                    if (getDirection() != left){
                        setDirection(left); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{

                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getX()+3 == getX()-1 && abs(getY() - a->getY()) < 4) {
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
                    else{
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getY() == getY()+4 && abs(getX() - a->getX()) < 4) {
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
                        for (Actor* a : getWorld()->getCharacterList()) {
                            if (a->getID() == IID_BOULDER && a->getY()+3 == getY()-1 && abs(getX() - a->getX()) < 4) {
                                blocked = true;//if blocked by boulder dont move
                            }
                        }
                        if (!blocked) {//move if nothing blocks it
                            moveTo(getX(), getY() - 1);
                        }
                        //}
                    }
                    break;
//                case KEY_PRESS_Z:
//                    if (sC < 0){
//                        //use charge,
//                        playSound(SOUND_SONAR);
//                        sC--;
//                        //nearby (radius of 12) props set to visible
//                    }
                    //break;
                    case KEY_PRESS_TAB:
                    if (gold < 0){
                        //drop gold for 100 ticks,
                        gold--;
                       // and set to temp state, pick up-able by pro and hpro
                        }
                        break;
                    // }
            }
        }
    }
    else{
        return;
    }

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

//NOT DONE YET

void IceMan::addOrSubGold(string x){
    if (x == "+"){
        gold++;
    }
    else if (x == "-"){
        gold--;
    }
}
void IceMan::getAnnoyed(int dAmage){
    //if is shouted at
    dAmage--;//decrement IceMans damage by 2
    dAmage--;
        //if damage ==0
        //setState("dead");
        //playSound(SOUND_PLAYER_GIVE_UP);
}

//protester
void Protester::doSomething(){
    //if (isAlive()){
    //      if in rest state { do nothing}
    //else{
    // return
    //}
}

//Prop

string Prop::is4Away(StudentWorld* world){
    if (isAlive()){
//        int x = 4;
//        int y = 4;
        for (Actor* a : world->getCharacterList()) {
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

string Prop::is3Away(StudentWorld* world){
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
            
//            radius = sqrt(pow(x - getX(), 2) + pow(y - getY(), 2));
            
////            int square = pow(m_iceman -> getX() - x, 2) + pow(m_iceman -> getY() - y, 2);
//            if (world->getMap().at(radius) == nullptr){
//             //world->pl
//            }
            if (world->getMap().at(getY() + y).at(getX() + x) == nullptr) {
                if (a->getID() ==IID_PLAYER && a->getX() <= getX() + 3 && a->getY() <= getY() + 3){// down and right of gold
                    return "IceMan";
                }
                else if (a->getID() ==IID_PLAYER && a->getX() >= getX() -3 && a->getY()  >= getY() -3 ){//up and left of gold
                    return "IceMan";
                }
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
        else if (getState() == proPickUp && is3Away(studW) == "Protester" || is3Away(studW) == "Hard Protester"){
            //&& if pick-up BY PROTESTER able
            setAlive(false);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            //bribe protester
            studW->increaseScore(25);}
    }
    return;
}
//DOES INCREMENT SCORE AND DISAPPEARS IF FULL PNG BUT SOMETIMES DOESNT WORK W PARTIAL

//MUST INCREMENT GOLD AND LET IT SO THAT IF IT ONLY TOUCHES A LITTLE IT SHOULD INCREMENT
bool Gold::isPickUpAble(Actor IceMan,StudentWorld* world){
    //IceMan::addOrSubGold("+");
//    if (actor == "Iceman" || actor == "IceMan"){
//        
//    }
//    else if (actor == "Protester"){
//        
//    }
    return false;
}


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
            //increment iceMan's oil inventory
        }
    }
    return;
}



