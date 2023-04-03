#include "devices.hpp"
#include "pros/adi.hpp"
#include "pros/motors.h"

// =========================== Device Definitions =========================== //

// HID
pros::Controller master_controller(pros::E_CONTROLLER_MASTER);

// Drivetrain motors
pros::Motor drive_l(18);
pros::Motor drive_r(-19);
pros::Motor drive_m(-20);

// Catapult motors
pros::Motor primer(3, pros::motor_gearset_e::E_MOTOR_GEAR_RED);
pros::Motor latch(4, pros::motor_gearset_e::E_MOTOR_GEAR_RED);

// Claw motors
pros::Motor claw_pinch(5);
pros::Motor claw_vert(6);

// ADI Devices
pros::ADIDigitalIn cata_limit('A');
pros::ADIAnalogIn latch_pot('B');

// ============================ Device Functions ============================ //

bool cata_primed;

void cata_control() {
	while (true) {
		if (master_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X) &&
		    latch_pot.get_value_calibrated() < 16) {
			latch.move_relative(670, 200);
			cata_primed = false;
		}

		if (master_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			if (cata_primed) continue;

			latch.move_relative(120, 200);
			primer.move(127);

			while (!cata_limit.get_value()) {
				pros::delay(100);
			}

			primer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			primer.brake();

			latch.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			latch.move(-64);

			pros::delay(2000);

			primer.move_absolute(0, 200);
			latch.brake();

			cata_primed = true;
		}
	}
}

void claw_control() {
	bool open;
	bool close;
	bool up;
	bool down;
	bool lat_active;
	bool pinch_active;

	claw_vert.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	claw_pinch.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	while (true) {
		up = master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
		down = master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

		if (up && !down) {
			claw_vert.move(-32);
			lat_active = true;
		} else if (down && !up) {
			claw_vert.move(16);
			lat_active = true;
		} else if (lat_active) {
			claw_vert.brake();
			lat_active = false;
		}

		open = master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);
		close = master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT);

		if (open && !close) {
			claw_pinch.move(64);
			pinch_active = true;
		} else if (close && !open) {
			claw_pinch.move(-64);
			pinch_active = true;
		} else if (pinch_active) {
			claw_pinch.brake();
			pinch_active = false;
		}
	}
}

void drive_control() {
	int turn;
	int drive;
	int strafe;
	bool active;

	while (true) {
		turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		drive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		strafe = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		if ((turn < -0.5 || turn > 0.5) || (drive < -0.5 || drive > 0.5) ||
		    (strafe < -0.5 || strafe > 0.5)) {
			drive_l.move(drive + turn);
			drive_r.move(drive - turn);
			drive_m.move(strafe);
			active = true;
		} else if (active) {
			drive_l.brake();
			drive_r.brake();
			drive_m.move(strafe);
			active = false;
		}
	}
}