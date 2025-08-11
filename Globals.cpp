#include "Globals.h"

bool bModMenuEnabled = false;

bool bDebugMenuCreated = false;
bool bDebugMenuEnabled = false;
bool bDebugMenuToggled = false;

bool bPlayerGodModeEnabled = false;
bool bPlayerNormalJumpEnabled = false;
bool bPlayerSuperJumpEnabled = false;
bool bPlayerSuperSpeedEnabled = false;
bool bPlayerUnlimitedAmmoEnabled = false;
bool bPlayerGetWeaponsEnabled = false;
bool bPlayerExplodingBulletsEnabled = false;
bool bPlayerTeleportToMapMarker = false;
bool bPlayerConsoleEnabled = false;
bool bPlayerNoClipEnabled = false;
bool bPlayerInvisibleEnabled = false;

bool bVehicleNoDamageEnabled = false;
bool bVehicleUnlimitedFuelEnabled = false;
bool bVehicleUnlimitedNitrosEnabled = false;

bool bSpawnBikeEnabled = false;
bool bSpawnBuddyEnabled = false;
bool bKillBuddyEnabled = false;

bool bSlowTimeEnabled = false;

SDK::UWorld* WorldCached = nullptr;
SDK::ABendPlayerPawn* MyPawnCached = nullptr;
SDK::ABendPlayerController* MyPlayerControllerCached = nullptr;
SDK::AGameCheatMenu* DebugMenuActor = nullptr;
