#ifndef PROBE_STATE__H_
#define PROBE_STATE__H_

typedef struct state {
  void (*init)();
  void (*spin)();
  void (*end)();
} state;

void setState(state *newState);

void spinCurrentState();

#endif //PROBE_STATE__H_
