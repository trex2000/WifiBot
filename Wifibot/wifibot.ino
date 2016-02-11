
/**
 * @brief Enumeration of logical inputs
 *
 * Enumeration of logical inputs.
 */
typedef enum en_input_pins 
{
  EN_SIA_LIGHTSENSOR ,				/**< Lightsensor analogic input */
  EN_SID_WIFI_CONTROL_UP ,			/**< WifiControl Up. */
  EN_SID_WIFI_CONTROL_DOWN ,          /**< WifiControl Down*/
  EN_SID_WIFI_CONTROL_RIGHT ,         /**< WifiControl Right*/
  EN_SID_WIFI_CONTROL_LEFT ,          /**< WifiControl Left */
  EN_NUMBER_OF_ELEMENTS_INPUT ,       /**< Number of input pins*/
} EN_INPUT_PINS;


/**
 * @brief Enumeration of logical outputs
 *
 * Enumeration of logical outputs.
 */
typedef enum en_output_pins 
{
  EN_SOD_MOTOR12_0,					/**< Motor 12 digital output 1 */
  EN_SOD_MOTOR12_1,				    /**< Motor 12 digital output 2 */
  EN_SOD_MOTOR12_2,				    /**< Motor 12 digital output 3 */
  EN_SOD_MOTOR12_3,					/**< Motor 12 digital output 4 */
  EN_SODPWM_ENABLE_MOTOR1,			/**< Digital out PWM on  Motor1 */
  EN_SODPWM_ENABLE_MOTOR2,			/**< Digital out PWM on  Motor2 */
  EN_SOD_LEFT_FLASHER,				/**< Digital out for Left Flasher */
  EN_SOD_RIGHT_FLASHER,				/**< Digital out for Right Flasher */
  EN_SOD_LOW_BEAM,					/**< Digital out for Low Beam */
  EN_NUMBER_OF_ELEMENTS_OUTPUTS       /**< Number of output pins  */
} EN_OUTPUT_PINS;

/**
 * @brief Function prototypes
 *
 * Function Prototypes
 */
uint8_t GetLogicalInput (EN_INPUT_PINS pinId_en);
void setLogicalOutput (EN_OUTPUT_PINS pinId_en, uint8_t value_u8);

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
}  

/**
 * @brief Cyclic loop
 *
 * Put all functions to be called cyclically
 */
void loop() 
{
 processADCconversion();
 processInputBuffer();
 motorsCyclic();
 lightsCyclic();
 processOutputBuffer();
}
