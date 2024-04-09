#include "config.h"

config::Arduino config::arduino;
config::Misc config::misc;
config::Triggerbot config::triggerbot;
config::Aimbot config::aimbot;

std::string config::config(std::string section, std::string param)
{
    std::ifstream cFile("config.ini");
    if (cFile.is_open())
    {
        std::string line;
        std::string currentSection;
        while (getline(cFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if (line.empty() || line[0] == ';' || line[0] == '#')
                continue;

            if (line[0] == '[')
            {
                currentSection = line.substr(1, line.find(']') - 1);
                continue;
            }

            auto commentPos = line.find('#');
            if (commentPos != std::string::npos)
            {
                line.erase(commentPos);
            }

            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            if (currentSection == section && name == param)
            {
                return value;
            }
        }
    }
    else
    {
        utils::printc("[-]", "Config file not found", RED);
        createConfigFile();
        utils::printc("[!]", "Please restart cheat.", YELLOW);

        system("pause");
        exit(0);
    }
    utils::printc("[-]", ("Config file found, but variable " + section + "." + param + " not found").c_str(), RED);
    utils::printc("[!]", "Please delete config file and restart cheat.", YELLOW);
    system("pause");
    exit(0);
}

void config::readConfigFile()
{
    config::arduino.enable = utils::stob(config("Arduino", "enable"));
    config::arduino.name = config("Arduino", "name");

    config::misc.only_enemies = utils::stob(config("Settings", "only-enemies"));
    config::misc.head_position = std::stoi(config("Settings", "head-position"));
    config::misc.tg_with_ab = utils::stob(config("Settings", "tb-with-ab"));

    config::triggerbot.enable = utils::stob(config("Triggerbot", "enable"));
    config::triggerbot.key = std::stoi(config("Triggerbot", "key"));
    config::triggerbot.delay_a = std::stoi(config("Triggerbot", "delay-a"));
    config::triggerbot.delay_b = std::stoi(config("Triggerbot", "delay-b"));

    config::aimbot.enable = utils::stob(config("Aimbot", "enable"));
    config::aimbot.key = std::stoi(config("Aimbot", "key"));
    config::aimbot.fov = std::stoi(config("Aimbot", "fov"));
    config::aimbot.speed = std::stoi(config("Aimbot", "speed"));
    config::aimbot.smooth = std::stoi(config("Aimbot", "smooth"));

    utils::printc("[+]", "Config file found", GREEN);
}

void config::createConfigFile()
{
    std::ofstream configFile("config.ini");
    if (configFile.is_open())
    {
        configFile << "# Standard config file has optimal settings for legit gameplay\n";
        configFile << "# If you want to use cheat without arduino change enable to false in Arduino section\n";
        configFile << "# Standart config has small fov and only-enemy=true so you need to aim near to the head to see that it works\n\n";
        configFile << "[Arduino]\n";
        configFile << "enable=true       # if you dont have arduino change to false\n";
        configFile << "name=Leonardo     # change this to your arduino name in Device Manager\n\n";
        configFile << "[Settings]\n";
        configFile << "head-position=65     # UPD: i forget to remove it, for 2.5 update its useless, so just dont touch it\n";
        configFile << "only-enemies=true    # true = aiming only on enemies false = on teammates too\n";
        configFile << "tb-with-ab=false     # true = when you holding triggerbot key it will also triggers aimbot\n\n";
        configFile << "[Triggerbot]\n";
        configFile << "enable=true\n";
        configFile << "key=164\n";
        configFile << "delay-b=0     # delay before click\n";
        configFile << "delay-a=200   # delay after click\n\n";
        configFile << "[Aimbot]\n";
        configFile << "enable=true\n";
        configFile << "key=1\n";
        configFile << "fov=20\n";
        configFile << "speed=2\n";
        configFile << "smooth=5\n";
        configFile.close();
        utils::printc("[+]", "Config file created successfully.", GREEN);
    }
    else {
        utils::printc("[-]", "Unable to create config file.", RED);
        system("pause");
        exit(0);
    }
}
