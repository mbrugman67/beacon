#include "project.h"

#define EEPROM_SIGNATURE_WORD   0x54fa

CFG::CFG()  {}
CFG::~CFG() {}

static host_command_t defaultConfig =
{
    0
};

void CFG::init(bool resetCounts)
{
    configSize = sizeof(config_t);
    this->loadConfig();
    
    // check to see if signature matches; if not, we'll have to set default values
    if(config.signature != EEPROM_SIGNATURE_WORD || resetCounts)
    {
        config.bootCount = 0;
        config.signature = EEPROM_SIGNATURE_WORD;
    }
    
    this->resetCommand();

    ++config.bootCount;
    this->saveConfig();
}

void CFG::resetCommand()
{
    memcpy((void*)&config.cmd, (void*)&defaultConfig, sizeof(host_command_t));
}

void CFG::loadConfig()
{
    eeprom_read_block((void*)&config, (void*)0x100, configSize);
}
void CFG::saveConfig()
{
    eeprom_update_block((void*)&config, (void*)0x100, configSize);
}
