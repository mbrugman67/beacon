#ifndef _EEPROM_H_
#define _EEPROM_H_

class CFG
{
public:
    CFG();
    ~CFG();

    void init(bool resetCounts = false);
    void resetCommand();
    void loadConfig();
    void saveConfig();

    config_t config;

private: 
    size_t configSize;

};

#endif //define _EEPROM_H_
