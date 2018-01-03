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
 * @file watchdog.c
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


#include "IO_extern.h"
#include "custom_types.h"
#include "watchdog.h"
#include "watchdog_extern.h"

flags8 flags8_watchdog;

#define WATCHDOG_TIMER_INTERVAL (3000u/1000u)
#define INPUT_CHANGE_DETECTION flags8_watchdog.bits.b0
#define CHANGED 1
#define NOT_CHANGED 0

typedef enum en_wd_states {
	EN_WD_INIT ,
	EN_WD_HIGH ,
	EN_WD_LOW ,
	EN_WD_NUMBER_OF_STATES,
} EN_WD_STATES;

typedef enum en_wd_request_tates {
	EN_WD_REQ_HIGH ,
	EN_WD_REQ_LOW ,
	EN_WD_REQ_NUMBER_OF_STATES,
} EN_WD_REQ_STATES;

EN_WD_STATES wdStates_en;
EN_WD_REQ_STATES wdReqStates_en;

int watchdogTimer = 0;
/**
* @brief Implementation of function that handles the initialization of light control
*
* implementation of function that handles the initialization of light control
* @return void
* @note Void function with no return.
*/
void watchdogInit()
{
	wdStates_en = EN_WD_LOW;
}

/**
* @brief Implementation of function that handles the light control
*
* Implementation of function that handles the light control
* @return void
* @note Void function with no return.
*/
void watchdogCyclic()
{
	watchdogInputsAQ();
	watchdogSM();
	watchdogActuator();
}

void watchdogInputsAQ()
{

	if(GetInputPin(EN_SID_RPI_CHECK))
	{
		wdReqStates_en = EN_WD_REQ_HIGH;
	}
	else
	{
		wdReqStates_en = EN_WD_REQ_LOW;
	}
}

/**
* @brief Implementation of function that handles the watchdog state machine
*
* Implementation of function that handles the watchdog control
* @return void
* @note Void function with no return.
*/
void watchdogSM()
{
	switch (wdStates_en) {
		case EN_WD_HIGH:
			switch (wdReqStates_en) {
				case EN_WD_REQ_LOW:
					INPUT_CHANGE_DETECTION = CHANGED;
					wdStates_en = EN_WD_LOW;
				break;
				case EN_WD_REQ_HIGH:
					INPUT_CHANGE_DETECTION = NOT_CHANGED;
					wdStates_en = EN_WD_HIGH;
				break;
				default:
				break;
			}
		break;
		case EN_WD_LOW:
			switch (wdReqStates_en) {
				case EN_WD_REQ_HIGH:
					INPUT_CHANGE_DETECTION = CHANGED;
					wdStates_en = EN_WD_HIGH;
				break;
				case EN_WD_REQ_LOW:
					INPUT_CHANGE_DETECTION = NOT_CHANGED;
					wdStates_en = EN_WD_LOW;
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
}

/**
* @brief Implementation of function that handles the watchdog actuator
*
* Implementation of function that handles the watchdog actuator
* @return void
* @note Void function with no return.
*/
void watchdogActuator()
{


	if (INPUT_CHANGE_DETECTION == CHANGED)
	{
		watchdogTimer = WATCHDOG_TIMER_INTERVAL;
	}
	else
	{
		if (watchdogTimer > 0)
		{
		watchdogTimer--;
	  }
	}
	Serial.println(watchdogTimer);

	if (watchdogTimer <= 0)
	{
		setOutputPin(EN_SODPWM_ENABLE_MOTOR1, 0);
		setOutputPin(EN_SODPWM_ENABLE_MOTOR2, 0);
	}
}

int getWatchdogState()
{
	if (watchdogTimer <= 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
