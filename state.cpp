#include "state.h"

#include <Arduino.h>

state* currentState = NULL;

/**************************************************************************/
/*!
    @brief  Transition from the current state to a new state. It is not safe to call this method during a states end method. Doing so will result in undefined behaviour
*/
/**************************************************************************/
void setState(state *newState) {
  if(newState == currentState) return;
  
  if(currentState!=NULL && currentState->end!=NULL)
    currentState->end();

  currentState = newState;

  if(currentState!=NULL && currentState->init!=NULL)
    currentState->init();
}

/**************************************************************************/
/*!
    @brief  Call the current states spin method (if it has one)
*/
/**************************************************************************/
void spinCurrentState() {
  if(currentState!=NULL && currentState->spin!=NULL)
    currentState->spin();
}
