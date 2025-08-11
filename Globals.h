#pragma once
#include <unordered_set>

#include "../SDK/Basic.hpp"
#include "../SDK/CoreUObject_classes.hpp"
#include "../SDK/Engine_classes.hpp"
#include "../SDK/UMG_classes.hpp"
#include "../SDK/BendGame_classes.hpp"
#include "../SDK/BendDefaultGameMode_classes.hpp"

extern bool bModMenuEnabled;

extern bool bDebugMenuCreated;
extern bool bDebugMenuEnabled;
extern bool bDebugMenuToggled;

extern bool bPlayerGodModeEnabled;
extern bool bPlayerNormalJumpEnabled;
extern bool bPlayerSuperJumpEnabled;
extern bool bPlayerSuperSpeedEnabled;
extern bool bPlayerUnlimitedAmmoEnabled;
extern bool bPlayerGetWeaponsEnabled;
extern bool bPlayerExplodingBulletsEnabled;
extern bool bPlayerTeleportToMapMarker;
extern bool bPlayerConsoleEnabled;
extern bool bPlayerNoClipEnabled;
extern bool bPlayerInvisibleEnabled;

extern bool bVehicleNoDamageEnabled;
extern bool bVehicleUnlimitedFuelEnabled;
extern bool bVehicleUnlimitedNitrosEnabled;

extern bool bSpawnBikeEnabled;
extern bool bSpawnBuddyEnabled;
extern bool bKillBuddyEnabled;

extern bool bSlowTimeEnabled;

extern SDK::UWorld* WorldCached;
extern SDK::ABendPlayerPawn* MyPawnCached;
extern SDK::ABendPlayerController* MyPlayerControllerCached;
extern SDK::AGameCheatMenu* DebugMenuActor;

