/*
 * GTA.cpp
 *
 *  Created on: 12 May 2017
 *      Author: hhoca
 */

#include "GTASA.h"
#include "GTA.h"

JNIEXPORT jboolean JNICALL Java_GTA_init(JNIEnv *env, jobject obj){
	return GTASA::getInstance()->isRunning();
}

JNIEXPORT void JNICALL Java_GTA_kill(JNIEnv *env, jobject obj){
	delete GTASA::getInstance();
}

JNIEXPORT jboolean JNICALL Java_GTA_onFoot(JNIEnv *env, jobject obj){
	Car car = GTASA::getInstance()->getPlayer()->getCar();
	bool onFoot = car.valid();
	return !onFoot;
}

JNIEXPORT jint JNICALL Java_GTA_getX(JNIEnv *env, jobject obj){
	return (int) GTASA::getInstance()->getPlayer()->getX();
}

JNIEXPORT jint JNICALL Java_GTA_getY(JNIEnv *env, jobject obj){
	return (int) GTASA::getInstance()->getPlayer()->getY();
}

JNIEXPORT jint JNICALL Java_GTA_getRotation(JNIEnv *env, jobject obj){
	return GTASA::getInstance()->getPlayer()->getRotation();
}

JNIEXPORT jint JNICALL Java_GTA_getHealth(JNIEnv *env, jobject obj){
	return GTASA::getInstance()->getPlayer()->getHeatlh();
}

JNIEXPORT jfloatArray JNICALL Java_GTA_getCars(JNIEnv *env, jobject obj){
	Car::updateCarList();
	int size = Car::listOfCars.size();
	jfloatArray arr = env->NewFloatArray(size * 4);
	jfloat buffer[size * 4];
	for(int i = 0; i < size; i++){
		buffer[(i * 4)] = Car::listOfCars[i]->getX();
		buffer[(i * 4) + 1] = Car::listOfCars[i]->getY();
		buffer[(i * 4) + 2] = Car::listOfCars[i]->getZ();
		buffer[(i * 4) + 3] = Car::listOfCars[i]->getRotation();
	}
	env->SetFloatArrayRegion(arr, 0, size * 4, buffer);
	return arr;
}
