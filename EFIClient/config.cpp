#include "config.h"
#include "includes.h"

#include <iostream>
#include <fstream>
#include <algorithm>

std::string arduino_name;

int movement_type;
bool only_enemies;
int head_position;

bool triggerbot;
int triggerbot_button;
int triggerbot_delay_b;
int triggerbot_delay_a;

bool aimbot;
int aimbot_button;
int aimbot_fov;
int aimbot_speed;
int aimbot_smooth_amount;

void ConfigRead() {

    std::string arduino_name = "Arduino " + config("arduino");

    movement_type = std::stoi(config("movement-type"));
    only_enemies = stringToBool(config("only-enemies"));
    head_position = std::stoi(config("head-position"));

    triggerbot = stringToBool(config("triggerbot"));
    triggerbot_button = std::stoi(config("triggerbot-key"));
    triggerbot_delay_b = std::stoi(config("triggerbot-delay-before-click"));
    triggerbot_delay_a = std::stoi(config("triggerbot-delay-after-click"));

    aimbot = stringToBool(config("aimbot"));
    aimbot_button = std::stoi(config("aimbot-key"));
    aimbot_fov = std::stoi(config("aimbot-fov"));
    aimbot_speed = std::stoi(config("aimbot-speed"));
    aimbot_smooth_amount = std::stoi(config("aimbot-smooth"));
}

void createConfigFile() {
    std::ofstream configFile("config.ini");
    if (configFile.is_open()) {
        configFile << "[Setup]\n";
        configFile << "movement-type=1      # 1 = arduino 2 = default mouse movement\n";
        configFile << "arduino=Leonardo     # change this to your arduino name in Device Manager\n\n";
        configFile << "[Settings]\n";
        configFile << "head-position=65     # change this if the aimbot is aiming below or above the head\n";
        configFile << "only-enemies=true    # true = aiming only on enemies false = on teammates too\n\n";
        configFile << "[Triggerbot]\n";
        configFile << "triggerbot=true\n";
        configFile << "triggerbot-key=164\n";
        configFile << "triggerbot-delay-before-click=0\n";
        configFile << "triggerbot-delay-after-click=200\n\n";
        configFile << "[Aimbot]\n";
        configFile << "aimbot=true\n";
        configFile << "aimbot-key=1\n";
        configFile << "aimbot-fov=20\n";
        configFile << "aimbot-speed=2\n";
        configFile << "aimbot-smooth=5\n";
        configFile.close();
        std::cout << "[Z3BRA] Config file created successfully.\n";
    }
    else {
        std::cerr << "Unable to create config file.\n";
    }
}

std::string config(std::string param)
{
    std::ifstream cFile("config.ini");
    if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                line.end());
            if (line[0] == '[' || line[0] == '#' || line.empty())
                continue;

            auto commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line.erase(commentPos);
            }

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