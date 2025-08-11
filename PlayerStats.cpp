#include "Globals.h"


static float OriginalHealthRegenDelay = 0;
static float OriginalHealthRegenRate = 1;
static bool bOriginalPlayerGodModeValuesCached = false;
void PlayerGodMode()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;
	
	if (!bOriginalPlayerGodModeValuesCached)
	{
		OriginalHealthRegenDelay = MyPawnCached->HealthRegenDelay;
		OriginalHealthRegenRate = MyPawnCached->HealthRegenRate;
		bOriginalPlayerGodModeValuesCached = true;
	}

	if (bPlayerGodModeEnabled) 
	{
		MyPawnCached->m_bIgnoreDamage = true;
		MyPawnCached->Health = MyPawnCached->MaxHealth;
		MyPawnCached->HealthRegenDelay = 0.f;
		MyPawnCached->HealthRegenRate = 100.0f;
		MyPlayerControllerCached->SetFocusToMax();
	}
	else 
	{
		MyPawnCached->m_bIgnoreDamage = false;
		MyPawnCached->HealthRegenDelay = OriginalHealthRegenDelay;
		MyPawnCached->HealthRegenRate = OriginalHealthRegenRate;
	}
}


static bool bOriginalPlayerUnlimitedAmmoEnabled = false;
static bool bOriginalPlayerExplodingBulletsEnabled = false;
void PlayerGetWeapons()
{
	if (!bPlayerGetWeaponsEnabled || !WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	bPlayerGetWeaponsEnabled = false;

	bOriginalPlayerUnlimitedAmmoEnabled = bPlayerUnlimitedAmmoEnabled;
	bOriginalPlayerExplodingBulletsEnabled = bPlayerExplodingBulletsEnabled;
	bPlayerUnlimitedAmmoEnabled = false;
	bPlayerExplodingBulletsEnabled = false;

	if (MyPawnCached->IsAlive() && !MyPawnCached->IsInVehicle())
	{
		SDK::ABendWeapon* OutWeapon1 = nullptr;
		SDK::ABendWeapon* OutWeapon2 = nullptr;

		SDK::FLatentActionInfo Latent1{};
		Latent1.CallbackTarget = MyPawnCached;
		Latent1.UUID = 1;

		SDK::FLatentActionInfo Latent2{};
		Latent2.CallbackTarget = MyPawnCached;
		Latent2.UUID = 2;

		MyPawnCached->SetCanEquipWeapons(true);

		MyPawnCached->AddWeapon_NEW(
			SDK::EInventoryWeaponID::kWeaponID_HoneyBadger,
			nullptr,
			Latent1,
			SDK::EEquippableSlot(),
			SDK::EInventoryAmmoID::kAmmoID_HoneyBadger,
			false
		);
		MyPawnCached->AddWeapon_NEW(
			SDK::EInventoryWeaponID::kWeaponID_AK47,
			nullptr,
			Latent2,
			SDK::EEquippableSlot(),
			SDK::EInventoryAmmoID::kAmmoID_AK47,
			false
		);
	}

	bPlayerUnlimitedAmmoEnabled = bOriginalPlayerUnlimitedAmmoEnabled;
	bPlayerExplodingBulletsEnabled = bOriginalPlayerExplodingBulletsEnabled;
}


void PlayerUnlimitedAmmo()
{
	if (!bPlayerUnlimitedAmmoEnabled || !WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if(MyPawnCached->IsEquippedWeaponGun(SDK::EEquippableSlot()))
	{
		MyPawnCached->GetWeapon(SDK::EEquippableSlot())->SetCurrentAmmoInClip(100);
	}
}


static std::vector<SDK::EInventoryWeaponID> SavedWeaponIDs;
static std::vector<SDK::EInventoryAmmoID> SavedAmmoIDs;
void PlayerExplodingBullets()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bPlayerExplodingBulletsEnabled)
	{
		SDK::ABendWeapon* CurrentWeapon = MyPawnCached->GetWeapon(SDK::EEquippableSlot());
		if (!CurrentWeapon || !MyPawnCached->IsEquippedWeaponGun(SDK::EEquippableSlot()))
			return;

		SDK::EInventoryWeaponID WeaponID = CurrentWeapon->GetInventoryWeaponID();

		// Save only if not already in our saved list
		auto it = std::find(SavedWeaponIDs.begin(), SavedWeaponIDs.end(), WeaponID);
		if (it == SavedWeaponIDs.end())
		{
			SavedWeaponIDs.push_back(WeaponID);
			SavedAmmoIDs.push_back(CurrentWeapon->AmmoID);

			CurrentWeapon->SetAmmoType(SDK::EInventoryAmmoID::kAmmoID_Molotov);
		}
	}
	else
	{
		SDK::ABendWeapon* CurrentWeapon = MyPawnCached->GetWeapon(SDK::EEquippableSlot());
		if (!CurrentWeapon || !MyPawnCached->IsEquippedWeaponGun(SDK::EEquippableSlot()))
			return;

		SDK::EInventoryWeaponID WeaponID = CurrentWeapon->GetInventoryWeaponID();

		for (size_t i = 0; i < SavedWeaponIDs.size(); ++i)
		{
			if (SavedWeaponIDs[i] == WeaponID)
			{
				CurrentWeapon->SetAmmoType(SavedAmmoIDs[i]);
				break;
			}
		}
		// Optional: clear cache when turning off
		SavedWeaponIDs.clear();
		SavedAmmoIDs.clear();
	}
}


