#include "Globals.h"

void SlowTime() 
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bSlowTimeEnabled) 
	{
		SDK::UGameplayStatics::SetGlobalTimeDilation(WorldCached, 0.3f);
		MyPawnCached->CustomTimeDilation = 2.5;
		MyPlayerControllerCached->CustomTimeDilation = 2.5;
		if (MyPawnCached->IsInVehicle()) 
		{
			MyPawnCached->GetVehicle()->CustomTimeDilation = 2.5;
		}
	}
	else 
	{
		SDK::UGameplayStatics::SetGlobalTimeDilation(WorldCached, 1);
		MyPawnCached->CustomTimeDilation = 1;
		MyPlayerControllerCached->CustomTimeDilation = 1;
		if (MyPawnCached->IsInVehicle())
		{
			MyPawnCached->GetVehicle()->CustomTimeDilation = 1;
		}
	}
}