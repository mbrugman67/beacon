#ifndef DESK_H_
#define DESK_H_

#define DESK_LENGTH     260

class desk
{
public:
    enum desk_light_t : uint16_t
    {
        DL_CLEAR = 1,
        DL_FLASH_ONCE = 10,
        DL_POPO_ONCE,
        DL_POPO_CYCLE,
        DL_TRACER_ONCE,
        DL_TRACER_CYCLE,
        DL_RACER,
        DL_ALL_ONE
    };

    desk();
    ~desk();

    bool setDeskPattern(desk_light_t ptrn = DL_CLEAR, uint16_t where = 0, uint16_t red = 0,
                uint16_t grn = 0, uint16_t blu = 0, uint16_t count = 0);

    void update();

protected:
private:	
	bool popo(bool onceOnly = false);
    bool tracer(bool onceOnly = false);
	void racer();
	inline void allOneColor();
    inline void clearAll();

    WS2812 deskLEDs;
    cRGB commonPixel;
    desk_light_t currentState;
    desk_light_t lastState;

    size_t ledPosn;
    size_t ledPosn2;
    size_t len;
    size_t tics;
};

#endif // DESK_H_