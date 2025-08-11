#include "Globals.h"

void VehicleNoDamage()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bVehicleNoDamageEnabled)
	{
		if (MyPawnCached->GetVehicle() && MyPawnCached->IsInVehicle())
		{
			MyPawnCached->GetVehicle()->StopDamageForMission(1);
		}
	}
}

void VehicleUnlimitedFuel() 
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bVehicleUnlimitedFuelEnabled)
	{
		if (MyPawnCached->GetVehicle())
		{
			if(MyPawnCached->IsInVehicle())
			{
				MyPawnCached->GetVehicle()->FuelComponent->FuelRemaining = MyPawnCached->GetVehicle()->FuelComponent->FuelCapacity;
			}
		}
	}
}

void VehicleUnlimitedNitros()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bVehicleUnlimitedNitrosEnabled) 
	{
		if (GetAsyncKeyState(VK_MENU)) 
		{
			if (MyPawnCached->GetVehicle() && MyPawnCached->IsInVehicle())
			{
				MyPawnCached->GetVehicle()->ActivateBoostForTime(1, true, false);
			}
		}
	}
}