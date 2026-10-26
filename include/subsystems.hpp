#pragma once

#include "lemlib/chassis/chassis.hpp"
#include "pros/optical.hpp"

#include "pros/distance.hpp"


extern pros::v5::Distance FrontDistSensor; // Front (X correction)
extern pros::v5::Distance RightDistSensor; // Right (Y correction)
extern pros::v5::Distance LeftDistSensor; // Left (Y correction)




extern pros::ADIDigitalOut Wing;
extern pros::ADIDigitalOut Loader;
extern pros::ADIDigitalOut MidGoal;

extern pros::MotorGroup Winch;

extern pros::MotorGroup leftMotors;
// right motor group
extern pros::MotorGroup rightMotors;

// Inertial Sensor on port 10
extern pros::Imu imu;

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
extern pros::Rotation horizontalEnc;
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
extern pros::Rotation verticalEnc;


// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
extern lemlib::TrackingWheel horizontal;
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
extern lemlib::TrackingWheel vertical;


// drivetrain settings
extern lemlib::Drivetrain drivetrain;

// lateral motion controller
extern lemlib::ControllerSettings linearController;

// angular motion controller
extern lemlib::ControllerSettings angularController;

// sensors for odometry
extern lemlib::OdomSensors sensors;

// input curve for throttle input during driver control
extern lemlib::ExpoDriveCurve throttleCurve;

// input curve for steer input during driver control
extern lemlib::ExpoDriveCurve steerCurve;

// create the chassis
extern lemlib::Chassis chassis;


