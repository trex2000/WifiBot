
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
	EN_NUMBER_OF_ELEMENTS_STATES       /**< Number of states*/
} EN_MOVEMENT_STATES;



/**
 * @brief Enumeration of input states
 *
 * Enumeration of input states..
 */
typedef enum en_input_states {
	EN_INPUT_STATE_UP ,				/**< Acceleration */
	EN_INPUT_STATE_DOWN ,				/**< Breaking */
	EN_INPUT_STATE_LEFT ,	          		/**< ST Left*/
	EN_INPUT_STATE_RIGHT,		        	/**< ST Right*/
	EN_INPUT_STATE_UP_LEFT,			        /**< ST Left + acceleration*/
	EN_INPUT_STATE_UP_RIGHT,			/**< ST Right + acceleration */
	EN_INPUT_STATE_DOWN_LEFT,		        /**< ST Left + braking*/
	EN_INPUT_STATE_DOWN_RIGHT,			/**< ST right + braking*/
	EN_INPUT_STATE_IDLE,			        /**< ST right + braking*/
	EN_INPUT_STATE_INVALID,				/**< Idle */
	EN_NUMBER_OF_ELEMENTS_INPUT_STATES            /**< Number of states*/
} EN_INPUT_STATES;

/**
 * @brief Number of control inputs
 *
 * enumeration of control inputs (4 directional keys, next element in enum is EN_SIA_LIGHTSENSOR)
 */
#define MAX_CONTROL_INPUTS   EN_SIA_LIGHTSENSOR

/*
#if MAX_CONTROL_INPUTS>=EN_NUMBER_OF_ELEMENTS_INPUT
  #error MAX_CONTROL_INPUTS shall be less than the maximum available inputs (EN_NUMBER_OF_ELEMENTS_INPUT)
#endif
*/

/**
 * @brief Input combination truth table
 
 *  Input combination truth table     
 */
EN_INPUT_STATES inputStatesTruthTable[1u<<MAX_CONTROL_INPUTS]=
{
  /**< WifiControl Up.  WifiControl Down, WifiControl Right, WifiControl Left */ 
  EN_INPUT_STATE_IDLE,      /**< 0:    0 0 0 0  - all off */ 
  EN_INPUT_STATE_UP,        /**< 1:    0 0 0 1  - up only*/  
  EN_INPUT_STATE_DOWN,      /**< 2:    0 0 1 0  - down only */ 
  EN_INPUT_STATE_INVALID,   /**< 3:    0 0 1 1  - simultaineous up and down*/ 
  EN_INPUT_STATE_RIGHT,     /**< 4:    0 1 0 0  - steering right only */ 
  EN_INPUT_STATE_UP_RIGHT,  /**< 5:    0 1 0 1  - steering right while accelerating*/ 
  EN_INPUT_STATE_DOWN_RIGHT,/**< 6:    0 1 1 0  - steering right while braking */ 
  EN_INPUT_STATE_INVALID,   /**< 7:    0 1 1 1  - simulataineous right, up and down*/ 
  EN_INPUT_STATE_LEFT,      /**< 8:    1 0 0 0  - left only*/ 
  EN_INPUT_STATE_UP_LEFT,   /**< 9:    1 0 0 1 - steering left while accelerating*/ 
  EN_INPUT_STATE_DOWN_LEFT, /**< A:    1 0 1 0 - steering left while braking*/ 
  EN_INPUT_STATE_INVALID,   /**< B:    1 0 1 1 */ 
  EN_INPUT_STATE_INVALID,   /**< C:    1 1 0 0 -  simulataineous left and right*/ 
  EN_INPUT_STATE_INVALID,   /**< D:    1 1 0 1 -  simulataineous left, right and up*/ 
  EN_INPUT_STATE_INVALID,   /**< E:    1 1 1 0  - simulataineous left, right and  down*/ 
  EN_INPUT_STATE_INVALID,   /**< F:    1 1 1 1  - all pressed*/   
};

/**
 * @brief State of the input aquisition output
 *
 *  After the input aquistion function processes the physical input
 *  combinations, one state will be chosen from the enum above.
 */
EN_INPUT_STATES currentInputState_en;
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
  currentInputState_en = EN_INPUT_STATE_IDLE; 
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
  uint8_t inputVal_lu8=0, inputs_lu8;
  uint8_t forIdx_lu8;
  
  for (forIdx_lu8=0; forIdx_lu8 <MAX_CONTROL_INPUTS; forIdx_lu8++)
  { 
     inputVal_lu8 = (uint8_t)(0x0001u&GetLogicalInput((EN_INPUT_PINS)forIdx_lu8)); /*only bit 0 is relevant, as inputs are digital*/
     inputs_lu8 = (inputs_lu8 | (inputVal_lu8<<forIdx_lu8));
  }  
  if (inputs_lu8 < (1u<<MAX_CONTROL_INPUTS))
  {
     currentInputState_en = inputStatesTruthTable[inputs_lu8];
  }
  else
  {
    /*something wrong has occured, as index is out of range*/ 
    currentInputState_en = EN_INPUT_STATE_INVALID;
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
		break;
		case EN_STATE_STEERING_RIGHT:
		break;
		default:
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
	
	if (motorSpeed_u8>0)
	{
		setLogicalOutput(EN_SOD_MOTORA_1, 1);
		setLogicalOutput(EN_SOD_MOTORA_2, 0);
		setLogicalOutput(EN_SOD_MOTORB_1, 1);
		setLogicalOutput(EN_SOD_MOTORB_2, 0);
	}
	else
	{
		setLogicalOutput(EN_SOD_MOTORA_1, 0);
		setLogicalOutput(EN_SOD_MOTORA_2, 0);
		setLogicalOutput(EN_SOD_MOTORB_1, 0);
		setLogicalOutput(EN_SOD_MOTORB_2, 0);
	}
}

