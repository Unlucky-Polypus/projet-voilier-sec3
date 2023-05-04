#!/bin/bash

S_MAIN_INO=./main/main.ino
S_IMU_C=./projet_voilier_imu/projet_voilier_imu.cpp
S_IMU_H=./projet_voilier_imu/projet_voilier_imu.h
S_GPS_C=./projet_voilier_gps/projet_voilier_gps.cpp
S_GPS_H=./projet_voilier_gps/projet_voilier_gps.h
S_SERVO_C=./projet_voilier_commandes_servo/projet_voilier_commandes_servo.cpp
S_SERVO_H=./projet_voilier_commandes_servo/projet_voilier_commandes_servo.h
S_GIROUETTTE_H=./projet_voilier_girouette/projet_voilier_girouette.h

BUILD_DIR=./build
D_MAIN_INO=$BUILD_DIR/main.ino
D_IMU_C=$BUILD_DIR/projet_voilier_imu.cpp
D_IMU_H=$BUILD_DIR/projet_voilier_imu.h
D_GPS_C=$BUILD_DIR/projet_voilier_gps.h
D_GPS_H=$BUILD_DIR/projet_voilier_gps.h
D_SERVO_C=$BUILD_DIR/projet_voilier_commandes_servo.cpp
D_SERVO_H=$BUILD_DIR/projet_voilier_commandes_servo.h
D_GIROUETTTE_H=$BUILD_DIR/projet_voilier_girouette.h


[ ! -d "$BUILD_DIR" ] && mkdir $BUILD_DIR

if test -f "$D_IMU_C"; then
    rm $D_IMU_C
fi
cp $S_IMU_C $D_IMU_C
echo "$D_IMU_C : Added to the build"

if test -f "$D_IMU_H"; then
    rm $D_IMU_H
fi
cp $S_IMU_H $D_IMU_H
echo "$D_IMU_H : Added to the build"

if test -f "$D_GPS_C"; then
    rm $D_GPS_C
fi
cp $S_GPS_C $D_GPS_C
echo "$D_GPS_C : Added to the build"

if test -f "$D_GPS_H"; then
    rm $D_GPS_H
fi
cp $S_GPS_H $D_GPS_H
echo "$D_GPS_H : Added to the build"

if test -f "$D_GIROUETTTE_H"; then
    rm $D_GIROUETTTE_H
fi
cp $S_GIROUETTTE_H $D_GIROUETTTE_H
echo "$D_GIROUETTTE_H : Added to the build"

if test -f "$D_SERVO_C"; then
    rm $D_SERVO_C
fi
cp $S_SERVO_C $D_SERVO_C
echo "$D_SERVO_C : Added to the build"

if test -f "$D_SERVO_H"; then
    rm $D_SERVO_H
fi
cp $S_SERVO_H $D_SERVO_H
echo "$D_SERVO_H : Added to the build"

if test -f "$D_MAIN_INO"; then
    rm $D_MAIN_INO
fi
cp $S_MAIN_INO $D_MAIN_INO
echo "$D_MAIN_INO : Added to the build"