#include "loop.h"

std::vector<Player> loop::player_list;

bool loop::start() 
{
    std::thread update_thread(loop::cacheLoop);

    if (update_thread.joinable()) {
        update_thread.detach();
        return true;
    }
    else
        return false;
}

bool loop::triggetbot(Entity* local_player, uintptr_t* entity_list)
{
    if (settings::triggerbot::enable)
    {
        int entity_id = driver::read<int>(shared::game::pid, local_player->address + offsets::m_iIDEntIndex);
        if (entity_id > 0)
        {
            uintptr_t entity_entry = driver::read<uintptr_t>(shared::game::pid, *entity_list + 0x8 * (entity_id >> 9) + 0x10);
            uintptr_t entity = driver::read<uintptr_t>(shared::game::pid, entity_entry + 120 * (entity_id & 0x1FF));

            if (settings::misc::only_enemies)
            {
                int entity_team = driver::read<int>(shared::game::pid, entity + offsets::m_iTeamNum);

                if (entity_team != local_player->team)
                {
                    int entity_hp = driver::read<int>(shared::game::pid, entity + offsets::m_iHealth);
                    shared::triggetbot::onCrosshair = (entity_hp > 0);
                    return true;
                }
                else {
                    shared::triggetbot::onCrosshair = false;
                    return false;
                }
            }
            else if (!settings::misc::only_enemies)
            {
                int entity_hp = driver::read<int>(shared::game::pid, entity + offsets::m_iHealth);
                shared::triggetbot::onCrosshair = (entity_hp > 0);
                return true;
            }
            else {
                shared::triggetbot::onCrosshair = false;
                return false;
            }
        }
        else {
            shared::triggetbot::onCrosshair = false;
            return false;
        }
    }
}

bool loop::update_entity(Entity* entity)
{
    uintptr_t game_scene = driver::read<uintptr_t>(shared::game::pid, entity->address + offsets::m_pGameSceneNode);
    if (!game_scene)
        return false;
    
    uintptr_t bone_array = driver::read<uintptr_t>(shared::game::pid, game_scene + offsets::m_modelState + offsets::m_vecOrigin);
    if (!bone_array)
        return false;

    entity->health = driver::read<int>(shared::game::pid, entity->address + offsets::m_iHealth);
    if (entity->health > 100 || entity->health < 1)
        return false;

    entity->team = driver::read<int>(shared::game::pid, entity->address + offsets::m_iTeamNum);
    if (entity->team != 2 && entity->team != 3)
        return false;

    entity->head = driver::read<vector3>(shared::game::pid, bone_array + 6 * 32);

    return true;
}

void loop::cacheLoop()
{

	Entity local_player = {};
    view_matrix_t vm = {};
	uintptr_t entity_list = 0;
    int max_clients = 0;
	DWORD sign_on = 0;

    DWORD pid = shared::game::pid;
    uintptr_t client = shared::game::client;
    uintptr_t engine = shared::game::engine;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        player_list.clear();
        if (shared::game::screen_size.x != GetSystemMetrics(SM_CXSCREEN)) 
        {
            shared::game::screen_size.x = GetSystemMetrics(SM_CXSCREEN);
            shared::game::screen_size.y = GetSystemMetrics(SM_CYSCREEN);
        }

        Player player{};

        uintptr_t game_client = driver::read<uintptr_t>(pid, engine + offsets::dwNetworkGameClient);
        if (!game_client) 
        {
            DWORD id = utils::process::findProcessIdByName("cs2.exe");
            if (!id || id == 1) {
                Log("[-]", "Game is closed.", RED);
                system("pause");
                exit(1);
            }
            continue;
        }
        
        sign_on = driver::read<DWORD>(pid, game_client + offsets::dwNetworkGameClient_signOnState);
        if (sign_on != SIGNONSTATE_FULL) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        max_clients = driver::read<int>(pid, game_client + offsets::dwNetworkGameClient_getMaxClients);
        if (max_clients < 2 || max_clients > 64) 
            continue;

        local_player.address = driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
        if (!local_player.address) 
            continue;
        
        vm = driver::read<view_matrix_t>(pid, client + offsets::dwViewMatrix);
        entity_list = driver::read<uintptr_t>(pid, client + offsets::dwEntityList);
        if (!entity_list)
            continue;

        if (settings::triggerbot::enable)
            std::async(std::launch::async, triggetbot, &local_player, &entity_list);
        
        for (int n_index = 1; n_index < max_clients; n_index++)
        {
            uintptr_t list_entry = driver::read<uintptr_t>(pid, entity_list + (8 * (n_index & 0x7FFF) >> 9) + 16);
            if (!list_entry)
                continue;

            uintptr_t cplayer = driver::read<uintptr_t>(pid, list_entry + 120 * (n_index & 0x1FF));
            if (!cplayer)
                continue;

            const std::uint32_t player_pawn = driver::read<std::uint32_t>(pid, cplayer + offsets::m_hPlayerPawn);
            if (!player_pawn)
                continue;

            const uintptr_t list_entry2 = driver::read<uintptr_t>(pid, entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
            if (!list_entry2)
                continue;

            Entity ent;
            ent.address = driver::read<uintptr_t>(pid, list_entry2 + 120 * (player_pawn & 0x1FF));
            if (!ent.address)
                continue;
            
            if (!update_entity(&ent))
                continue;
            
            if (ent.address == local_player.address)
            {
                local_player.team = ent.team;
                continue;
            }

            if (settings::misc::only_enemies && ent.team == local_player.team)
                continue;

            if (!world_to_screen(
                shared::game::screen_size,
                ent.head,
                player.position,
                vm
            ))
                continue;

            player_list.push_back(player);
        }
    }
}