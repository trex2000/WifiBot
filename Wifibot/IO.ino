

/**
 * @brief Defines maximum number of analog channels
 *
 * Defines maximum number of analog channels
 */
#define MAX_ADC_CHANNELS 6u 


/**
 * @brief Motor 1 Pin numbers
 *
 * Defines the HW pin numbers where the motor 1 is connected
 */
#define HW_INPUT_dir1PinA 2
#define HW_INPUT_dir2PinA 3

/**
 * @brief Motor 2 Pin numbers
 *
 * Defines the HW pin numbers where the motor 2 is connected
 */
#define HW_INPUT_dir1PinB 4
#define HW_INPUT_dir2PinB 5


/**
 * @brief Motor 1,2 PWM Pin numbers
 *
 * Defines the HW pin numbers where the 2 enable pins are connected. 
 * These must be PWM capable pins to be able to control the speed of the motor
 */
#define HW_INPUT_speedPinA 9 
#define HW_INPUT_speedPinB 10

/**
 * @brief buffer holds the value of the inputs.
 *
 * Detailed buffer holds the value of the inputs.
 */
uint8_t inputBuffer_u8[EN_NUMBER_OF_ELEMENTS_INPUT];

/**
 * @brief array holds the results of ADC conversion
 *
 * array holds the results of ADC conversion
 */

uint16_t adc_Result_u16[MAX_ADC_CHANNELS];

/**
 * @brief buffer holds the value of the outputs.
 *
 * Detailed buffer holds the value of the outputs.
 */
uint8_t outputBuffer_u8[EN_NUMBER_OF_ELEMENTS_OUTPUTS];


/**
 * @brief Init function of Input/Output module
 *
 * Function implementation for input module initialization
 * @return void
 */
void  initIO()
{
  pinMode(HW_INPUT_dir1PinA,OUTPUT);
  pinMode(HW_INPUT_dir2PinA,OUTPUT);
  pinMode(HW_INPUT_dir1PinB,OUTPUT);
  pinMode(HW_INPUT_dir2PinB,OUTPUT);
  pinMode(HW_INPUT_speedPinA,OUTPUT);
  pinMode(HW_INPUT_speedPinB,OUTPUT);
}


/**
 * @brief Function definition for processing input buffer
 *
 * Function implementation for processing input buffer.
 * @return void
 */
void processInputBuffer()
{
  
}


/**
 * @brief Function definition for processing output buffer
 *
 * Function implementation for processing output buffer.
 * For functions, automatic links are generated when the parenthesis () follow
 * the name of the function, like Box_The_Function_Name().
 * Alternatively, you can use #Box_The_Function_Name.
 * @return void
 */
void processOutputBuffer()
{
  uint8_t forIdx_lu8=0;
  for (forIdx_lu8=0; forIdx_lu8<EN_NUMBER_OF_ELEMENTS_OUTPUTS; forIdx_lu8++)
  {
  }
}


/**
 * @brief Function processes ADC conversion
 *
 * Function processes ADC conversion
 * @return void
 */
void processADCconversion()
{
}



/**
 * @brief Returns status of logical Input Pin.
 *
 * @param pinId_en logical input pin name. range: EN_INPUT_PINS
 * @return @c state of the pin.
 */
uint8_t GetLogicalInput (EN_INPUT_PINS pinId_en)
{	
  if(pinId_en>=EN_NUMBER_OF_ELEMENTS_INPUT)
  {
     return 0;
  }
  else
  {
      return inputBuffer_u8[pinId_en];	
  }
}

/**
 * @brief Sets the logical Output
 *
  * @param pinId_en logical output pin name. range: EN_OUPUT_PINS
 * @return @c state of the pin.
 */
void setLogicalOutput (EN_OUTPUT_PINS pinId_en, uint8_t value_u8) 
{

  if (pinId_en>=EN_NUMBER_OF_ELEMENTS_OUTPUTS) /**< tests if the pin is out of range */
  {
 	// do nothing
  }
  else
  {
        outputBuffer_u8[pinId_en]=value_u8;
  }
}
