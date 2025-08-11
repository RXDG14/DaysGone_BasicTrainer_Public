#include "Globals.h"

void PlayerNormalJump()
{
	if(bPlayerNormalJumpEnabled)
	{
		if (!MyPawnCached)
			return;

		MyPawnCached->bCanTakeFallingDamage = false;
		MyPawnCached->bCanTakeFallingDamageBeforeLanding = false;

		if (GetAsyncKeyState(VK_MENU) & 1)
		{
			MyPawnCached->Jump();
		}
	}	
}

void PlayerSuperJump()
{
	if (bPlayerSuperJumpEnabled)
	{
		if (!MyPawnCached)
			return;

		SDK::UAnimInstance* AnimInstance = MyPawnCached->GetPawnMesh()->GetAnimInstance();
		SDK::UBendPlayerAnimInstance* PlayerAnim = (SDK::UBendPlayerAnimInstance*)AnimInstance;
		
		PlayerAnim->ShouldEnterFall = false;

		MyPawnCached->bCanTakeFallingDamage = false;
		MyPawnCached->bCanTakeFallingDamageBeforeLanding = false;

		if (GetAsyncKeyState(VK_MENU) & 1)
		{
			MyPawnCached->LaunchCharacter({ 0,0,1500.0f }, true, true);
		}
	}
}