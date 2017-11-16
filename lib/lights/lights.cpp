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
 * @file lights.c
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


#include "lights.h"
#include "IO_extern.h"

/**
 * @brief Enumeration of low beam states
 *
 * Enumeration of low beam states.
 */
typedef enum en_low_beam_states {
	EN_LB_STATE_OFF,				/**< low beam Off */
	EN_LB_STATE_ON,				/**< low beam left side */
	EN_LB_NUMBER_OF_ELEMENTS_STATES ,      /**< Number of states*/
} EN_LOW_BEAM_STATES;


EN_LOW_BEAM_STATES lowBeamState_en;
/**
* @brief Implementation of function that handles the initialization of light control
*
* mplementation of function that handles the initialization of light control
* @return void
* @note Void function with no return.
*/
void lightsInit()
{
	lowBeamState_en = EN_LB_STATE_OFF;
}

/**
* @brief Implementation of function that handles the light control
*
* Implementation of function that handles the light control
* @return void
* @note Void function with no return.
*/
void lightsCyclic()
{
	lightsInputsAQ();
	lightsSM();
	lightsActuator();
}

void lightsInputsAQ()
{

}

/**
* @brief Implementation of function that handles the lights state machine
*
* Implementation of function that handles the lights control
* @return void
* @note Void function with no return.
*/
void lightsSM()
{

}

/**
* @brief Implementation of function that handles the lights actuator
*
* Implementation of function that handles the lights actuator
* @return void
* @note Void function with no return.
*/
void lightsActuator()
{
	
}
