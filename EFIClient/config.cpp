#include "config.h"
#include "includes.h"

#include <iostream>
#include <fstream>
#include <algorithm>

void createConfigFile() {
    std::ofstream configFile("config.cfg");
    if (configFile.is_open()) {
        configFile << "# 1 = arduino 2 = default mouse movement\n";
        configFile << "movement_type=2\n\n";
        configFile << "# Triggerbot\n";
        configFile << "triggerbot=true\n";
        configFile << "triggerbot_delay_before_click=0\n";
        configFile << "triggerbot_delay_after_click=200\n\n";
        configFile << "# Aimbot\n";
        configFile << "aimbot=true\n";
        configFile << "aimbot_fov=20\n";
        configFile << "aimbot_speed=2\n";
        configFile << "aimbot_smooth_amount=10\n";
        configFile.close();
        std::cout << "[Z3BRA] Config file created successfully.\n";
    }
    else {
        std::cerr << "Unable to create config file.\n";
    }
}

std::string config(std::string param)
{
    // std::ifstream is RAII, i.e. no need to call close
    std::ifstream cFile("config.cfg");
    if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                line.end());
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            if (name == param) {
                return value;
            }
        }

    }
    else {
        std::cout << "[Z3BRA] Didnt find config file, creating one...\n";

        createConfigFile();
        
        std::cout << "[Z3BRA] Please restart cheat.\n";

        system("pause");
        exit(0);
    }
}