#include "Globals.h"

void PlayerSuperSpeed()
{
    if (!bPlayerSuperSpeedEnabled)
        return;

    if (!MyPawnCached)
        return;

    if(GetAsyncKeyState(VK_LSHIFT) & 1)
    {
        SDK::FVector forwardVector = MyPlayerControllerCached->GetCamManager()->GetActorForwardVector();
        MyPawnCached->LaunchCharacter(forwardVector * 2000.0f, true, true);
    }
}
