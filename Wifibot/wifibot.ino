#include <TaskScheduler.h>


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
 * @brief Enumeration of logical inputs
 *
 * Enumeration of logical inputs.
 */
typedef enum en_input_pins
{
  EN_SID_WIFI_CONTROL_UP ,	      /**< WifiControl Up. */
  EN_SID_WIFI_CONTROL_DOWN ,          /**< WifiControl Down*/
  EN_SID_WIFI_CONTROL_RIGHT ,         /**< WifiControl Right*/
  EN_SID_WIFI_CONTROL_LEFT ,          /**< WifiControl Left */
  EN_SIA_LIGHTSENSOR ,		      /**< Lightsensor analogic input */
  EN_NUMBER_OF_ELEMENTS_INPUT        /**< Number of input pins*/
} EN_INPUT_PINS;


/**
 * @brief Enumeration of logical outputs
 *
 * Enumeration of logical outputs.
 */
typedef enum en_output_pins
{
  EN_SOD_MOTORA_1,				/**< Motor A In 1 */
  EN_SOD_MOTORA_2,				/**< Motor A In 2 */
  EN_SOD_MOTORB_1,				/**< Motor B In 3 */
  EN_SOD_MOTORB_2,				/**< Motor B In 4 */
  EN_SODPWM_ENABLE_MOTOR1,			/**< Digital out PWM on  Motor1 */
  EN_SODPWM_ENABLE_MOTOR2,			/**< Digital out PWM on  Motor2 */
  EN_SOD_LEFT_FLASHER,				/**< Digital out for Left Flasher */
  EN_SOD_RIGHT_FLASHER,				/**< Digital out for Right Flasher */
  EN_SOD_LOW_BEAM,				/**< Digital out for Low Beam */
  EN_NUMBER_OF_ELEMENTS_OUTPUTS                 /**< Number of output pins  */
} EN_OUTPUT_PINS;


/**
 * @brief Defines type of physical port
 *
 * Defines type of physical port
 */
typedef enum flasher_states
{
  EN_STATE_FLA_OFF,		   /**< No flasing */
  EN_STATE_FLA_LEFT,		   /**< Left turn indication active */
  EN_STATE_FLA_RIGHT,		   /**< Right turn indication active */
  EN_STATE_FLA_HAZARD,		   /**< Hazard flashing active */
  EN_STATE_FLA_LAST_ELEMENT	   /**< Number of states */
} EN_FLASHER_STATES;


/**
 * @brief Function prototypes
 *
 * Function Prototypes
 */
uint16_t GetLogicalInput (EN_INPUT_PINS pinId_en);
void setLogicalOutput (EN_OUTPUT_PINS pinId_en, uint8_t value_u8);
void setFlasherActive(EN_FLASHER_STATES flaSta_en);
/**
 * @brief Task scheduler entries
 *
 * Here you can define the cyclic tasks
 */
Task t_Task20ms(20, TASK_FOREVER, &task20ms);
Task t_Task40ms(40, TASK_FOREVER, &task40ms);
Task t_Task60ms(60, TASK_FOREVER, &task60ms);
Task t_Task100ms(60, TASK_FOREVER, &task100ms);
Task t_Task1000ms(60, TASK_FOREVER, &task1000ms);

/**
 * @brief Task scheduler runner
 *
 * Task scheduler Runner. Don't forget to add the tasks and start them
 */
Scheduler t_runner;


/*
 * Define this to provide debugging over serial port. Use the
 * AddSerialDebugData() function to push data into the buffer.
 * The values will be sent out cyclically every 1s
*/
#define ENABLE_DEBUG


/*
 * @brief Serial debug
 *
 * Define above to provide debugging over serial port. Use the
 * AddSerialDebugData() function to push data into the buffer.
 * The values will be sent out cyclically every 1s
 */
#ifdef ENABLE_DEBUG
#define MAX_DEBUG_ARR_SIZE 255u
/*
 * @brief Used to provide debug data through serial port
 *
 * Array of tasks and interval of execution
 */
uint8_t debugBuffer_au8[MAX_DEBUG_ARR_SIZE];
uint8_t debugArrIndex_u8 = 0;
#endif





/**
* @brief Implementation of function that handle the 20ms requests
*
* Implementation of function that handle the 20ms requests
* @return void
* @note Void function with no return.
*/
void task20ms(void)
{
  processInputBuffer();
  processOutputBuffer();
}


/**
* @brief Implementation of function that handle the 40ms requests
*
* Implementation of function that handle the 40ms requests
* @return void
* @note Void function with no return.
*/
void task40ms(void)
{
  motorsCyclic();
};

/**
* @brief Implementation of function that handle the 60ms requests
*
* Implementation of function that handle the 60ms requests
* @return void
* @note Void function with no return.
*/
void task60ms(void)
{
  lightsCyclic();
};

/**
* @brief Implementation of function that handle the 100ms requests
*
* Implementation of function that handle the 100ms requests
* @return void
* @note Void function with no return.
*/
void task100ms(void)
{
  processADCconversion();
};

/**
* @brief Implementation of function that handle the 1000ms requests
*
* Implementation of function that handle the 1000ms requests
* @return void
* @note Void function with no return.
*/
void task1000ms(void)
{
  /*
  #ifdef ENABLE_DEBUG
  static uint8_t value_lu8 = 'a';
  AddSerialDebugData(value_lu8);
  value_lu8++;
  if (value_lu8 =='z')
  {
    value_lu8 = 'a';
  }
  else
  {
  }
  #endif
  */
  processSerialDebugData();
}


/**
 * @brief Init function
 *
 * Put all functions to be called cyclically
 */
void setup()
{
  initIO();
  motorsInit();
  lightsInit();
#ifdef ENABLE_DEBUG
  Serial.begin(115200);
  Serial.println("Serial Debug has started");
#endif
  /*Initialize the task scheduler*/
  t_runner.init();
  t_runner.addTask(t_Task20ms);
  t_runner.addTask(t_Task40ms);
  t_runner.addTask(t_Task60ms);
  t_runner.addTask(t_Task100ms);
  t_runner.addTask(t_Task1000ms);
  /*Start the cyclic tasks*/
  t_Task20ms.enable();
  t_Task40ms.enable();
  t_Task60ms.enable();
  t_Task100ms.enable();
  t_Task1000ms.enable();
}

/**
 * @brief Cyclic loop
 *
 * Put all functions to be called cyclically
 */
void loop()
{
  t_runner.execute();
}



#ifdef ENABLE_DEBUG
/**
 * @brief Add debug data to buffer that will be sent out through serial port
 *
 * Implementation of the function that handle timer1 overflow ISR
 * @return 0: not success, 1: success
 *
 */
uint8_t AddSerialDebugData(uint8_t Value_lu8)
{
  if (debugArrIndex_u8 < 255)
  {
    debugBuffer_au8[debugArrIndex_u8] = Value_lu8;
    debugArrIndex_u8++;
    return 1u;
  }
  else
  {
    //buffer full
    return 0u;
  }
}

/**
 * @brief Function will try to send out debug info through serial port
 * *
 * @return void
 *
 */
void processSerialDebugData()
{
  uint8_t data_lu8;
  if (debugArrIndex_u8 > 1)
  {
    if (Serial.availableForWrite() >= 1)
    {

      data_lu8 = debugBuffer_au8[debugArrIndex_u8 - 1];
      Serial.write(data_lu8);
      debugArrIndex_u8--;
    }
    else
    {
      //buffer is full, wait for a round
    }
  }
  else
  {
    //nothing to send out
  }
}
#endif

