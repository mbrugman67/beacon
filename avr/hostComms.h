
#ifndef HOST_COMMS_H_
#define HOST_COMMS_H_

class HOST_COMMS
{
public:
    HOST_COMMS();
    ~HOST_COMMS()   {}

    void update(bool rwNow = false);

    bool bootLoaderRequested()              { return (usb.isBootloaderRequested());}
    const host_command_t* getConfig()       { return (&cfg.config.cmd);}

private:
    response_to_host_t resp;
    desk::desk_light_t last;

    CFG cfg;
    usbHID usb;
};

#endif // HOST_COMMS_H_
