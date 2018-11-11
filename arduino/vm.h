typedef struct {
  byte mac[6];
  char uid[40];
  int pin;
  int activityPin;
  bool running;
  bool starting;
  unsigned long startedAt;
  bool selected;
  unsigned long selectedAt;
  bool pressed;
  unsigned long lastPressed;
} VM;

