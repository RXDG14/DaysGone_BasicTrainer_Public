#include "Globals.h"

void CreateDebugMenu() 
{
	if (bDebugMenuCreated)
		return;

	if (DebugMenuActor) 
		return;
	
	auto Level = WorldCached->PersistentLevel;
	if (!Level)
		return;

	for (int i = 0; i < Level->Actors.Num(); ++i)
	{
		auto* actor = Level->Actors[i];
		if (actor && actor->IsA(SDK::AGameCheatMenu::StaticClass()))
		{
			DebugMenuActor = (SDK::AGameCheatMenu*)actor;
			break;
		}
	}
}

void ShowDebugMenu()
{
	if (DebugMenuActor && !bDebugMenuToggled)
	{
		DebugMenuActor->ToggleCheatMenu();
		bDebugMenuToggled = true;
	}
}

void HideDebugMenu()
{
	if (DebugMenuActor && bDebugMenuToggled)
	{
		DebugMenuActor->HideCheatMenu();
		bDebugMenuToggled = false;
	}
}

void DebugMenuMain()
{
	if (bDebugMenuEnabled)
	{
		if (!bDebugMenuCreated)
		{
			CreateDebugMenu();
			bDebugMenuCreated = true;
		}
		ShowDebugMenu();
	}
	else
	{
		HideDebugMenu();
	}
}
