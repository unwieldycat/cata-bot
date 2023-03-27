#include "devices.hpp"
#include "pros/misc.h"

// =========================== Device Definitions =========================== //

// HID
pros::Controller master_controller(pros::E_CONTROLLER_MASTER);

// Drivetrain motors
pros::Motor drive_l(1);
pros::Motor drive_r(2);

// Catapult motors
pros::Motor primer(3);
pros::Motor latch(4);

// Claw motors
pros::Motor claw_pinch(5);
pros::Motor claw_vert(6);

// ============================ Device Functions ============================ //
