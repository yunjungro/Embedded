#include "../../ledtest.h"
#include "../../buttontest.h"
#include "../../buzzertest.h"

#include "../fndtest.h"
#include "../textlcdtest.h"
#include "../colorLEDtest.h"

#include "temperatureTest.h"
#include "accelManGyroTest.h"
#include <stdio.h>

int ledFlag[LED_CNT] = {0};

void init() {
    ledLibInit();
    buttonInit();
    buzzerInit();
    textLCDInit();
    colorLED_init();
    colorLED_pwmEnable();
}

void closeAll() {
    buttonExit();
    ledLibExit();
    buzzerExit();
    fndExit();
    textLCDExit();
}

int main(void) {
    BUTTON_MSG_T msg;
    int gyro[3], accel[3], magnet[3];
    int mode = 0, prevMode = -1;

    init();
    int qid = msgget(MESSAGE_ID, IPC_CREAT | 0666);

    while (1) {
        if (msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 0, 0) < 0)
            continue;

        if (msg.type != EV_KEY)
            continue;

        if (msg.pressed) {
            int ledNo = -1, hz = 0;
            const char *note = "";

            switch (msg.keyInput) {
                case KEY_HOME: mode = 0; ledNo = 0; hz = DO; note = "do"; break;
                case KEY_BACK: mode = 1; ledNo = 1; hz = RE; note = "re"; break;
                case KEY_SEARCH: mode = 2; ledNo = 2; hz = MI; note = "mi"; break;
                case KEY_MENU: mode = 3; ledNo = 3; hz = FA; note = "fa"; break;
                case KEY_VOLUMEUP: mode = 0; ledNo = 4; hz = SOL; note = "sol"; break;
                case KEY_VOLUMEDOWN: mode = 1; ledNo = 5; hz = RA; note = "la"; break;
            }

            if (ledNo >= 0) {
                ledFlag[ledNo] ^= 1;
                ledOnOff(ledNo, ledFlag[ledNo]);
                buzzerTone(hz);
                printf("Btn %d → LED %s, buzzer : %s\n", ledNo, ledFlag[ledNo] ? "ON" : "OFF", note);
            }
        } else {
            buzzerStop();
        }

        // Sensor output when mode changes
        if (mode != prevMode) {
            prevMode = mode;

            if (mode == 0) { getGyro(gyro); printf("[Gyro] : %d, %d, %d\n", gyro[0], gyro[1], gyro[2]); }
            else if (mode == 1) { getAccel(accel); printf("[Accel] : %d, %d, %d\n", accel[0], accel[1], accel[2]); }
            else if (mode == 2) { getMagnet(magnet); printf("[Magnet] : %d, %d, %d\n", magnet[0], magnet[1], magnet[2]); }
            else if (mode == 3) { temp_read(); }

            printf("\n");
        }

        sleep(1);  // Avoid frequent updates
    }

    closeAll();
    return 0;
}

