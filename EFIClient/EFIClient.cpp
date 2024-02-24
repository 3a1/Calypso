

#include "includes.h"
#include "offsets.h"
#include "functions.h"
#include <stdint.h>
#include "arduino.h"

arduino duino("Arduino Leonardo");

void AimbotMain(int pid, uintptr_t client) {
 
    uintptr_t player = Driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
    int entityId = Driver::read<int>(pid, player + offsets::m_iIDEntIndex);

    if (entityId > 0) {
        uintptr_t entList = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

        uintptr_t entEntry = Driver::read<uintptr_t>(pid, entList + 0x8 * (entityId >> 9) + 0x10 );
        uintptr_t entity = Driver::read<uintptr_t>(pid, entEntry + 120 * (entityId & 0x1FF) );

        int entityTeam = Driver::read<int>(pid, entity + offsets::m_iTeamNum);
        int playerTeam = Driver::read<int>(pid, player + offsets::m_iTeamNum);

        if (entityTeam != playerTeam) {
            int entityHp = Driver::read<int>(pid, entity + offsets::m_iHealth);
            if (entityHp > 0) {
                char buffer[] = "-10";
                duino.send_data(buffer, sizeof(buffer));
                //std::cout << "enemy" << std::endl;
            }
        }



    }
}


int main()
{
    if (!Driver::initialize() || !CheckDriverStatus()) {
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);//delete var

        printf("No EFI Driver found\n");
        system("pause");
        exit(1);
        return 1;
    }
    printf("[Z] EFI Driver found\n");

    auto hwnd = FindWindowA(NULL, "Counter-Strike 2");
    printf("[Z] CS2 found\n");

    printf("[Z] Try connection to the arduino...\n");
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    uintptr_t client = getModuleAddress(pid, "client.dll");

    printf("[Z] Starting main thread...\n");
    while (true) {
        if (GetAsyncKeyState(VK_LSHIFT))
            AimbotMain(pid, client);
    }

    system("pause");

}
