#include "project.h"

desk::desk() : deskLEDs(DESK_LENGTH)
{
    currentState = DL_CLEAR;
    lastState = DL_CLEAR;
    ledPosn = 0;
    ledPosn2 = 0;
    len = 0;
    tics = 0;

    deskLEDs.setOutput(&PORTF, &DDRF, 0);
    deskLEDs.setColorOrderGRB();
}

desk::~desk()
{

}

void desk::update()
{
    switch (currentState)
    {
        case DL_CLEAR:
        {
            this->clearAll();
        } break;

        case DL_FLASH_ONCE:
        {
            deskLEDs.set_crgb_at(ledPosn, commonPixel);
            if (tics > 10)
            {
                currentState = DL_CLEAR;
            }
        } break;

        case DL_POPO_ONCE:
        {
            if (this->popo(true))
            {
                currentState = DL_CLEAR;
            }
        } break;

        case DL_POPO_CYCLE:
        {
            this->popo();
        } break;

        case DL_TRACER_ONCE:
        {
            if (this->tracer(true))
            {
                currentState = DL_CLEAR;
            }
        } break;

        case DL_TRACER_CYCLE:
        {
            this->tracer();
        } break;

        case DL_RACER:
        {
            this->racer();
        } break;

        case DL_ALL_ONE:
        {
            this->allOneColor();
        } break;
    }

    cli();
    deskLEDs.sync();
    sei();

    ++tics;
}

bool desk::popo(bool onceOnly)
{
    bool retVal = false;

    if (tics > 3)
    {
        tics = 0;

        this->clearAll();

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 10;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 30;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 60;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 0;
        commonPixel.b = 255;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.b = 0;
        commonPixel.r = 60;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 30;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= DESK_LENGTH;
        commonPixel.r = 10;
        deskLEDs.set_crgb_at(ledPosn, commonPixel);

        if (onceOnly && ledPosn > (DESK_LENGTH - 8))
        {
            retVal = true;
        }
    }

    return (retVal);
}


bool desk::tracer(bool onceOnly)
{
    this->clearAll();

    for (size_t ii = 0; ii < len; ii++)
    {
        deskLEDs.set_crgb_at((ii + ledPosn) % DESK_LENGTH, commonPixel);
    }

    ++ledPosn; ledPosn %= DESK_LENGTH;

    if (onceOnly && ledPosn > (DESK_LENGTH - len - 1))
    {
        return (true);
    }

    return (false);
}

void desk::racer()
{
    this->clearAll();

    // base "slow" racer
    if (tics < 2)
    {
        ++ledPosn; ledPosn %= DESK_LENGTH;
    }
    else if (tics == 3)
    {
        tics = 0;
    }

    commonPixel.r = 120;
    commonPixel.g = 40;
    commonPixel.b = 0;

    for (size_t ii = 0; ii < 6; ii++)
    {
        deskLEDs.set_crgb_at((ii + ledPosn) % DESK_LENGTH, commonPixel);
    }

    commonPixel.r = 40;
    commonPixel.g = 0;
    commonPixel.b = 180;

    for (size_t ii = 0; ii < 2; ii++)
    {
        deskLEDs.set_crgb_at((ii + ledPosn2) % DESK_LENGTH, commonPixel);
    }
    ++ledPosn2; ledPosn2 %= DESK_LENGTH;
}

void desk::allOneColor()
{
    if (tics < 2)
    {
        for (size_t ii = 0; ii < DESK_LENGTH; ii++)
        {
            deskLEDs.set_crgb_at(ii, commonPixel);
        }
    }
}

void desk::clearAll()
{
    cRGB clear;
    clear.r = clear.g = clear.b = 0;

    for (size_t ii = 0; ii < DESK_LENGTH; ii++)
    {
        deskLEDs.set_crgb_at(ii, clear);
    }
}

bool desk::setDeskPattern(desk_light_t ptrn, uint16_t where, uint16_t red, uint16_t grn, uint16_t blu, uint16_t count)
{
    // actual pattern change?
    if (ptrn != lastState)
    {
        this->clearAll();
        lastState = ptrn;
        currentState = ptrn;

        commonPixel.r = (uint8_t)red;
        commonPixel.g = (uint8_t)grn;
        commonPixel.b = (uint8_t)blu;

        ledPosn = where;
        ledPosn2 = where;
        len = count;
        tics = 0;

        return (true);
    }

    return (false);
}