void PlayerTeleportToMapMarker()
{
	if (!bPlayerTeleportToMapMarker || !WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bPlayerNoClipEnabled) bPlayerNoClipEnabled = false;

	bPlayerTeleportToMapMarker = false;
	
	int actors = WorldCached->PersistentLevel->Actors.Num();
	for (int i = 0; i < actors; i++)
	{
		if (WorldCached->PersistentLevel->Actors[i]->IsA(SDK::AMapWaypointMarker::StaticClass()))
		{
			//MapWaypointMarkerBP_C_1
			if (WorldCached->PersistentLevel->Actors[i]->GetName() == "MapWaypointMarkerBP_C_1") 
			{
				SDK::AMapWaypointMarker* mapMarker = static_cast<SDK::AMapWaypointMarker*>(WorldCached->PersistentLevel->Actors[i]);
				if(mapMarker)
				{
					SDK::FVector location = mapMarker->GetEndLocation();
					SDK::FVector zero = { 0,0,0 };
					if (mapMarker->HasValidNavPath()) //&& (mapMarker->WaypointMarkerType == SDK::EMapWaypointMarker::UserWaypoint || mapMarker->WaypointMarkerType == SDK::EMapWaypointMarker:)
					{
						SDK::FVector lineTraceStart = location;
						SDK::FVector lineTraceEnd = location;
						lineTraceEnd.Z = location.Z - 50000.0f;
						//lineTraceEnd.Z = lineTraceEnd.Z * -50000; <-- causes game crash, too large value

						SDK::TArray<SDK::AActor*> IgnoreActors;
						IgnoreActors.Add(MyPawnCached);

						SDK::FHitResult hitResult = {};

						bool bHit = SDK::UKismetSystemLibrary::LineTraceSingle_NEW(
							WorldCached,
							lineTraceStart,
							lineTraceEnd,
							SDK::ETraceTypeQuery::TraceTypeQuery1,
							false,
							IgnoreActors,
							SDK::EDrawDebugTrace::None,
							&hitResult,
							true
						);

						if (bHit)
						{
							SDK::FVector targetLocation = hitResult.Location;
							targetLocation.Z += 300;
							if (MyPawnCached->IsInVehicle())
							{
								MyPawnCached->GetVehicle()->K2_SetActorLocation(targetLocation, false, nullptr, true);
							}
							else
							{
								MyPawnCached->K2_SetActorLocation(targetLocation, false, nullptr, true);
							}
						}
						else
						{
							if (MyPawnCached->IsInVehicle())
							{
								MyPawnCached->GetVehicle()->K2_SetActorLocation(location, false, nullptr, true);
							}
							else
							{
								MyPawnCached->K2_SetActorLocation(location, false, nullptr, true);
							}
						}
					}

					mapMarker->ClearPath();
					break;
				}
			}
		}
	}
}


