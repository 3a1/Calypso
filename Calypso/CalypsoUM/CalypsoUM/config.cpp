#include "config.h"

std::string Config::read(std::string section, std::string param)
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

            auto delimiterPos = line.find(":");
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
        Log("[-]", "Config file not found", RED);
        createConfigFile();
        Log("[!]", "Please restart cheat.", YELLOW);

        system("pause");
        exit(0);
    }
    Log("[-]", ("Config file found, but variable " + section + "." + param + " not found").c_str(), RED);
    Log("[!]", "Please delete config file and restart cheat.", YELLOW);
    system("pause");
    exit(0);
}

void Config::init()
{
    settings::arduino::enable = utils::text::stob(read("Arduino", "enable"));
    settings::arduino::name = utils::text::utos(read("Arduino", "name"));

    settings::misc::only_enemies = utils::text::stob(read("Settings", "only-enemies"));
    settings::misc::tg_with_ab = utils::text::stob(read("Settings", "tb-with-ab"));
    settings::misc::bone = utils::text::stobn(read("Settings", "bone-position"));

    settings::triggerbot::enable = utils::text::stob(read("Triggerbot", "enable"));
    settings::triggerbot::key = utils::text::stovk(read("Triggerbot", "key"));
    settings::triggerbot::delay_a = std::stoi(read("Triggerbot", "delay-a"));
    settings::triggerbot::delay_b = std::stoi(read("Triggerbot", "delay-b"));

    settings::aimbot::enable = utils::text::stob(read("Aimbot", "enable"));
    settings::aimbot::key = utils::text::stovk(read("Aimbot", "key"));
    settings::aimbot::fov = std::stoi(read("Aimbot", "fov"));
    settings::aimbot::smooth = std::stof(read("Aimbot", "smooth"));

    Log("[+]", "Config file found", GREEN);
}

void Config::createConfigFile()
{
    std::ofstream configFile("config.ini");
    if (configFile.is_open())
    {
        configFile << "# Windows virtual keys list: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes\n\n";
        configFile << "[Arduino]\n";
        configFile << "enable: false              # if you want to use arduino change to true\n";
        configFile << "name: Arduino_Leonardo     # arduino name in Device Manager, spaces = underscores\n\n";
        configFile << "[Settings]\n";
        configFile << "only-enemies: true    # false = aiming on teammates too\n";
        configFile << "bone-position: HEAD   # HEAD, NECK, PELVIS, HAND_R, LEG_UPPER_L etc.\n";
        configFile << "tb-with-ab: false     # true = when you holding triggerbot key it will also triggers aimbot\n\n";
        configFile << "[Triggerbot]\n";
        configFile << "enable: true\n";
        configFile << "key: VK_LMENU\n";
        configFile << "delay-b: 0     # delay before shot\n";
        configFile << "delay-a: 200   # delay after shot\n\n";
        configFile << "[Aimbot]\n";
        configFile << "enable: true\n";
        configFile << "key: VK_LBUTTON\n";
        configFile << "fov: 100\n";
        configFile << "smooth: 2.5";
        configFile.close();
        Log("[+]", "Config file created successfully.", GREEN);
    }
    else 
    {
        Log("[-]", "Unable to create config file.", RED);
        system("pause");
        exit(0);
    }
}
