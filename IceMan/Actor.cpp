#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
bool Actor::outOfField(int x, int y, Actor::Direction d){
    switch(d){
        case Actor::up:
            if(y >= 60){
                return true;
            }
            break;
        case Actor::down:
            if(y <= 0){
                return true;
            }
            break;
        case Actor::left:
            if(x <= 0){
                return true;
            }
            break;
        case Actor::right:
            if(x >= 61){
                return true;
            }
            break;
        case Actor::none:
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
        //if IceMan overlaps with Ice objects{
        //calls StudentWorld object getWorld->destroyIce..?
        //destroys Ice from the 4x4 area occupied by the Iceman (from x, y to x+3,y+3 inclusive)
        //playSound(SOUND_DIG);
        //}
        overlap(studW);
        int a;
        
        if (getWorld()->getKey(a) == true) { //player pressed key
            bool blocked = false;
            switch (a){
                case KEY_PRESS_ESCAPE:
                    //isDead();
                    break;
                case KEY_PRESS_SPACE:
                    //shoot water
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
                                blocked = true;
                            }
                        }
                        if (!blocked) {
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
                            if (a->getID() == IID_BOULDER && a->getX() == getX() + 4 && abs(getY() - a->getY()) < 4) {
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
                                blocked = true;
                            }
                        }
                        if (!blocked) {
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
                                blocked = true;
                            }
                        }
                        if (!blocked) {
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
    bool digging = false;
    if (getY() <= 59) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (getY()+y <= 59) {
                    if (world->getMap().at(getY() + y).at(getX() + x) != nullptr) {
                        Ice* temp = world->getMap().at(getY() + y).at(getX() + x);
                        world->getMap().at(getY() + y).at(getX() + x) = nullptr;
                        delete temp;
                        digging = true;
                    }
                }
            }
        }
    }
    if (digging) {
        world->playSound(SOUND_DIG);
    }
    //Boulder

}

void IceMan::getAnnoyed(int dAmage){
    //if is shouted at
    dAmage--;
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
                setAlive(false);
                a->setAlive(false);
            }

        }
    }
}