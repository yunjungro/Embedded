#ifndef _COLORLEDTEST_H_
#define _COLORLEDTEST_H_

#define COLOR_LED_DEV_R_	"/sys/class/pwm/pwmchip0/"
#define COLOR_LED_DEV_G_	"/sys/class/pwm/pwmchip1/"
#define COLOR_LED_DEV_B_	"/sys/class/pwm/pwmchip2/"

#define PWM_EXPORT          "export"
#define PWM_UNEXPORT        "unexport"
#define PWM_DUTY            "pwm0/duty_cycle"
#define PWM_PERIOD          "pwm0/period"   
#define PWM_ENABLE		    "pwm0/enable"

#define PWM_COLOR_R	0
#define PWM_COLOR_G	1
#define PWM_COLOR_B	2

#define  PWM_PERIOD_NS 1000000

int colorLED_pwmActiveAll();
int colorLED_pwmDisable();
int colorLED_setPeriod(int period, int index);
int colorLED_setDuty(int dutyCycle, int index);
int colorLED_serPercent(int percent, int ledColor);
int colorLED_pwmEnable();
void colorLED_init();
#endif
