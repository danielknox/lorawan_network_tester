#ifndef PROBE_JOYSTICK__H_
#define PROBE_JOYSTICK__H_

#include <AxisJoystick.h>

#define JOY_UP      (AxisJoystick::Move::UP)
#define JOY_DOWN    (AxisJoystick::Move::DOWN)
#define JOY_LEFT    (AxisJoystick::Move::RIGHT)
#define JOY_RIGHT   (AxisJoystick::Move::LEFT)
#define JOY_PRESSED (AxisJoystick::Move::PRESS)

typedef AxisJoystick::Move joyState;

void initJoystick();

joyState readJoystick();

#endif //PROBE_JOYSTICK__H_
