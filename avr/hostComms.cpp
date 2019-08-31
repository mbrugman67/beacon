#include "project.h"

HOST_COMMS::HOST_COMMS()
{
    resp.versionMajor = VERSION_MAJOR;
    resp.versionMinor = VERSION_MINOR;
    cfg.init();

    resp.bootCount = cfg.config.bootCount;
    resp.sequence = 0;

    last = desk::DL_CLEAR;
}

void HOST_COMMS::update(bool rwNow)
{
    usb.update();
    static bool once = false;
    
    if (usb.isEnumerated())
    {
        // check for new messages every coupla milliseconds
        if ((!(milliseconds % 2)) && !once)
        {
            once = true;

            if (usb.read((char*)&cfg.config.cmd, sizeof(host_command_t)))
            {
                desk::desk_light_t c = (desk::desk_light_t)cfg.config.cmd.ptrn;

                if (last != c)
                {
                    last = c;                
                    
                    robsDesk.setDeskPattern(c, cfg.config.cmd.posn, 
                                        cfg.config.cmd.red, cfg.config.cmd.grn, cfg.config.cmd.blu, 
                                        cfg.config.cmd.len);
                }

                ++resp.sequence;
                usb.write((char*)&resp, sizeof(response_to_host_t));
            }
        }
        else
        {
            once = false;
        }
    }
}
