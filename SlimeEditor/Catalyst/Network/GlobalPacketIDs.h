#pragma once
#include "Catalyst/Types.h"

enum EGlobalPacketId : byte
{
	PlayerConnection = 250,
	NetworkTransform,
	ActorSpawn,
	NetworkId
};