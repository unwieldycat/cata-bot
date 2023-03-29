#include "devices.hpp"

// =========================== Device Definitions =========================== //

// HID
pros::Controller master_controller(pros::E_CONTROLLER_MASTER);

// Drivetrain motors
pros::Motor drive_l(1);
pros::Motor drive_r(2);

// Catapult motors
pros::Motor primer(3, pros::motor_gearset_e::E_MOTOR_GEAR_RED);
pros::Motor latch(4, pros::motor_gearset_e::E_MOTOR_GEAR_RED);

// Claw motors
pros::Motor claw_pinch(5);
pros::Motor claw_vert(6);

// ADI Devices
pros::ADIDigitalIn cata_limit('A');

// ============================ Device Functions ============================ //

bool cata_primed;

void cata_control() {
	while (true) {
		if (master_controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			if (!cata_primed) {
				master_controller.set_text(1, 1, "Not primed!");
				master_controller.rumble("-");
				continue;
			}

			latch.move_absolute(0, 200);
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
			latch.move_relative(-670, 200);

			pros::delay(2000);

			primer.move_absolute(0, 200);

			cata_primed = true;
		}
	}
}
