#include "project.h"

// blue
void beacon::available()
{
    static uint8_t currentBlue = 255;

    if (newPattern)
    {
        currentBlue = 255;
    }

    // middle ball is blue
    commonPixel.r = 0;
    commonPixel.g = 0;
    commonPixel.b = 255;
    beaconLEDs.set_crgb_at(ID_MID, commonPixel);

    if (!(tics % 4))
    {
        commonPixel.b = currentBlue;
        
        for (size_t ii = ID_NORTH; ii < ID_LAST; ++ii)
        {
            beaconLEDs.set_crgb_at(ii, commonPixel);
            commonPixel.b -= 64;
        }

        --currentBlue;
    }
}

void beacon::busy()
{
    static bool up;
    static size_t inx = ID_NORTH;
    static size_t inx2 = ID_WEST;
    static cRGB north;
    static cRGB east;

    if (newPattern)
    {
        up = true;
        north.r = 0;
        north.b = 0;
        north.g = 0;
        east.r = 255;
        east.b = 0;
        east.g = 0;

        thisPixel.r = 0;
        thisPixel.b = 0;
        thisPixel.g = 0;
        
        // middle ball is red
        commonPixel.r = 255;
        commonPixel.g = 0;
        commonPixel.b = 0;
    }

    beaconLEDs.set_crgb_at(ID_MID, commonPixel);

    //if (!(tics % 2))
    {
        if (up)
        {
            ++north.r; 
            --east.r;

            if (north.r > 254)
            {
                up = false;

                if (inx2 == ID_WEST)        inx2 = ID_EAST;
                else if (inx2 == ID_EAST)   inx2 = ID_WEST;
            }
        }
        else
        {
            --north.r; 
            ++east.r;

            if (north.r < 1)
            {
                up = true;

                if (inx == ID_NORTH)        inx = ID_SOUTH;
                else if (inx == ID_SOUTH)    inx = ID_NORTH;
            }
        }

        for (size_t ii = ID_NORTH; ii < ID_LAST; ++ii)
        {
            beaconLEDs.set_crgb_at(ii, thisPixel);
        }
        
        beaconLEDs.set_crgb_at(inx, north);
        beaconLEDs.set_crgb_at(inx2, east);

    }
}

void beacon::dnd()
{
    static uint8_t reds[] = {0, 50, 100, 150};

    if (newPattern)
    {
        commonPixel.g = 0;
        commonPixel.r = 200;
        commonPixel.b = 0;

        thisPixel.b = 0;
        thisPixel.g = 0;
    }

    if (!(tics % 4))
    {
        --commonPixel.r;
        beaconLEDs.set_crgb_at(ID_MID, commonPixel);

        for (size_t ii = 0; ii < 4; ++ii)
        {
            thisPixel.r = reds[1];
            beaconLEDs.set_crgb_at(ii + ID_NORTH, thisPixel);
        }

        --(reds[1]);
    }
}

void beacon::away()
{
    static bool dir = true;
    static uint16_t currentRed = 0;
    static uint16_t currentGreen = 0;

    if (newPattern)
    {
        currentRed = 0;
        currentGreen = 0;
        dir = true;
    }

    commonPixel.r = 255;
    commonPixel.g = 255;
    commonPixel.b = 0;
    beaconLEDs.set_crgb_at(ID_MID, commonPixel);

    if (!(tics % 8))
    {
        if (dir)
        {
            ++currentRed;
            ++currentGreen;

            if (currentRed > 254)
            {
                dir = false;
            }
        }
        else
        {
            --currentRed;
            --currentGreen;

            if (currentRed < 1)
            {
                dir = true;
            }
        }

        commonPixel.g = currentGreen;
        commonPixel.r = currentRed;

        for (size_t ii = ID_NORTH; ii < ID_LAST; ++ii)
        {
            beaconLEDs.set_crgb_at(ii, commonPixel);
        }
    }
}

void beacon::rainbow()
{
    static cRGB balls[4];
    static bool dir;
    static unsigned char inx = 0;
    static unsigned char count = 0;

    if (newPattern)
    {
        balls[0].r = 0;
        balls[0].g = 0;
        balls[0].b = 255;

        balls[1].r = 0;
        balls[1].g = 0;
        balls[1].b = 255;

        balls[2].r = 255;
        balls[2].g = 0;
        balls[2].b = 0;

        balls[3].r = 0;
        balls[3].g = 255;
        balls[3].b = 0;
    
        commonPixel.r = 0;
        commonPixel.g = 0;
        commonPixel.b = 255;

        dir = true;
        inx = 0;
        count = 0;
    }

    beaconLEDs.set_crgb_at(ID_MID, commonPixel);

    if (!(tics % 2))
    {
        ++count;
        if (dir)
        {
            --balls[1].b;
            ++balls[1].r;

            --balls[2].r;
            ++balls[2].g;

            --balls[3].g;
            ++balls[3].b;

            if (!count)
            {
                ++inx;
            }
        }
        else
        {
            ++balls[1].b;
            --balls[1].r;

            ++balls[2].r;
            --balls[2].g;

            ++balls[3].g;
            --balls[3].b;

            if (!balls[1].r)
            {
                dir = true;
                ++inx;
                balls[1].r = balls[2].g = balls[3].b = 0;
                balls[1].b = balls[2].r = balls[3].g = 255;
            }        
        }
    }
        
    beaconLEDs.set_crgb_at(ID_NORTH, balls[(inx + 3) % 4]);
    beaconLEDs.set_crgb_at(ID_EAST,  balls[(inx + 2) % 4]);
    beaconLEDs.set_crgb_at(ID_SOUTH, balls[(inx + 1) % 4]);
    beaconLEDs.set_crgb_at(ID_WEST,  balls[(inx + 0) % 4]);
}