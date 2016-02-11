
/**
* @brief Implementation of bit field
*
* mplementation of function that handles the initialization of motor control
**/


struct BIT {
	uint8_t  b0     :1;  // bit 0 single bit
	uint8_t  b1     :1;  // bit 1 single bit
	uint8_t  b2		:1;  // bit 2 single bit
	uint8_t  b3		:1;  // bit 3 single bit
	uint8_t  b4		:1;  // bit 4 single bit
	uint8_t  b5		:1;  // bit 5 single bit
	uint8_t  b6		:1;  // bit 6 single bit
	uint8_t  b7		:1;  // bit 7 single bit
};


/**
* @brief Implementation of union bit field
*
* mplementation of function that handles the initialization of motor control
**/

union flags8 {
	uint8_t  byte;
	struct BIT bits;
};


/**
 * @brief Enumeration of movement states
 *
 * Enumeration of movement states.
 */
typedef enum en_movement_states {
	EN_STATE_ACCELERATION ,				/**< Acceleration */
	EN_STATE_DECELERATION ,				/**< Breaking */
	EN_STATE_STEERING_LEFT ,			/**< ST Left*/
	EN_STATE_STEERING_RIGHT,			/**< ST Right*/
	EN_STATE_IDLE ,						/**< Idle */
	EN_NUMBER_OF_ELEMENTS_STATES ,      /**< Number of states*/
} EN_MOVEMENT_STATES;


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

#define FLAG_DIRECTION_UP flags8_motors_u.bits.b0
#define FLAG_DIRECTION_DOWN flags8_motors_u.bits.b1
#define FLAG_DIRECTION_LEFT flags8_motors_u.bits.b2
#define FLAG_DIRECTION_RIGHT flags8_motors_u.bits.b3
#define FLAG_STEERING_LEFT_ACTIVE flags8_motors_u.bits.b4
#define FLAG_STEERING_RIGHT_ACTIVE flags8_motors_u.bits.b5
#define UNUSED6 flags8_motors_u.bits.b6
#define UNUSED7 flags8_motors_u.bits.b7

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
#define MOTOR_SPEED_STEP_DOWN 1u

	
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
	flags8_motors_u.byte=0;
	motorSpeed_u8=0;
	
	
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

	if(GetLogicalInput(EN_SID_WIFI_CONTROL_UP)){
		FLAG_DIRECTION_UP=1;
	}
	
	else{
		FLAG_DIRECTION_UP=0;
	}
	
	
	if(GetLogicalInput(EN_SID_WIFI_CONTROL_DOWN)){
		FLAG_DIRECTION_DOWN=1;
	}
	
	else{
		FLAG_DIRECTION_DOWN=0;
	}
	
	if(GetLogicalInput(EN_SID_WIFI_CONTROL_LEFT)){
		FLAG_DIRECTION_LEFT=1;
	}
	
	else{
		FLAG_DIRECTION_LEFT=0;
	}
	
	if(GetLogicalInput(EN_SID_WIFI_CONTROL_RIGHT)){
		FLAG_DIRECTION_RIGHT=1;
	}
	
	else{
		FLAG_DIRECTION_RIGHT=0;
	}
	

	if (FLAG_DIRECTION_LEFT)
	{
		if(FLAG_DIRECTION_RIGHT)
		{
			//keep last state
		}
		else
		{
			motorStates_en=EN_STATE_STEERING_LEFT;
		}
	}
	else 
	{
		if (FLAG_DIRECTION_RIGHT)
		{
			motorStates_en=EN_STATE_STEERING_RIGHT;
		}
		else
		{
			if (FLAG_DIRECTION_DOWN)
			{
				motorStates_en=EN_STATE_DECELERATION;
			}
			else
			{
				if (FLAG_DIRECTION_UP)
				{
					motorStates_en=EN_STATE_ACCELERATION;
				}
				else
				{
					motorStates_en=EN_STATE_IDLE;
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
	
	
	switch(motorStates_en)
	{
		case EN_STATE_ACCELERATION:
			if ((motorSpeed_u8+MOTOR_SPEED_STEP_UP)<=MAX_MOTOR_SPEED)
			{
				
				motorSpeed_u8+=MOTOR_SPEED_STEP_UP;
			}
			else
			{
				
				motorSpeed_u8=MAX_MOTOR_SPEED;
			}
		break;
		case EN_STATE_DECELERATION:
			if ((motorSpeed_u8-MOTOR_SPEED_STEP_DOWN)>=0)
			{
				
				motorSpeed_u8-=MOTOR_SPEED_STEP_DOWN;
			
			}
			else
			{
				motorSpeed_u8=0;
			}
		break;
		case EN_STATE_STEERING_LEFT:
				FLAG_STEERING_LEFT_ACTIVE=1;
				FLAG_STEERING_RIGHT_ACTIVE=0;
		break;
		case EN_STATE_STEERING_RIGHT:
				FLAG_STEERING_LEFT_ACTIVE=0;
				FLAG_STEERING_RIGHT_ACTIVE=1;
		break;
		default:
				FLAG_STEERING_LEFT_ACTIVE=0;
				FLAG_STEERING_RIGHT_ACTIVE=0;
		break;
	}		
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
	
	if (FLAG_STEERING_LEFT_ACTIVE)
	{
		setLogicalOutput(EN_SODPWM_ENABLE_MOTOR1, 0);
		setLogicalOutput(EN_SODPWM_ENABLE_MOTOR2, motorSpeed_u8);
	}
	else
	{
		if (FLAG_STEERING_RIGHT_ACTIVE)
		{
			setLogicalOutput(EN_SODPWM_ENABLE_MOTOR1, motorSpeed_u8);
			setLogicalOutput(EN_SODPWM_ENABLE_MOTOR2, 0);
		}
		else
		{
			setLogicalOutput(EN_SODPWM_ENABLE_MOTOR1, motorSpeed_u8);
			setLogicalOutput(EN_SODPWM_ENABLE_MOTOR2,motorSpeed_u8);
		}
	}
	
	
	if (motorSpeed_u8>0)
	{
		setLogicalOutput(EN_SOD_MOTOR12_0, 1);
		setLogicalOutput(EN_SOD_MOTOR12_1, 0);
		setLogicalOutput(EN_SOD_MOTOR12_2, 1);
		setLogicalOutput(EN_SOD_MOTOR12_3, 0);
	}
	else
	{
		setLogicalOutput(EN_SOD_MOTOR12_0, 0);
		setLogicalOutput(EN_SOD_MOTOR12_1, 0);
		setLogicalOutput(EN_SOD_MOTOR12_2, 0);
		setLogicalOutput(EN_SOD_MOTOR12_3, 0);
	}
}

