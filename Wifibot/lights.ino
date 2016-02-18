
/**
 * @brief flasher states
 *
 *  Holds the currently running flasher state
 */
EN_FLASHER_STATES flasherStates_en;



/**
* @brief Implementation of function that handles the initialization of light control
*
* mplementation of function that handles the initialization of light control
* @return void
* @note Void function with no return.
*/
void lightsInit()
{
   flasherStates_en = EN_STATE_FLA_OFF;
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
	
	
}


/**
* @brief Implementation of function that handles the light control
*
* Implementation of function that handles the light control
* @return void
* @note Void function with no return.
*/
void setFlasherActive(EN_FLASHER_STATES flaSta_en)
{	
  if (flaSta_en < EN_STATE_FLA_LAST_ELEMENT)
  {
    flasherStates_en = flaSta_en;
  }
  else
  {
    //wrong state reuqested, keep last state
  }
}
