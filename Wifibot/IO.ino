
/**
 * @brief Defines type of physical port
 *
 * Defines type of physical port
 */
typedef enum port_type_enum
{
	EN_PORT_DI,				/**< Digital Input port */
	EN_PORT_DI_W_PUP,		        /**< Digital Input port with internal 20k pullup*/
	EN_PORT_DO,				/**< Digital Output port */
	EN_PORT_AI,				/**< Analog Input port */
	EN_PORT_DOPWM,			/**< Digital PWM Output port */
	EN_PORT_LAST_ELEMENT	/**< Number of port types */
} PORT_TYPES_EN;


/**
 * @brief Defines the states for ADC conversion
 *
 * Defines the states for ADC conversion
 */
typedef enum en_conversion_state{
	EN_CONVERSION_IN_PROGRESS,		/**< Current conversion in progress  */
	EN_CONVERSION_IDLE				/**< No conversion in progress */
} EN_CONVERSION_STATE;

/**
 * @brief New datatype used in table which connects Logical Input Definitions to Physical Input Def
 *
 * New datatype used in table which connects Logical Input Definitions to Physical Input Def
 */
typedef struct port_types_struct
{
	uint8_t portVal_u8;				/**< Bit position of port */
	PORT_TYPES_EN portType_en;		/**< Type of the port ( Digital, Analog..) */
} PORT_TYPES_ST;


/**
 * @brief New datatype used in table which connects Logical Outputs Definitions to Physical Outputs Def
 *
 * New datatype used in table which connects Logical Outputs Definitions to Physical Outputs Def
 */
PORT_TYPES_ST const matchingTableOutputPins_acst[EN_NUMBER_OF_ELEMENTS_OUTPUTS] = {
	{PORTB2, EN_PORT_DO},     /**<SOD_MOTORA_1 */
	{PORTB1, EN_PORT_DO},	 /** <SOD_MOTORA_2 */
	{PORTD5, EN_PORT_DO},     /**<SOD_MotorB_1 */
	{PORTD4, EN_PORT_DO},     /**<SOD_MotorB_2 */
	{PORTB3, EN_PORT_DOPWM},  /**<SODPWM_EnableMotor1 */
	{PORTD3, EN_PORT_DOPWM},  /**<ODPWM_EnableMotor2 */
	{PORTC2, EN_PORT_DO},     /**<SOD_LeftFlasher */
	{PORTC1, EN_PORT_DO},     /**<SOD_RightFlasher */
	{PORTD6, EN_PORT_DO}      /**<SOD_LowBeam */
};
/**
 * @brief New datatype used in table which connects Logical Input Definitions to Physical Input Def
 *
 * New datatype used in table which connects Logical Input Definitions to Physical Input Def
 */
PORT_TYPES_ST const matchingTableInputPins_acst[EN_NUMBER_OF_ELEMENTS_INPUT] = {
	{PORTD7, EN_PORT_DI}, 	 /**< EN_SID_WIFI_CONTROL_UP */
	{PORTB0, EN_PORT_DI},	 /**< EN_SID_WIFI_CONTROL_DOWN */
	{PORTB5, EN_PORT_DI},	 /**< EN_SID_WIFI_CONTROL_RIGHT */
	{PORTB4, EN_PORT_DI},	     /**< EN_SID_WIFI_CONTROL_LEFT */
	{A0,     EN_PORT_AI},     /**< EN_SIA_LIGHTSENSOR */
};




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
uint16_t inputBuffer_u16[EN_NUMBER_OF_ELEMENTS_INPUT];

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
void initIO()
{
  uint8_t forIdx_lu8;
  /*setup input ports*/
  for (forIdx_lu8; forIdx_lu8 < EN_NUMBER_OF_ELEMENTS_INPUT; forIdx_lu8)
  {
    switch (matchingTableInputPins_acst[forIdx_lu8].portType_en)    
    {
      case EN_PORT_AI:
        pinMode(matchingTableInputPins_acst[forIdx_lu8].portVal_u8,INPUT);
      break;
      
      case EN_PORT_DI_W_PUP:
        pinMode(matchingTableInputPins_acst[forIdx_lu8].portVal_u8,INPUT_PULLUP);
      break;
      
      case EN_PORT_DI:              
      default:
        pinMode(matchingTableInputPins_acst[forIdx_lu8].portVal_u8,INPUT);
      break;     
      
    }
  }

  /*setup output ports*/
  for (forIdx_lu8; forIdx_lu8 < EN_NUMBER_OF_ELEMENTS_OUTPUTS; forIdx_lu8)
  {
      switch(matchingTableOutputPins_acst[forIdx_lu8].portType_en)
      {
        case EN_PORT_DO:
        case EN_PORT_DOPWM:
            pinMode(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8, OUTPUT);
        break;
        default:
            /*Some incorrect configuration, set the pin to input for safety*/
            pinMode(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8,INPUT);
        break;
      }      
  }
}


