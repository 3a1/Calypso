#pragma once
#include "includes.h"
#include "settings.h"
#include "utils.h"

class Config
{
private:
    static void createConfigFile();
    static std::string read(std::string section, std::string param);

public:
    static void init();
};