#include <stdio.h>

typedef struct {
   void (*start)(int gpio);
   void (*stop)(int gpio);
   void (*changeSpeed)(int gpio, int speed);
} MotorController;

typedef int PIN;

// Các hàm chung
void startMotor(PIN pin) {
   printf("Start motor at PIN %d\n", pin);
}

void stopMotor(PIN pin) {
   printf("Stop motor at PIN %d\n", pin);
}

void changeSpeedMotor(PIN pin, int speed) {
   printf("Change speed at PIN %d: %d\n", pin, speed);
}

// Macro de khoi tao GPIO và MotorController
#define INIT_MOTOR(motorName, pinNumber) \
   PIN PIN_##motorName = pinNumber; \
   MotorController motorName = {startMotor, stopMotor, changeSpeedMotor};

int main() {
   // Su dung macro de khoi tao
   INIT_MOTOR(motorA, 1);
   INIT_MOTOR(motorB, 2);

   // Su dung motor A
   motorA.start(g_motorA);
   motorA.changeSpeed(g_motorA, 50);
   motorA.stop(g_motorA);

   // Su dung motor B
   motorB.start(g_motorB);
   motorB.changeSpeed(g_motorB, 75);
   motorB.stop(g_motorB);

   return 0;
}








