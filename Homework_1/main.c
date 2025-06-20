/* Button → LED toggle + tone */

#include "ledtest.h"
#include "buttontest.h"
#include "buzzertest.h"
#include <stdio.h>



int ledFlag[LED_CNT] = {0};

int main(void)
{
    BUTTON_MSG_T msg;

    ledLibInit();
    buttonInit();
    buzzerInit();

    int qid = msgget(MESSAGE_ID, IPC_CREAT | 0666);

    while (1)
    {
        if (msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 0, 0) < 0)
            continue;
        if (msg.type != EV_KEY)
            continue;

        if (msg.pressed)                       /* key down */
        {
            int ledNo = -1, hz = 0;
            const char *note = "";

            switch (msg.keyInput) {
                case KEY_HOME:       ledNo = 0; hz = DO;  note = "do"; break;
                case KEY_BACK:       ledNo = 1; hz = RE;  note = "re"; break;
                case KEY_SEARCH:     ledNo = 2; hz = MI;  note = "mi"; break;
                case KEY_MENU:       ledNo = 3; hz = FA;  note = "fa"; break;
                case KEY_VOLUMEUP:   ledNo = 4; hz = SOL; note = "sol"; break;
                case KEY_VOLUMEDOWN: ledNo = 5; hz = RA;  note = "la"; break;
            }
            if (ledNo >= 0) {
                ledFlag[ledNo] ^= 1;
                ledOnOff(ledNo, ledFlag[ledNo]);
                buzzerTone(hz);
                printf("Btn %d → LED %s, buzzer : %s\n",
                       ledNo,
                       ledFlag[ledNo] ? "ON" : "OFF",
                       note);
            }
        }
        else                                    /* key up  */
        {
            buzzerStop();
        }
    }

    buttonExit();
    ledLibExit();
    buzzerExit();
    return 0;
}