static bool bPrevPlayerConsoleEnabled = false;
void PlayerConsole()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bPlayerConsoleEnabled && !bPrevPlayerConsoleEnabled)
	{
		SDK::UEngine* Engine = SDK::UEngine::GetEngine();
		SDK::UInputSettings::GetDefaultObj()->ConsoleKeys[0].KeyName = SDK::UKismetStringLibrary::Conv_StringToName(L"F4");
		SDK::UObject* NewObject = SDK::UGameplayStatics::SpawnObject(Engine->ConsoleClass, Engine->GameViewport);
		Engine->GameViewport->ViewportConsole = static_cast<SDK::UConsole*>(NewObject);
	}

	bPrevPlayerConsoleEnabled = bPlayerConsoleEnabled;
}


static bool bOriginalCollisionProfileNameCached = false;
static SDK::FName OriginalCollisionProfileName;
static bool bWasPlayerNoClipEnabled = false;
void PlayerNoClip()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	// Just enabled
	if (bPlayerNoClipEnabled && !bWasPlayerNoClipEnabled)
	{
		if (!bOriginalCollisionProfileNameCached)
		{
			OriginalCollisionProfileName = MyPawnCached->CapsuleComponent->GetCollisionProfileName();
			bOriginalCollisionProfileNameCached = true;
		}

		SDK::FName NoCollision = SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(L"NoCollision"));
		MyPawnCached->CapsuleComponent->SetCollisionProfileName(NoCollision);
		MyPawnCached->CapsuleComponent->SetEnableGravity(0);

		if (MyPawnCached->CharacterMovement)
		{
			MyPawnCached->CharacterMovement->DisableMovement();
			MyPawnCached->CharacterMovement->StopMovementImmediately();
			MyPawnCached->CharacterMovement->SetComponentTickEnabled(false);
		}
	}

	if (bPlayerNoClipEnabled)
	{
		float Speed = 0.5f; // Units per second

		SDK::FVector MoveDir(0, 0, 0);

		if (GetAsyncKeyState('W') & 0x8000) MoveDir += MyPlayerControllerCached->GetCamManager()->GetActorForwardVector();
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) MoveDir += MyPawnCached->GetActorUpVector();
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) MoveDir -= MyPawnCached->GetActorUpVector();

		if (!MoveDir.IsZero())
		{
			MoveDir.Normalize();
			SDK::FVector NewLocation = MyPawnCached->K2_GetActorLocation() + (MoveDir * Speed);
			MyPawnCached->K2_SetActorLocation(NewLocation, false, nullptr, true);
		}
	}


	// Just disabled
	if (!bPlayerNoClipEnabled && bWasPlayerNoClipEnabled)
	{
		MyPawnCached->CapsuleComponent->SetCollisionProfileName(OriginalCollisionProfileName);
		MyPawnCached->CapsuleComponent->SetEnableGravity(1);

		if (MyPawnCached->CharacterMovement)
		{
			MyPawnCached->CharacterMovement->SetComponentTickEnabled(true);
			MyPawnCached->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Walking, 0);
		}
	}

	bWasPlayerNoClipEnabled = bPlayerNoClipEnabled;
}


void PlayerInvisible()
{
	if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
		return;

	if (bPlayerInvisibleEnabled)
	{
		if (MyPawnCached->GetPawnNoiseEmitterComponent())
		{
			MyPawnCached->Hide(true, true);
		}
	}
	else
	{
		if (MyPawnCached->GetPawnNoiseEmitterComponent())
		{
			MyPawnCached->Unhide();
		}
	}
}

