#ifndef BEACON_H
#define BEACON_H

#define BEACON_LENGTH     5

class beacon
{
public:
    enum beacon_light_t : uint16_t
    {
        BL_CLEAR = 0,
        BL_START,
        BL_SWING,
        BL_FADE,
        BL_BREATHE,
        BL_PULSE,
        BL_LAST
    };

    beacon();
    ~beacon();

    bool setBeaconPattern(beacon_light_t ptrn = BL_CLEAR, uint16_t where = 0, uint16_t red = 0,
                uint16_t grn = 0, uint16_t blu = 0);

    void update();

protected:
private:	
    void startPattern();
    void swing();
    void fade();
    void breathe();
    void pulse();
	inline void allOneColor();
    inline void clearAll();

    WS2812 beaconLEDs;
    cRGB commonPixel;
    cRGB thisPixel;
    beacon_light_t currentState;
    beacon_light_t lastState;

    size_t tics;
    bool startupDone;
};

#endif // DESK_H_