#include "ofApp.h"
#include "Emitter.hpp"
#include "EnemyEmitter.hpp"
#include "Sprite.hpp"
#include "GameController.hpp"
//----------------------------------------------------------------------------------
//
// CS 134 - Game Design Programming Project 1 - Part 2
//
// This code demonstrates the use of Vintage 2D Shooter Arcade Game.
//
// It has the following functionality as of 03/10/2019:
//
// 1) Game starts with the keyboard event of pressing the "Spacebar".
// 2) The player can move in any direction (up, down, left, right) or by using the mouse drag.
// 3) The spacebar is also used to shoot missiles. Holding the spacebar causes the missiles to
//    fire automatically. The rate and direction of the missile is also controlled by a GUI slider.
// 4) The firing of missiles emits a burst sound effect.
// 5) The visual design of the game has bee changed. i.e. Background Image, Background Sound, Player Sprite.
// 6) Pressing the "I" key hides/unhides the instructions menu.
// 7) Pressing the "H" key hides/unhides the help menu (GUI slider).
// 8) There are multiple emitters that are spawned and can be controlled using the slider.
// 9) Detection of collisions is working where it will remove both enemy and bullet sprite on collision.
// 10) Sound has been added for firing missiles and collisions.
// 11) Modified the velocity vector to randomize the movement of the enemy sprites.
// 12) The game design/look has been updated.
// 13) There are certain levels in the game.
// 14) After reaching certain level in the game, the player will receive some "helpers" to fight off the invaders!
//
//
//  Author(s):
//             Parth Patel (013705718) - CS 134 SJSU
//
// Audio Credit:
//             https://soundcloud.com/spaceinvaderspaceinvader/street-traffic
//             http://www.classicgaming.cc/classics/space-invaders/sounds

