/*
 * GTASA.h
 *
 *  Created on: 12 May 2017
 *      Author: hhoca
 */

#ifndef GTASA_H_
#define GTASA_H_

#include "windows.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "string.h"
#include "Math.h"

using namespace std;

class Car;

class Car {
public:
	Car(DWORD address);
	virtual ~Car();
	WORD id;
	float x, y, z;
	void print();
	BOOL valid();
	float getRotation();
	float getRotationRads();
	float getX();
	float getY();
	float getZ();
	static void updateCarList();
	static std::vector<Car*> listOfCars;
	DWORD address;
private:
	DWORD getRotMatrix();
};

class Player{
public:
	Player(unsigned long address);
	virtual ~Player();
	float getX();
	float getY();
	float getZ();
	int getRotation();
	int getHeatlh();
	int getMaxHealth();
	int getArmor();
	DWORD getAddress();
	Car& getCar();
private:
	DWORD address;
	DWORD getPosMatrix();
};

class GTASA {
public:
	static GTASA *getInstance();
	float getPlayerRotation();
	void forward();
	void teleportPlayerToTestArea();
	Player* getPlayer();
	virtual ~GTASA();
	bool isRunning();
	int getDWORD(unsigned long address);
	float getFloat(unsigned long address);
	HWND window;
	DWORD processID;
	HANDLE handle;
	DWORD PedPool;
	static ofstream LOG;
private:
	static GTASA instance;
	Player *player;
	GTASA();
};


#define _getDWORD(address) GTASA::getInstance()->getDWORD(address)
#define _getFloat(address) GTASA::getInstance()->getFloat(address)

#endif /* GTASA_H_ */