/**
 * @brief Function definition for processing input buffer
 *
 * Function implementation for processing input buffer.
 * @return void
 */
void processInputBuffer()
{
   uint8_t forIdx_lu8=0;
  /*process input ports*/
  for (forIdx_lu8; forIdx_lu8 < EN_NUMBER_OF_ELEMENTS_INPUT; forIdx_lu8)
  {
    switch (matchingTableInputPins_acst[forIdx_lu8].portType_en)    
    {
      case EN_PORT_AI:
           inputBuffer_u16[forIdx_lu8] = adc_Result_u16 [forIdx_lu8];
      break;
      
      case EN_PORT_DI_W_PUP:
      case EN_PORT_DI:              
           inputBuffer_u16[forIdx_lu8] = digitalRead(matchingTableInputPins_acst[forIdx_lu8].portVal_u8);
      break;
      
      
      default: /*do nothing*/
      break;     
      
    }
  }
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
     switch(matchingTableOutputPins_acst[forIdx_lu8].portType_en)
     {
       case EN_PORT_DOPWM:
              analogWrite(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8, outputBuffer_u8[forIdx_lu8]);  /*write the output */
       break;
       case EN_PORT_DO:
            if (outputBuffer_u8[forIdx_lu8]>0)
            {
                digitalWrite(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8,1);
            }
            else
            {
                digitalWrite(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8,0);
            }
            
       break;
       default:
           /*some invalid configuration has been set, it's better to disable the output*/
           digitalWrite(matchingTableOutputPins_acst[forIdx_lu8].portVal_u8,0);
       break;
     }     
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
  static uint8_t inputIdx=0, overflowFlg = 0;
    
  while(matchingTableInputPins_acst[inputIdx].portType_en != EN_PORT_AI)
  {
    inputIdx++;     
    if (inputIdx>=EN_NUMBER_OF_ELEMENTS_INPUT)
    {
      /*start from beginning*/
      if (!overflowFlg)
      {
        inputIdx = 0;
        overflowFlg = 1;
      }
      else
      {
        /*no analog input was found*/
        break;
      }      
    }
    else
    {
      /**/
      inputIdx++;
    }      
  }
  /*check if there was an analog input found*/
  if (inputIdx < EN_NUMBER_OF_ELEMENTS_INPUT)
  {
    adc_Result_u16[inputIdx] = analogRead(matchingTableInputPins_acst[inputIdx].portVal_u8); /*this takes around 100us - do only one conversion every 100ms to minimize CPU load*/
  }
  else
  {
    /*no analog input was found was found, restart over - this is not necessary, as config is static, but the function will be called anayway, so it won't hurt to try finding one the next time.*/
    inputIdx = 0;
    overflowFlg = 0;       
  }   
}



/**
 * @brief Returns status of logical Input Pin.
 *
 * @param pinId_en logical input pin name. range: EN_INPUT_PINS
 * @return @c state of the pin.
 */
uint16_t GetLogicalInput (EN_INPUT_PINS pinId_en)
{	
  if(pinId_en>=EN_NUMBER_OF_ELEMENTS_INPUT)
  {
     return 0;
  }
  else
  {
      return inputBuffer_u16[pinId_en];	
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
