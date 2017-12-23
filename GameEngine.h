#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "Globals.h"
#include "Ground.h"
#include "Obstacles.h"
#include "Player.h"
#include "Scoreboard.h"
/* This file contains both the GameEngine statemachine,
 * the State interface and all the GameStates. 
 * 
 * Making the GameEngine a state machine helps us avoid 
 * needless branching in the core loop.  
 */
enum class GameState{
	StartMenu,
	GamePlay,
	GameOver
};

//These objects represents all objects in the game
//thus providing global access to the entire game state
//neatly avoiding the need for dynamic memory allocation
static Ground _ground;
static Obstacles _obstacles;
static Player _dino;
static Scoreboard _score;

//the State interface
class GameEngine; //forward declaration
struct State{
	explicit State(GameEngine& game) : _game{game}{};
	virtual void enter() const {};  //necessary, since "new" does not execute constructors for me
	virtual void input() const = 0; //the frame methods would normally not be const
	virtual void update() const = 0;//but since all game state is kept in the global Entities
	virtual void render() const = 0;//the engine states hold only behaviours and no data. 
	virtual void exit(){};
	virtual ~State(){};
protected:
	GameEngine& _game;
};

//the GameEngine state machine
class GameEngine{
	State* _currentState = nullptr;
	inline void maybeExitAndDeleteState() {
		if(!_currentState){ return; }
		_currentState->exit();
		delete _currentState;					
		_currentState = nullptr;
	}
public:	
	GameEngine(){		
		arduboy.initRandomSeed(); //gets re-seeded at start of gameplay		
		changeState(GameState::StartMenu);
	};
	inline void input() const {
		arduboy.pollButtons();
		_currentState->input();
	};
	inline void update() const{
		_currentState->update();
	};
	inline void render() const{
		arduboy.clear();
		_currentState->render();
		arduboy.display();
	};
	void changeState(const GameState next);
	virtual ~GameEngine(){
		maybeExitAndDeleteState();
	};
};

//Implementation of all three gamestates
struct GamePlayState : public State{
	explicit GamePlayState(GameEngine& game) : State(game){};
	void enter() const override{
		arduboy.setFrameRate(FPS);
		_obstacles.reset();
		_score.reset();
		_dino.reset();
		arduboy.initRandomSeed();
	};
	void input() const override{
		_dino.input();
	};
	void update() const override{
		_ground.update();
		_obstacles.update();
		_dino.update();
		_score.update();
		if(_obstacles.collidesWith(_dino)){
			_dino.onCollision();			
			_game.changeState(GameState::GameOver);
			return; //NOTE: we must return immediately after a state change.
		}
	};
	void render() const override{
		_obstacles.render();
		_ground.render();
		_dino.render();
		_score.render();		
	};
	virtual ~GamePlayState(){		
	};
};
struct StartMenuState : public State{
	explicit StartMenuState(GameEngine& game) : State(game){};
	void enter() const override{
		arduboy.setFrameRate(FPS_MENUS);
	};
	void input() const override{
		if(arduboy.justPressed(ACCEPT_BUTTON)){			
			_game.changeState(GameState::GamePlay);
			return; //NOTE: we must return immediately after a state change.
		}
	};	
	void update() const override{
	};
	void render() const override{
		arduboy.setCursor(17, 12);
		arduboy.print(F("Press A to start!"));
		_ground.render();
		_dino.render();
		_score.render();		
	};
	virtual ~StartMenuState(){		
	}
};
struct GameOverState : public State{
	explicit GameOverState(GameEngine& game) : State(game){	};	
	void enter() const override{
		arduboy.setFrameRate(FPS_MENUS);
		_score.onGameOver();
	}
	void input() const override{
		if(arduboy.justPressed(ACCEPT_BUTTON)){			
			_game.changeState(GameState::GamePlay);
			return;  //NOTE: we must return immediately after a state change.
		}
	};
	void update() const override{		
		_dino.update(); //keep updating to allow dino corpse to fall to the ground
	};
	void render() const override{				
		_obstacles.render();
		_ground.render();
		_dino.render();
		arduboy.setCursor(40, 12);
		arduboy.print(F("Game Over!"));
		_score.render();		
	};
	virtual ~GameOverState(){}
};
#endif //GAMEENGINE_H