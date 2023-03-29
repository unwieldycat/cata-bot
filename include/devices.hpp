#pragma once
#include "main.h"
#include "pros/motors.hpp"

// ========================== Device Declarations ========================== //

// HID
extern pros::Controller master_controller;

// Drivetrain motors
extern pros::Motor drive_l;
extern pros::Motor drive_r;
extern pros::Motor drive_m;

// Catapult motors
extern pros::Motor primer;
extern pros::Motor latch;

// Claw motors
extern pros::Motor claw_pinch;
extern pros::Motor claw_vert;

// ====================== Device Function Declarations ====================== //

extern bool cata_primed;
extern void cata_control();
extern void drive_control();