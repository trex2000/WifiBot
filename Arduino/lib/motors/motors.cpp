/****************************************************************************
 * Copyright (C) 2015														*
 *                                                                          *
 * This file is part of RC CAR                                              *
 *                                                                          *
 *   Box is free software: you can redistribute it and/or modify it         *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Box is distributed in the hope that it will be useful,                 *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file motors.cpp
 * @author Alex
 * @date 10 Jul 2015
 * @brief File containing functions for IO
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */


#include "motors.h"
#include "IO_extern.h"
#include "ProjectMain.h"
#include "custom_types.h"

/**
 * @brief Enumeration of movement states
 *
 * Enumeration of movement states.
 */
typedef enum en_movement_states {
	EN_STATE_ACCELERATION ,				/**< Acceleration */
	EN_STATE_DECELERATION ,				/**< Breaking */
	EN_STATE_STEERING_LEFT ,			/**< ST Left*/
	EN_STATE_BREAK_LEFT ,			/**< B Left*/
	EN_STATE_STEERING_RIGHT,			/**< ST Right*/
	EN_STATE_BREAK_RIGHT,			/**< B Right*/
	EN_STATE_IDLE ,						/**< Idle */
	EN_NUMBER_OF_ELEMENTS_STATES ,      /**< Number of states*/
} EN_MOVEMENT_STATES;

/**
 * @brief Enumeration of requests movement states
 *
 * Enumeration of requests movement states.
 */
typedef enum en_movement_request_states {
	EN_STATE_REQ_ACCELERATION ,				/**< Acceleration */
	EN_STATE_REQ_DECELERATION ,				/**< Breaking */
	EN_STATE_REQ_STEERING_LEFT ,			/**< ST Left*/
	EN_STATE_REQ_STEERING_RIGHT,			/**< ST Right*/
	EN_STATE_REQ_IDLE ,						/**< Idle */
	EN_STATE_REQ_ERROR ,						/**< Idle */
	EN_NUMBER_OF_REQ_ELEMENTS_STATES ,      /**< Number of states*/
} EN_MOVEMENT_REQUEST_STATES;

/**
 * @brief Enumeration of movement directions
 *
 * Enumeration of movement directions
 */
typedef enum en_movement_directions {
	EN_STATE_FORWARD ,				/**< Forward */
	EN_STATE_BACKWARD ,				/**< Backward */
	EN_NUMBER_OF_MOVEMENT_STATES ,      /**< Number of states*/
} EN_MOVEMENT_DIRECTIONS;

/**
 * @brief union of flags
 *
 *  union of flags
 */
flags8 flags8_motors_u;

/**
 * @brief enum of states
 *
 *  enum of states
 */
uint8_t motorSpeed_u8;
/**
 * @brief enum of states
 *
 *  enum of states
 */
EN_MOVEMENT_STATES motorStates_en;

/**
 * @brief enum of requests states
 *
 *  enum of requests states
 */
EN_MOVEMENT_REQUEST_STATES motorReqStates_en;


#define MOTOR_DIRECTION flags8_motors_u.bits.b0


/**
 * @brief maximum engine duty cycle
 *
 * maximum engine duty cycle
 */
#define MAX_MOTOR_SPEED 100u

/**
 * @brief engine acceleration step
 *
 *engine acceleration step
 */
#define MOTOR_SPEED_STEP_UP 1u

/**
 * @brief engine deceleration step
 *
 *engine deceleration step
 */
#define MOTOR_SPEED_STEP_DOWN 8u

/**
 * @brief engine deceleration step while no one of the input button is puhsed
 *
 *engine deceleration step in idle state
 */
#define MOTOR_SPEED_STEP_DOWN_IDLE 1u

/**
 * @brief engine minimum speed
 *
 *engine minimum speed
 */
#define MOTOR_MIN_SPEED 20u

/**
 * @brief engine steering speed
 *
 *engine steering speed
 */
#define MOTOR_STEERING_SPEED 20u

#define delayBetweenDirectionChange (350u/20u)
/**
* @brief Implementation of function that handles the initialization of motor control
*
* mplementation of function that handles the initialization of motor control
* @return void
* @note Void function with no return.
*/
void motorsInit()
{
	motorStates_en=EN_STATE_IDLE;
	motorSpeed_u8=0;
	MOTOR_DIRECTION = EN_STATE_FORWARD;
}