//--------------------------------------------------------------
void ofApp::setup(){
    gameState = GameController::GameStates::MAIN_MENU;
	ofSetVerticalSync(true);
    
    score = 0;
    numEmitters = 3;
    numMinions = 2;
    
    margin = 300;
    emitterWidth = 50;
    winsize = ofGetWindowSize();
    spacing = (winsize.x-emitterWidth - margin*2) / (numEmitters - 1);
    x = emitterWidth / 2 + margin;
    
    minionMargin = 100;
    minionEmitterWidth = 20;
    minionSpacing = (winsize.x-minionEmitterWidth - minionMargin*2) / (numMinions - 1);
    xMinionDist = minionEmitterWidth / 2 + minionMargin;

	player = new Emitter( new SpriteSystem() );
    moveDir = MoveStop;
	imageLoaded = false;

    playershipImage.load("images/player_2.png");
    minionsImage.load("images/player.png");
    enemyshipImage.load("images/enemy_2.png");
    playerBulletImage.load("images/player_bullet_2.png");
    minionBulletImage.load("images/minion_bullet.png");
//    backgroundImage.load("images/background_04.jpg");
    font.load("fonts/SNES Italic.ttf", 30);
    gameplaySong.load("music/gameplaySong.mp3");
    menuScreenBg.load("videos/galaxy_bg_2.mp4");
    sfxEnemyCollision.load("music/enemyCollision.wav");
    
    background.load("images/starBg.jpg");
    background2.load("images/starBg.jpg");
    background.resize(ofGetWindowWidth(), ofGetWindowHeight());
    background2.resize(ofGetWindowWidth(), ofGetWindowHeight());
    
    bgPos1 = ofVec2f(0, 0);
    bgPos2 = ofVec2f(0, -ofGetWindowHeight());
    
    playershipImage.resize(115, 80);
    minionsImage.resize(62, 48);
    enemyshipImage.resize(90, 65);
    playerBulletImage.resize(40, 80);
    minionBulletImage.resize(20, 40);
    
    turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
    gravityForce = new GravityForce(ofVec3f(0, -20, 0)); //-10
    radialForce = new ImpulseRadialForce(1000.0);
    radialForce->setHeight(-1.0, 1.0);
    
    boosterTurbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
    boosterGravityForce = new GravityForce(ofVec3f(0, 10, 0)); //-10
    boosterRadialForce = new ImpulseRadialForce(1000.0);
    
//    minionBoosterTurbForce1 = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
//    minionBoosterGravityForce1 = new GravityForce(ofVec3f(0, 10, 0)); //-10
//    minionBoosterRadialForce1 = new ImpulseRadialForce(1000.0);
//
//    minionBoosterTurbForce2 = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
//    minionBoosterGravityForce2 = new GravityForce(ofVec3f(0, 10, 0)); //-10
//    minionBoosterRadialForce2 = new ImpulseRadialForce(1000.0);
    
    explosion.sys->addForce(turbForce);
    explosion.sys->addForce(gravityForce);
    explosion.sys->addForce(radialForce);
    
    boosterParticle.sys->addForce(boosterTurbForce);
    boosterParticle.sys->addForce(boosterGravityForce);
    boosterParticle.sys->addForce(boosterRadialForce);
    
//    minionBoosterParticle1.sys->addForce(minionBoosterTurbForce1);
//    minionBoosterParticle1.sys->addForce(minionBoosterGravityForce1);
//    minionBoosterParticle1.sys->addForce(minionBoosterRadialForce1);
//
//    minionBoosterParticle2.sys->addForce(minionBoosterTurbForce2);
//    minionBoosterParticle2.sys->addForce(minionBoosterGravityForce2);
//    minionBoosterParticle2.sys->addForce(minionBoosterRadialForce2);
    
    explosion.setVelocity(ofVec3f(200, 200, 0));
    explosion.setOneShot(true);
    explosion.setEmitterType(RadialEmitter);
    explosion.setGroupSize(20);
    explosion.setParticleRadius(5);
    explosion.setLifespan(1);
    explosion.setPosition(ofVec2f(ofGetWindowWidth()/2, ofGetWindowHeight()/2));
    
    boosterParticle.setVelocity(ofVec3f(0, 1000, 0));
    boosterParticle.setOneShot(false);
    boosterParticle.setEmitterType(DirectionalEmitter);
    boosterParticle.setGroupSize(100);
    boosterParticle.setParticleRadius(10);
    boosterParticle.setLifespan(0.1);
    boosterParticle.setPosition(ofVec2f(ofGetWindowWidth()/2, ofGetWindowHeight()/2));
    boosterParticle.setRate(20);
    
//    minionBoosterParticle1.setVelocity(ofVec3f(0, 1000, 0));
//    minionBoosterParticle1.setOneShot(false);
//    minionBoosterParticle1.setEmitterType(DirectionalEmitter);
//    minionBoosterParticle1.setGroupSize(100);
//    minionBoosterParticle1.setParticleRadius(10);
//    minionBoosterParticle1.setLifespan(0.1);
//    minionBoosterParticle1.setPosition(ofVec2f(ofGetWindowWidth()/2, ofGetWindowHeight()/2));
//    minionBoosterParticle1.setRate(20);
//
//    minionBoosterParticle2.setVelocity(ofVec3f(0, 1000, 0));
//    minionBoosterParticle2.setOneShot(false);
//    minionBoosterParticle2.setEmitterType(DirectionalEmitter);
//    minionBoosterParticle2.setGroupSize(100);
//    minionBoosterParticle2.setParticleRadius(10);
//    minionBoosterParticle2.setLifespan(0.1);
//    minionBoosterParticle2.setPosition(ofVec2f(ofGetWindowWidth()/2, ofGetWindowHeight()/2));
//    minionBoosterParticle2.setRate(20);
    
    for (int i = 0;  i < numEmitters; i++) {
        EnemyEmitter *emit = new EnemyEmitter(new SpriteSystem());
        emit->setPosition(ofVec3f(x, 0, 0));
        emit->setVelocity(ofVec3f(0, 400, 0));
        emit->setLifespan(5000);
        emit->setRate(1);
        emit->drawable = false;                // make emitter itself invisible
        emit->setChildSize(20, 20);
        emit->setChildImage(enemyshipImage);
        x += spacing;
        invaders.push_back(emit);
        emit->start();
    }
    
    player->setImage(playershipImage);
    player->setChildImage(playerBulletImage);
    player->setChildSize(40, 80);
    player->setPosition(ofVec3f(ofGetWindowWidth()/2, ofGetWindowHeight()-10, 0));
    player->setVelocity(ofVec3f(0, -1000, 0));
    player->setRate(3);
    player->setLifespan(500);
    
    for (int i = 0; i < numMinions; i++) {
        Emitter *emit = new Emitter(new SpriteSystem());
        emit->setImage(minionsImage);
        emit->setPosition(ofVec3f(xMinionDist, player->getPosition().y, player->getPosition().z));
        emit->setVelocity(ofVec3f(0, -1000, 0));
        emit->setLifespan(500);
        emit->setRate(3);
        emit->setChildSize(20, 40);
        emit->setChildImage(minionBulletImage);
        xMinionDist += minionSpacing;
        minions.push_back(emit);
    }
    
	guiPlayer.setup();
	guiPlayer.add(playerRate.setup("rate", 10, 1, 10));
	guiPlayer.add(playerLife.setup("life", 2, .1, 10));
	guiPlayer.add(playerVelocity.setup("velocity", ofVec3f(0, -1000, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
    guiPlayer.setName("Player Parameters");
    
    guiInvader.setup();
    guiInvader.add(invaderRate.setup("rate", 0.5, 0.001, 100));
    guiInvader.add(invaderLife.setup("life", 2, .1, 10));
    guiInvader.setPosition(ofPoint(ofGetScreenWidth()/2+350, 0));
    guiInvader.setName("Enemy Parameters");

	bHide = false;
    
    gameplaySong.play();
    gameplaySong.setLoop(true);
    gameplaySong.setVolume(0.2);
    menuScreenBg.play();
    menuScreenBg.setLoopState(OF_LOOP_NORMAL);
    
    // set the delay for 10 seconds to start the game
    sucessTimeDelta = 10000;
    
    boosterParticle.start();
}

// Start the waves of invaders
//--------------------------------------------------------------
void ofApp::levelOne() {
    isLevelOne = true;
    isLevelTwo = false;
    isLevelThree = false;
    for (int i = 0; i < invaders.size(); i++) {
        invaders[i]->setRate(invaderRate);
        ofVec3f v = invaders[i]->velocity;
        invaders[i]->setVelocity(ofVec3f(ofRandom(-v.y /2, v.y /2), v.y, v.z));
    }
}

void ofApp::levelTwo() {
    isLevelOne = false;
    isLevelTwo = true;
    isLevelThree = false;
    for (int i = 0; i < invaders.size(); i++) {
        invaders[i]->setRate(invaderRate * 3);
        ofVec3f v = invaders[i]->velocity;
        invaders[i]->setVelocity(ofVec3f(ofRandom(-v.y /2, v.y /2), v.y, v.z));
    }
}

void ofApp::levelThree() {
    isLevelOne = false;
    isLevelTwo = false;
    isLevelThree = true;
    for (int i = 0; i < invaders.size(); i++) {
        invaders[i]->setRate(invaderRate * 10);
        ofVec3f v = invaders[i]->velocity;
        invaders[i]->setVelocity(ofVec3f(ofRandom(-v.y /2, v.y /2), v.y, v.z));
    }
}

//--------------------------------------------------------------
void ofApp::update() {
//    if (ofGetWindowWidth() < xPos) {
//        xPos -= 2;
//    }
    
    player->setRate(playerRate);
    player->setLifespan(playerLife * 1000);    // convert to milliseconds
    player->setVelocity(ofVec3f(playerVelocity));
    actualTime = ofGetElapsedTimeMillis();
    
    boosterParticle.setPosition(ofVec3f(player->getPosition().x, player->getPosition().y, 0));
//    minionBoosterParticle1.setPosition(ofVec3f(500, 500, 0));
//    minionBoosterParticle2.setPosition(ofVec3f(200, 200, 0));
//    boosterParticle.sys->reset();
    
//    if (isLevelThree) {
//        minionBoosterParticle1.start();
//        minionBoosterParticle2.start();
//    }
    
    if (gameOver) {
        player->stop();
        player->lifespan = 0;
        player->drawable = false;
        minions[0]->stop();
        minions[1]->stop();
        minions[0]->drawable = false;
        minions[1]->drawable = false;
        boosterParticle.stop();
//        minionBoosterParticle1.stop();
//        minionBoosterParticle2.stop();
        for (int i = 0; i < invaders.size(); i++) {
            invaders[i]->stop();
        }
        player->sys->sprites.clear();
        player->sys->sprites.clear();
        minions[0]->sys->sprites.clear();
        minions[1]->sys->sprites.clear();
        invaders.clear();
    }
    
    if (win) {
        player->stop();
        player->lifespan = 0;
        player->drawable = false;
        minions[0]->stop();
        minions[1]->stop();
        minions[0]->drawable = false;
        minions[1]->drawable = false;
        boosterParticle.stop();
        //        minionBoosterParticle1.stop();
        //        minionBoosterParticle2.stop();
        for (int i = 0; i < invaders.size(); i++) {
            invaders[i]->stop();
        }
        player->sys->sprites.clear();
        minions[0]->sys->sprites.clear();
        minions[1]->sys->sprites.clear();
        invaders.clear();
    }
    
    // Reset the game
//    if (reset) {
//        gameState == GameController::GameStates::GAME;
//        score = 0;
//        player->start();
//        minions[0]->start();
//        minions[1]->start();
//        for (int i = 0; i < invaders.size(); i++) {
//            invaders[i]->start();
//        }
//    }
    
    if (gameState == GameController::GameStates::GAME) {
        for (int i = 0; i < invaders.size(); i++) {
            EnemyEmitter *emitter = invaders[i];
            emitter->setLifespan(invaderLife * 1000);    // convert to milliseconds
            emitter->setChildHealth(100);
            startLevel(1);
            if (score > 20) startLevel(2);
            if (score > 60) startLevel(3);
            if (score > 200) win = true;
            if (actualTime - sucessTimer > sucessTimeDelta) {
                emitter->update();
            }
        }
        
        for (int i = 0; i < minions.size(); i++) {
            Emitter *emitter = minions[i];
            emitter->setLifespan(invaderLife * 1000);    // convert to milliseconds
            emitter->setChildHealth(100);
            minions[0]->setPosition(ofVec3f(player->getPosition().x - 100, player->getPosition().y + 20, player->getPosition().z));
            minions[1]->setPosition(ofVec3f(player->getPosition().x + 100, player->getPosition().y + 20, player->getPosition().z));
            startLevel(1);
            if (score > 20) startLevel(2);
            if (score > 60) startLevel(3);
             emitter->setRate(playerRate);
             emitter->update();
        }
        
        if (bgPos1.y < ofGetWindowHeight() -1) {
            bgPos1 += ofVec2f(0, 0.5);
        }
        else {
            bgPos1 = ofVec2f(0, -ofGetWindowHeight());
        }
        
        if (bgPos2.y < ofGetWindowHeight() -1) {
            bgPos2 += ofVec2f(0, 0.5);
        }
        else {
            bgPos2 = ofVec2f(0, -ofGetWindowHeight());
        }
        
//        if (isLevelThree) {
//            minionBoosterParticle1.setPosition(ofVec3f(player->getPosition().x - 100, player->getPosition().y + 20, player->getPosition().z));
//            minionBoosterParticle2.setPosition(ofVec3f(player->getPosition().x + 100, player->getPosition().y + 20, player->getPosition().z));
//            minionBoosterParticle1.start();
//            minionBoosterParticle2.start();
//        }
    }
    
	player->update();
    updateSprite();
    checkCollisions();
    checkBoundary();
    explosion.update();
    boosterParticle.update();
//    minionBoosterParticle1.update();
//    minionBoosterParticle2.update();
    menuScreenBg.update();

}

// Switch case to change levels
//--------------------------------------------------------------
void ofApp::startLevel(int level) {
    switch (level) {
        case 1:
            levelOne();
            break;
        case 2:
            levelTwo();
            break;
        case 3:
            levelThree();
        default:
            break;
    }
}

// Given a distance return a modulated value between 1-10 based on
// sin wave shape from 0 to PI
//--------------------------------------------------------------
float ofApp::modulateAccel(float dist) {
    return sin(dist * PI) * 5.0 + 1.0;
}

//--------------------------------------------------------------
void ofApp::updateSprite() {
    
    //
    // calculate distance to travel for this update
    //
    float dist = player->speed * 1 / ofGetFrameRate();
    ofVec3f dir;
    ofRectangle r = ofGetWindowRect();
    
    //
    //  if the accelerator modifer key is pressed, accelerate and
    //  deacclerate sprite from starting position to window edge
    //
    if (accel) {
        
        switch (moveDir)
        {
            case MoveUp:
            {
                float totalDist = startAccelPoint.y;
                float frac = player->trans.y / totalDist;
                dir = ofVec3f(0, -dist * modulateAccel(frac), 0);
            }
                break;
            case MoveDown:
            {
                float totalDist = r.getMaxY() - startAccelPoint.y;
                float frac = player->trans.y / totalDist;
                dir = ofVec3f(0, dist * modulateAccel(frac), 0);
            }
                break;
            case MoveLeft:
            {
                float totalDist = startAccelPoint.x;
                float frac = player->trans.x / totalDist;
                dir = ofVec3f(-dist * modulateAccel(frac), 0, 0);
            }
                break;
            case MoveRight:
            {
                float totalDist = r.getMaxX() - startAccelPoint.x;
                float frac = player->trans.x / totalDist;
                dir = ofVec3f(dist * modulateAccel(frac), 0, 0);
                break;
            }
        }
        
    }
    else
    {
        switch (moveDir)
        {
            case MoveUp:
                dir = ofVec3f(0, -dist, 0);
                break;
            case MoveDown:
                dir = ofVec3f(0, dist, 0);
                break;
            case MoveLeft:
                dir = ofVec3f(-dist, 0, 0);
                break;
            case MoveRight:
                dir = ofVec3f(dist, 0, 0);
                break;
        }
    }
    player->trans += dir;
}

//--------------------------------------------------------------
void ofApp::moveSprite(MoveDir dir) {
    moveDir = dir;
}

//--------------------------------------------------------------
void ofApp::stopSprite() {
    moveDir = MoveStop;
}

//--------------------------------------------------------------
void ofApp::startAccel() {
    startAccelPoint = player->trans;
    accel = true;
}

//--------------------------------------------------------------
void ofApp::stopAccel() {
    accel = false;
}

//--------------------------------------------------------------
void ofApp::draw(){
    spacebarStrX = ofGetWidth() / 2;
    spacebarStrY = ofGetHeight() / 2;
    
    menuScreenBg.draw(0,0, ofGetWidth(), ofGetHeight());
    font.drawString("Press Spacebar to continue...", spacebarStrX-150, spacebarStrY+300);
    
    string scoreText;
    if (spacebarDown) {
        menuScreenBg.close();
//        backgroundImage.draw(0,0, ofGetWidth(), ofGetHeight());
        background.draw(bgPos1);
        background2.draw(bgPos2);
        explosion.sys->draw();
        boosterParticle.sys->draw();
//        minionBoosterParticle1.sys->draw();
//        minionBoosterParticle1.color = ofColor(52, 204, 255);
//        minionBoosterParticle2.sys->draw();
//        minionBoosterParticle2.color = ofColor(52, 204, 255);
        ofSetColor(255, 255, 255);
        scoreText += "Score: " + std::to_string(score);
        font.drawString(scoreText, 10, 40);
        player->draw();
        if (isLevelOne) {
            font.drawString("Level One", ofGetWindowWidth()/2+500, 40);
        } else if (isLevelTwo) {
            font.drawString("Level Two", ofGetWindowWidth()/2+500, 40);
        } else if (isLevelThree) {
            font.drawString("Level Three", ofGetWindowWidth()/2+500, 40);
        }
        for (int i = 0; i < invaders.size(); i++) {
            invaders[i]->draw();
        }
        for (int i = 0; i < minions.size(); i++) {
            if (isLevelThree) {
                minions[i]->draw();
            }
        }
//        for (int i = 0; i < minionBoosterParticle1.sys->particles.size(); i++) {
//            if (isLevelThree) {
//                minionBoosterParticle1.sys->draw();
//                minionBoosterParticle1.sys->particles[i].color = ofColor(52, 204, 255);
//                minionBoosterParticle2.sys->draw();
//                minionBoosterParticle2.sys->particles[i].color = ofColor(52, 204, 255);
//            }
//        }
        if (bulletFire) {
            player->start();
            for (int i = 0; i < minions.size(); i++) {
                minions[i]->start();
            }
        }
        if (gameOver) {
            font.drawString("GAME OVER", ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2);
        }
        if (win) {
            font.drawString("YOU WIN", ofGetWindowWidth() / 2 - 50, ofGetWindowHeight() / 2);
        }
    }

	if (!bHide) {
		guiPlayer.draw();
        guiInvader.draw();
	}
    
    if (bHelpText) {
        stringstream ss;
        ss <<"FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss <<"(f): Toggle Fullscreen"<<endl;
        ss <<"(h): Toggle Help"<<endl;
        ss <<"(i): Toggle Instructions"<<endl;
        ss <<"(Spacebar): Shoots a missile"<<endl;
        ss <<"Holding down the spacebar automatically shoots missiles"<<endl;
        ss <<"Move the player using the UP/DOWN/LEFT/RIGHT keys"<<endl;
        ss <<"Pressing the '.' key increases the speed of the player"<<endl;
        ss <<"Pressing the ',' key decreases the speed of the player"<<endl;
        ss <<"The rate and life of the emitter can be controlled using the GUI slider"<<endl;
        ss <<"Holding the (CTRL) key while moving will dash the player towards that direction"<<endl;
        
        ofDrawBitmapString(ss.str().c_str(), 0, guiPlayer.getPosition().x + guiPlayer.getHeight() + 50);
    }
}

//--------------------------------------------------------------
void ofApp::checkCollisions() {

    // remove all sprites emitted within a radius equal to the max
    // distance sprite can travel in one frame.
    //
//    float dist = player->maxDistPerFrame();
//    player->width = dist;
//    player->height = dist;
//    player->sys->removeNear(player->trans, player->maxDistPerFrame());
    float collisionDist, minionCollisionDist, playerCollisionDist;
    for (int i = 0; i < invaders.size(); i++) {
        collisionDist = player->childHeight / 2 + invaders[i]->childHeight / 2;
    }
    
    for (int i = 0; i < invaders.size(); i++) {
        playerCollisionDist = player->height / 2 + invaders[i]->childHeight / 2;
    }
    
    for (int i = 0; i < minions.size(); i++) {
        minionCollisionDist = invaders[i]->childHeight / 2 + minions[i]->childHeight / 2;
    }
    
    // Check for player bullet collision with the invader ships
    for (int i = 0; i < player->sys->sprites.size(); i++) {
        for (int j = 0; j < invaders.size(); j++) {
            
            int temp = invaders[j]->sys->removeNear(player->sys->sprites[i].trans, collisionDist);
            if (temp == 1) {
                score += temp;
                explosion.setPosition(ofVec3f(player->sys->sprites[i].trans));
                explosion.sys->reset();
                explosion.start();
                player->sys->sprites[i].lifespan = 0;
                sfxEnemyCollision.setVolume(0.5);
                sfxEnemyCollision.play();
            }
        }
    }
    
    // Check for minion bullet collision with the invader ships
    if (isLevelThree) {
        for (int i = 0; i < minions.size(); i++) {
            for (int j = 0; j < invaders.size(); j++) {
                for (int k = 0; k < minions[i]->sys->sprites.size(); k++) {
                    
                    int temp = invaders[j]->sys->removeNear(minions[i]->sys->sprites[k].trans, minionCollisionDist);
                    if (temp == 1) {
                        score += temp;
                        explosion.setPosition(ofVec3f(minions[i]->sys->sprites[k].trans));
                        explosion.sys->reset();
                        explosion.start();
                        minions[i]->sys->sprites[k].lifespan = 0;
                        sfxEnemyCollision.setVolume(0.5);
                        sfxEnemyCollision.play();
                    }
                }
            }
        }
    }
    
    // Check for player emitter collision with the invader ships
    for (int i = 0; i < invaders.size(); i++) {
        
        int temp = invaders[i]->sys->removeNear(player->trans, playerCollisionDist);
        if (temp == 1) {
            gameState == GameController::GameStates::GAME_OVER;
            gameOver = true;
            explosion.setPosition(ofVec3f(player->trans));
            explosion.sys->reset();
            explosion.start();
//            player->lifespan = 0;
//            player->drawable = false;
//            minions[0]->drawable = false;
//            minions[1]->drawable = false;
            sfxEnemyCollision.setVolume(0.5);
            sfxEnemyCollision.play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::checkBoundary() {
    
    if (player->trans.x-50 <= 0) {
        player->trans = ofVec3f(50, player->trans.y);
    } else if (player->trans.x+50 >= ofGetWidth()) {
        player->trans = ofVec3f(ofGetWidth()-50, player->trans.y);
    }
    
    if (player->trans.y-50 <= 0) {
        player->trans = ofVec3f(player->trans.x, 50);
    } else if (player->trans.y+50 >= ofGetHeight()) {
        player->trans = ofVec3f(player->trans.x, ofGetHeight()-50);
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouse_last;
	player->trans += delta;
	mouse_last = mouse_cur;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofPoint(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    switch (key) {
        case 'C':
        case 'c':
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            bHide = !bHide;
            break;
        case 'i':
        case 'I':
            bHelpText = !bHelpText;
        case 'r':
        case 'R':
            if (gameOver) {
                reset = true;
            }
            break;
        case 's':
            break;
        case 'u':
            break;
        case ' ':
            spacebarDown = true;
            gameState = GameController::GameStates::GAME;
            bulletFire = !bulletFire;
            break;
        case '.':
            player->speed += 30;
            break;
        case ',':
            player->speed -= 100;
            break;
        case OF_KEY_RIGHT:
            moveSprite(MoveRight);
            break;
        case OF_KEY_LEFT:
            moveSprite(MoveLeft);
            break;
        case OF_KEY_UP:
            moveSprite(MoveUp);
            break;
        case OF_KEY_DOWN:
            moveSprite(MoveDown);
            break;
        case OF_KEY_ALT:
            break;
        case OF_KEY_CONTROL:
            break;
        case OF_KEY_SHIFT:
            startAccel();
            break;
        case OF_KEY_DEL:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
    switch (key) {
        case ' ':
            bulletFire = true;
            break;
        case 'r':
        case 'R':
            reset = false;
            break;
        case OF_KEY_LEFT:
        case OF_KEY_RIGHT:
        case OF_KEY_UP:
        case OF_KEY_DOWN:
            stopSprite();
            stopAccel();
            break;
        case OF_KEY_ALT:
            break;
        case OF_KEY_CONTROL:
            break;
        case OF_KEY_SHIFT:
            stopAccel();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    spacebarStrX = float(w);
    spacebarStrY = float(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

