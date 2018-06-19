#include "state.h"

#include <Arduino.h>

state* currentState = NULL;

void setState(state *newState) {
  if(newState == currentState) return;
  
  if(currentState!=NULL && currentState->end!=NULL)
    currentState->end();

  currentState = newState;

  if(currentState!=NULL && currentState->init!=NULL)
    currentState->init();
}

void spinCurrentState() {
  if(currentState!=NULL && currentState->spin!=NULL)
    currentState->spin();
}