/**
* @brief Implementation of function that handles the motor control
*
* Implementation of function that handles the motor control
* @return void
* @note Void function with no return.
*/
void motorsCyclic()
{
	motorsInputsAQ();
	motorsSM();
	motorsActuator();
}

/**
* @brief Implementation of function that handles the motor input aquisition
*
* Implementation of function that handles the motor control
* @return void
* @note Void function with no return.
*/

void motorsInputsAQ()
{

	if(GetInputPin(EN_SID_WIFI_CONTROL_UP))
	{
		motorReqStates_en=EN_STATE_REQ_ACCELERATION;
	}
	else
	{
		if(GetInputPin(EN_SID_WIFI_CONTROL_LEFT))
		{
			motorReqStates_en=EN_STATE_REQ_STEERING_LEFT;
		}
		else
		{
			if(GetInputPin(EN_SID_WIFI_CONTROL_RIGHT))
			{
				motorReqStates_en=EN_STATE_REQ_STEERING_RIGHT;
			}
			else
			{
				if(GetInputPin(EN_SID_WIFI_CONTROL_DOWN))
				{
					motorReqStates_en=EN_STATE_REQ_DECELERATION;
				}
				else
				{
					if(!GetInputPin(EN_SID_WIFI_CONTROL_UP) && !GetInputPin(EN_SID_WIFI_CONTROL_LEFT) && !GetInputPin(EN_SID_WIFI_CONTROL_RIGHT) && !GetInputPin(EN_SID_WIFI_CONTROL_DOWN))
					{
						motorReqStates_en=EN_STATE_REQ_IDLE;
					}
					else
					{
						motorReqStates_en=EN_STATE_REQ_ERROR;
					}
				}
			}
		}
	}

}

