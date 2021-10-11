#pragma once

#include <cstdint>
#include "../BPUS/h_vector.h"

#include "../NetCommon/net.h"

enum class GameMsg : uint32_t {
	Server_GetStatus,
	Server_GetPing,

	Client_Accpepted,
	Client_AssignID,
	ClientRegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

struct sPlayerDesc {
	uint32_t nUniqueID = 0;
	uint32_t nSpriteID = 0;

	h_Vector2 pos;
	float posX;
	float posY;
};
