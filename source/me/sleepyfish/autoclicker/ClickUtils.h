// class by sleepyfish for abyss recode
// February 2024 Saturday 10:59 AM

#ifndef NEWABYSS_CLICKUTILS_H
#define NEWABYSS_CLICKUTILS_H

#include "Utils.h"
#include "Windows.h"
#include "chrono"

class ClickUtils {

private:
    POINT pos_cursor;
    int keyCode;
    int keyStateDown;
    int keyStateFinalUp;
    LPARAM param;

    float nextCps = 1;
    long long lastClickTime = 0;

public:
    float randomValue;

    ClickUtils(int keyCode) :
        keyCode(keyCode),
        keyStateDown(0),
        keyStateFinalUp(0)
    {
        this->init();
    }

    void init() {
        GetCursorPos(&this->pos_cursor);
        param = MAKELPARAM(this->pos_cursor.x, this->pos_cursor.y);

        if (keyCode == 0) {
            keyStateDown = WM_LBUTTONDOWN;
            keyStateFinalUp = WM_LBUTTONUP;
            return;
        }

        if (keyCode == 1) {
            keyStateDown = WM_RBUTTONDOWN;
            keyStateFinalUp = WM_RBUTTONUP;
            return;
        }
    }

    void update(float min, float max, bool extraRandom = false, float extraMin = 6.0F, float extraMax = 8.0F) {
        long long milliLeft = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        float finalMin = min;
        float finalMax = max;

        if (extraRandom) {
            finalMin = Utils::randomFloat(min - extraMin, min + extraMax);
            finalMax = Utils::randomFloat(max - extraMin, max + extraMax);
        }

        if (this->lastClickTime == 0L)
            this->lastClickTime = milliLeft;

        if ((milliLeft - this->lastClickTime) < (1000L / (long long) this->nextCps))
            return;

        const int code = this->keyCode + 1;

        const HWND handleWindow = GetForegroundWindow();
        SendMessageA(handleWindow, keyStateDown, code, this->param);
        SendMessageA(handleWindow, keyStateFinalUp, code, this->param);

        this->lastClickTime = milliLeft;

        this->randomValue = Utils::randomFloat(finalMin, finalMax);
        this->nextCps = this->randomValue;
    }

};

#endif //NEWABYSS_CLICKUTILS_H