/**
* @brief Implementation of function that handles the motor state machine
*
* Implementation of function that handles the motor control
* @return void
* @note Void function with no return.
*/
void motorsSM()
{
	static uint8_t delayBetweenDirectionChangeCounter = 0;

	switch(motorStates_en)
	{
		case EN_STATE_ACCELERATION:
		case EN_STATE_DECELERATION:
		case EN_STATE_IDLE:
			switch (motorReqStates_en) {
				case EN_STATE_REQ_ACCELERATION:
					motorStates_en = EN_STATE_ACCELERATION;
				break;
				case EN_STATE_REQ_DECELERATION:
					motorStates_en = EN_STATE_DECELERATION;
				break;
				case EN_STATE_REQ_STEERING_LEFT:
					motorStates_en = EN_STATE_BREAK_LEFT;
				break;
				case EN_STATE_REQ_STEERING_RIGHT:
					motorStates_en = EN_STATE_BREAK_RIGHT;
				break;
				case EN_STATE_REQ_IDLE:
				case EN_STATE_REQ_ERROR:
					motorStates_en = EN_STATE_IDLE;
				break;
				default:
				break;
			}
		break;
		case EN_STATE_STEERING_LEFT:
		case EN_STATE_BREAK_LEFT:
			switch (motorReqStates_en) {
				case EN_STATE_REQ_ACCELERATION:
					motorStates_en = EN_STATE_ACCELERATION;
				break;
				case EN_STATE_REQ_DECELERATION:
					motorStates_en = EN_STATE_DECELERATION;
				break;
				case EN_STATE_REQ_STEERING_LEFT:
					motorStates_en = EN_STATE_STEERING_LEFT;
				break;
				case EN_STATE_REQ_STEERING_RIGHT:
					motorStates_en = EN_STATE_BREAK_RIGHT;
				break;
				case EN_STATE_REQ_IDLE:
				case EN_STATE_REQ_ERROR:
					motorStates_en = EN_STATE_IDLE;
				break;
				default:
				break;
			}
		break;
		case EN_STATE_STEERING_RIGHT:
		case EN_STATE_BREAK_RIGHT:
			switch (motorReqStates_en) {
				case EN_STATE_REQ_ACCELERATION:
					motorStates_en = EN_STATE_ACCELERATION;
				break;
				case EN_STATE_REQ_DECELERATION:
					motorStates_en = EN_STATE_DECELERATION;
				break;
				case EN_STATE_REQ_STEERING_LEFT:
					motorStates_en = EN_STATE_BREAK_LEFT;
				break;
				case EN_STATE_REQ_STEERING_RIGHT:
					motorStates_en = EN_STATE_STEERING_RIGHT;
				break;
				case EN_STATE_REQ_IDLE:
				case EN_STATE_REQ_ERROR:
					motorStates_en = EN_STATE_IDLE;
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}

	if ((motorSpeed_u8 == 0) && (motorReqStates_en == EN_STATE_REQ_ACCELERATION))
	{
		if (delayBetweenDirectionChangeCounter == 0)
		{
			MOTOR_DIRECTION = EN_STATE_FORWARD;
			delayBetweenDirectionChangeCounter = delayBetweenDirectionChange;
		}
		delayBetweenDirectionChangeCounter--;
	}
	if ((motorSpeed_u8 == 0) && (motorReqStates_en == EN_STATE_REQ_DECELERATION))
	{
		if (delayBetweenDirectionChangeCounter == 0)
		{
			MOTOR_DIRECTION = EN_STATE_BACKWARD;
			delayBetweenDirectionChangeCounter = delayBetweenDirectionChange;
		}
		delayBetweenDirectionChangeCounter--;
	}

	switch(motorStates_en)
	{
		case EN_STATE_ACCELERATION:
			if(MOTOR_DIRECTION == EN_STATE_FORWARD)
			{
				if ((motorSpeed_u8+MOTOR_SPEED_STEP_UP)<=MAX_MOTOR_SPEED)
					{
						if(motorSpeed_u8 < MOTOR_MIN_SPEED)
						{
							motorSpeed_u8 = MOTOR_MIN_SPEED;
						}
						else
						{
							motorSpeed_u8+=MOTOR_SPEED_STEP_UP;
						}
					}
					else
					{
						motorSpeed_u8=MAX_MOTOR_SPEED;
					}
			}
			if(MOTOR_DIRECTION == EN_STATE_BACKWARD)
			{
				if ((motorSpeed_u8-MOTOR_SPEED_STEP_DOWN)>=0)
					{
						if (motorSpeed_u8 <= MOTOR_MIN_SPEED)
						{
							motorSpeed_u8 = 0;
						}
						else
						{
							motorSpeed_u8-=MOTOR_SPEED_STEP_DOWN;
						}
					}
					else
					{
						motorSpeed_u8=0;
					}
			}
		break;
		case EN_STATE_DECELERATION:
		if(MOTOR_DIRECTION == EN_STATE_FORWARD)
		{
			if ((motorSpeed_u8-MOTOR_SPEED_STEP_DOWN)>=0)
				{
					if (motorSpeed_u8 <= MOTOR_MIN_SPEED)
					{
						motorSpeed_u8 = 0;
					}
					else
					{
						motorSpeed_u8-=MOTOR_SPEED_STEP_DOWN;
					}
				}
				else
				{
					motorSpeed_u8=0;
				}
		}
		if(MOTOR_DIRECTION == EN_STATE_BACKWARD)
		{
			if ((motorSpeed_u8+MOTOR_SPEED_STEP_UP)<=MAX_MOTOR_SPEED)
				{
					if(motorSpeed_u8 < MOTOR_MIN_SPEED)
					{
						motorSpeed_u8 = MOTOR_MIN_SPEED;
					}
					else
					{
						motorSpeed_u8+=MOTOR_SPEED_STEP_UP;
					}
				}
				else
				{
					motorSpeed_u8=MAX_MOTOR_SPEED;
				}
		}
		break;
		case EN_STATE_IDLE:
			if ((motorSpeed_u8-MOTOR_SPEED_STEP_DOWN_IDLE)>=0)
					{
						if (motorSpeed_u8 <= MOTOR_MIN_SPEED)
						{
							motorSpeed_u8 = 0;
						}
						else
						{
							motorSpeed_u8-=MOTOR_SPEED_STEP_DOWN_IDLE;
						}
					}
					else
					{
						motorSpeed_u8=0;
					}
		break;
		case EN_STATE_STEERING_LEFT:
		case EN_STATE_STEERING_RIGHT:
			motorSpeed_u8=MOTOR_STEERING_SPEED;
		default:
		break;
	}
	Serial.println(motorSpeed_u8);
}



/**
* @brief Implementation of function that handles the motor actuator
*
* Implementation of function that handles the motor actuator
* @return void
* @note Void function with no return.
*/
void motorsActuator()
{

	switch(motorStates_en)
	{
		case EN_STATE_ACCELERATION:
		case EN_STATE_DECELERATION:
		case EN_STATE_IDLE:
			if (MOTOR_DIRECTION == EN_STATE_FORWARD)
			{
				setOutputPin (EN_SOD_MOTOR_RIGHT_0, 0);
				setOutputPin (EN_SOD_MOTOR_RIGHT_1, 1);
				setOutputPin (EN_SOD_MOTOR_LEFT_0, 0);
				setOutputPin (EN_SOD_MOTOR_LEFT_1, 1);
			}
			else if (MOTOR_DIRECTION == EN_STATE_BACKWARD)
			{
				setOutputPin (EN_SOD_MOTOR_RIGHT_0, 1);
				setOutputPin (EN_SOD_MOTOR_RIGHT_1, 0);
				setOutputPin (EN_SOD_MOTOR_LEFT_0, 1);
				setOutputPin (EN_SOD_MOTOR_LEFT_1, 0);
			}
			else
			{
				setOutputPin (EN_SOD_MOTOR_RIGHT_0, 0);
				setOutputPin (EN_SOD_MOTOR_RIGHT_1, 0);
				setOutputPin (EN_SOD_MOTOR_LEFT_0, 0);
				setOutputPin (EN_SOD_MOTOR_LEFT_1, 0);
			}
			break;
		case EN_STATE_STEERING_LEFT:
			setOutputPin (EN_SOD_MOTOR_RIGHT_0, 0);
			setOutputPin (EN_SOD_MOTOR_RIGHT_1, 1);
			setOutputPin (EN_SOD_MOTOR_LEFT_0, 1);
			setOutputPin (EN_SOD_MOTOR_LEFT_1, 0);
			break;
		case EN_STATE_BREAK_LEFT:
			setOutputPin (EN_SOD_MOTOR_RIGHT_0, 0);
			setOutputPin (EN_SOD_MOTOR_RIGHT_1, 1);
			setOutputPin (EN_SOD_MOTOR_LEFT_0, 1);
			setOutputPin (EN_SOD_MOTOR_LEFT_1, 1);
			break;
		case EN_STATE_STEERING_RIGHT:
			setOutputPin (EN_SOD_MOTOR_RIGHT_0, 1);
			setOutputPin (EN_SOD_MOTOR_RIGHT_1, 0);
			setOutputPin (EN_SOD_MOTOR_LEFT_0, 0);
			setOutputPin (EN_SOD_MOTOR_LEFT_1, 1);
			break;
		case EN_STATE_BREAK_RIGHT:
			setOutputPin (EN_SOD_MOTOR_RIGHT_0, 1);
			setOutputPin (EN_SOD_MOTOR_RIGHT_1, 1);
			setOutputPin (EN_SOD_MOTOR_LEFT_0, 0);
			setOutputPin (EN_SOD_MOTOR_LEFT_1, 1);
			break;
		default:
		break;
	}

	if (motorSpeed_u8>0)
	{
		setOutputPin(EN_SODPWM_ENABLE_MOTOR1, motorSpeed_u8);
		setOutputPin(EN_SODPWM_ENABLE_MOTOR2, motorSpeed_u8);
	}
	else
	{
		setOutputPin(EN_SODPWM_ENABLE_MOTOR1, 0);
		setOutputPin(EN_SODPWM_ENABLE_MOTOR2, 0);
	}

}

int getMotorDirection()
{
	if (MOTOR_DIRECTION == EN_STATE_FORWARD)
	{
		return 1;
	}
	if (MOTOR_DIRECTION == EN_STATE_BACKWARD)
	{
		return 0;
	}
}

int getMotorSpeed()
{
	if (motorSpeed_u8 > 0)
	{
		return 1;
	}
	return 0;
}
