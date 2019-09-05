#include "project.h"

beacon::beacon() : beaconLEDs(BEACON_LENGTH)
{
    currentState = BL_START;
    lastState = BL_START;
    startupDone = false;
    tics = 0;

    beaconLEDs.setOutput(&PORTB, &DDRB, PINB1);
    beaconLEDs.setColorOrderRGB();
}

beacon::~beacon()
{

}

void beacon::update()
{
    switch (currentState)
    {
        case BL_CLEAR: 
        {
            this->clearAll();
        }  break;

        case BL_START:
        {
            this->startPattern();
        }  break;

        case BL_SWING: 
        {

        }  break;

        case BL_FADE: 
        {
        }  break;

        case BL_BREATHE: 
        {
            this->breathe();
        }  break;

        case BL_PULSE: 
        {
            this->pulse();
        }  break;

        case BL_LAST: 
        {
            this->clearAll();
        }  break;
    }

    cli();
    beaconLEDs.sync();
    sei();

    ++tics;
}

void beacon::startPattern()
{
    static size_t sstate = 0;

    if (!(tics % 2))
    {
        switch(sstate)
        {
            case 0:
            {
                ++commonPixel.r = 0;
                ++commonPixel.g = 0;
                ++commonPixel.b = 0;

                sstate = 1;
            }  break;

            case 1:
            {
                ++commonPixel.r;
                ++commonPixel.g;
                ++commonPixel.b;

                if (commonPixel.r == 255)
                {
                    sstate = 2;
                }
            }  break;

            case 2:
            {
                --commonPixel.r;
                --commonPixel.g;
                --commonPixel.b;

                if (!commonPixel.r)
                {
                    sstate = 3;
                }
            }  break;

            case 3:
            {
                ++commonPixel.r;
                commonPixel.g = 0;
                commonPixel.b = 0;

                if (commonPixel.r == 255)
                {
                    sstate = 4;
                }
            }  break;

            case 4:
            {
                --commonPixel.r;
                ++commonPixel.g;
                commonPixel.b = 0;

                if (commonPixel.g == 255)
                {
                    sstate = 5;
                }
            }  break;

            case 5:
            {
                --commonPixel.g;
                ++commonPixel.b;
                commonPixel.r = 0;

                if (commonPixel.b == 255)
                {
                    sstate = 6;
                }
            }  break;

            case 6:
            {
                --commonPixel.b;
                commonPixel.g = 0;
                commonPixel.r = 0;

                if (!commonPixel.b)
                {
                    sstate = 7;
                    startupDone = true;
                }
            }  break;

            default:
            {
                commonPixel.b = 0;
                commonPixel.g = 0;
                commonPixel.r = 0;
            }
        }
    }

    for (size_t ii = 0; ii < BEACON_LENGTH; ++ii)
    {
        beaconLEDs.set_crgb_at(ii, commonPixel);
    }
}

void beacon::swing()
{

}

void beacon::fade()
{

}

void beacon::breathe()
{
    static bool dir = true;

    

    if (!(tics % 4))
    {
        if (dir)
        {
            if (commonPixel.r)  ++thisPixel.r;
            if (commonPixel.g)  ++thisPixel.g;
            if (commonPixel.b)  ++thisPixel.b;


            if (thisPixel.r > 253 || thisPixel.g > 253 || thisPixel.b > 253)
            {
                dir = false;
            }
        }
        else
        {
            if (commonPixel.r)  --thisPixel.r;
            if (commonPixel.g)  --thisPixel.g;
            if (commonPixel.b)  --thisPixel.b;

            if (thisPixel.r < 1 || thisPixel.g < 1 || thisPixel.b < 1)
            {
                dir = true;
            }
        }
    }

    for (size_t ii = 0; ii < BEACON_LENGTH; ++ii)
    {
        beaconLEDs.set_crgb_at(ii, thisPixel);
    }
}

void beacon::pulse()
{
    if (!(tics % 2))
    {
        commonPixel.b = commonPixel.g = 0;
        --commonPixel.r;
    }

    for (size_t ii = 0; ii < BEACON_LENGTH; ++ii)
    {
        beaconLEDs.set_crgb_at(ii, commonPixel);
    }
}

/*
bool desk::popo(bool onceOnly)
{
    bool retVal = false;

    if (tics > 3)
    {
        tics = 0;

        this->clearAll();

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 10;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 30;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 60;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 0;
        commonPixel.b = 255;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.b = 0;
        commonPixel.r = 60;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 30;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        ++ledPosn; ledPosn %= BEACON_LENGTH;
        commonPixel.r = 10;
        beaconLEDs.set_crgb_at(ledPosn, commonPixel);

        if (onceOnly && ledPosn > (BEACON_LENGTH))
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
        beaconLEDs.set_crgb_at((ii + ledPosn) % BEACON_LENGTH, commonPixel);
    }

    ++ledPosn; ledPosn %= BEACON_LENGTH;

    if (onceOnly && ledPosn > (BEACON_LENGTH - len - 1))
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
        ++ledPosn; ledPosn %= BEACON_LENGTH;
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
        beaconLEDs.set_crgb_at((ii + ledPosn) % BEACON_LENGTH, commonPixel);
    }

    commonPixel.r = 40;
    commonPixel.g = 0;
    commonPixel.b = 180;

    for (size_t ii = 0; ii < 2; ii++)
    {
        beaconLEDs.set_crgb_at((ii + ledPosn2) % BEACON_LENGTH, commonPixel);
    }
    ++ledPosn2; ledPosn2 %= BEACON_LENGTH;
}
*/

void beacon::allOneColor()
{
    if (tics < 2)
    {
        for (size_t ii = 0; ii < BEACON_LENGTH; ii++)
        {
            beaconLEDs.set_crgb_at(ii, commonPixel);
        }
    }
}

void beacon::clearAll()
{
    cRGB clear;
    clear.r = clear.g = clear.b = 0;

    for (size_t ii = 0; ii < BEACON_LENGTH; ii++)
    {
        beaconLEDs.set_crgb_at(ii, clear);
    }
}

bool beacon::setBeaconPattern(beacon_light_t ptrn, uint16_t where, uint16_t red, uint16_t grn, uint16_t blu)
{
    // actual pattern change?
    if (ptrn != lastState && startupDone)
    {
        this->clearAll();
        lastState = ptrn;
        currentState = ptrn;

        commonPixel.r = (uint8_t)red;
        commonPixel.g = (uint8_t)grn;
        commonPixel.b = (uint8_t)blu;
        thisPixel = commonPixel;

        tics = 0;

        return (true);
    }

    return (false);
}
