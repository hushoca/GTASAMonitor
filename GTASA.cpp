/*
 * GTASA.cpp
 *
 *  Created on: 12 May 2017
 *      Author: hhoca
 */

#include "GTASA.h"

std::vector<Car*> Car::listOfCars;
ofstream GTASA::LOG("log.txt");

Car::Car(DWORD address) {
	x = 0;
	y = 0;
	z = 0;
	this->address = address;
	id = _getDWORD(address + 34);
	//cout << "Car at " << address << " allocated." << endl;
}

DWORD Car::getRotMatrix(){
	if(address == NULL) return NULL;
	return _getDWORD(address + 20);
}

float Car::getX(){
	return _getFloat(getRotMatrix() + 48);
}

float Car::getY(){
	return _getFloat(getRotMatrix() + 52);
}

float Car::getZ(){
	return _getFloat(getRotMatrix() + 56);
}

float Car::getRotation(){
	if(address == NULL) return 0.0f;
	float xRot = _getFloat(getRotMatrix());
	float yRot = _getFloat(getRotMatrix() + 4);
	float angle = ((atan2(yRot, xRot) * 360) / (2 * 3.1415)); //radian angle to degrees (180 -> 0 -> -180)
	angle += (angle < 0)? 360: 0;	//convert 180 -> 0 -> -180 to 0 -> 360
	return angle;
}

float Car::getRotationRads(){
	if(address == NULL) return 0.0f;
	float xRot = _getFloat(getRotMatrix());
	float yRot = _getFloat(getRotMatrix() + 4);
	return atan2(yRot, xRot);
}

void Car::print(){
	GTASA::LOG << "\t[CAR][ID: " << id << ", Location: {" << getX() << ", " << getY() << ", " << getZ() << "}, Rotation: " << getRotation() << "]" << endl;
	//cout << "[CAR][ID: " << id << ", Location: {" << getX() << ", " << getY() << ", " << getZ() << "}, Rotation: " << getRotation() << "]" << endl;
}

void Car::updateCarList(){
	DWORD pToInfoStruct = _getDWORD(0xB74494);
	DWORD noOfCars = _getDWORD(pToInfoStruct + 12) + 1;
	noOfCars = _getDWORD(pToInfoStruct + 8);
	DWORD pool = _getDWORD(pToInfoStruct);

	for(unsigned int i = 0; i < listOfCars.size(); i++) delete listOfCars[i];
	listOfCars.clear();

	for(unsigned int i = 0; i < noOfCars; i++){
		DWORD add = pool + (i * 0xA18);
		if(add == GTASA::getInstance()->getPlayer()->getCar().address) continue;
		Car *c = new Car(add);
		if(c->valid()){
			listOfCars.push_back(c);
		}else{
			delete c;
		}
	}
}

BOOL Car::valid(){
	return !((getX() == 0 && getY() == 0 && getZ() == 0 && getRotation() == 0) || id == 0 || address == 0);
}

Car::~Car() {
	//cout << "Car at " << address << " deleted." << endl;
}


Player::Player(unsigned long address) {
	cout << "Player created" << endl;
	this->address = address;
}

DWORD Player::getPosMatrix() {
	return (address != 0)? _getDWORD(address + 0x14): 0;
}

float Player::getX() {
	return (address != 0)? (int)_getFloat(getPosMatrix() + 0x30): 0;
}

float Player::getY() {
	return (address != 0)? (int)_getFloat(getPosMatrix() + 0x34): 0;
}

float Player::getZ() {
	return (address != 0)? (int)_getFloat(getPosMatrix() + 0x38): 0;
}

int Player::getRotation() {
	if(address == 0) return 0;
	//float ang = _getFloat(getPosMatrix() + 0x558) * (180/3.1415);
	/*float ang = _getFloat(_getDWORD(address + 20) + 0x558) * (180/3.1415);
	ang += (ang < 0)? 360: 0;
	cout << "player rotation : " << ang << endl;
	cout << "hp:" << getHeatlh() << endl;


	//test code start
	if(address == NULL) return 0.0f;*/
	float xRot = _getFloat(getPosMatrix());
	float yRot = _getFloat(getPosMatrix() + 4);
	float angle = ((atan2(yRot, xRot) * 360) / (2 * 3.1415)); //radian angle to degrees (180 -> 0 -> -180)
	angle += (angle < 0)? 360: 0;	//convert 180 -> 0 -> -180 to 0 -> 360
	//test code end
	return (int) angle;
}

int Player::getHeatlh() {
	return (address != 0)? (int)_getFloat(address + 0x540): 0;
}

int Player::getMaxHealth() {
	return (address != 0)? (int)_getFloat(address + 0x544): 0;
}

int Player::getArmor() {
	return 0;
}

DWORD Player::getAddress() {
	return address;
}

Car& Player::getCar(){
	DWORD PlayersCarPointer = _getDWORD(0xBA18FC);
	Car &c = *new Car(PlayersCarPointer);
	return c;
}

Player::~Player(){
	cout << "Player object killed!" << endl;
}

GTASA::GTASA() {
	processID = NULL;
	handle = NULL;
	window = FindWindow(0, "GTA: San Andreas");

	LOG << "Trainer started!" << endl;

	if(window != NULL){
		GetWindowThreadProcessId(window, &processID);
		handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
		//cout << "GTA: San Andreas is running. Attaching..."<< endl;
		LOG << "GTA: San Andreas is running. Attaching..." << endl;

		if(processID != NULL){
			LOG << "Process ID found." << endl << endl;
			PedPool = getDWORD(0xB6F5F0);
			player = new Player(PedPool);
		}
	}
}

bool GTASA::isRunning(){
	return (processID != 0);
}

GTASA *GTASA::getInstance(){
	static GTASA ins;
	return &ins;
}

int GTASA::getDWORD(unsigned long address){
	DWORD d;
	ReadProcessMemory(handle, (LPCVOID)address, &d, sizeof(DWORD), NULL);
	return d;
}

float GTASA::getFloat(unsigned long address){
	float d;
	ReadProcessMemory(handle, (LPCVOID)address, &d, sizeof(float), NULL);
	return d;
}

Player *GTASA::getPlayer(){
	return player;
}


void GTASA::forward(){
	PostMessage(GTASA::window, WM_KEYDOWN, 'W', 0);
	Sleep(50);
	PostMessage(GTASA::window, WM_KEYUP, 'W', 0);
}

GTASA::~GTASA() {
	if(processID != 0){
		delete player;
	}
	LOG << "\nTrainer terminated.";
	LOG.close();
}


