#include "Globals.h"

void SpawnBike()
{
	if (!bSpawnBikeEnabled || !MyPawnCached || !MyPlayerControllerCached || !WorldCached)
		return;

	auto gameMode = (SDK::ABendDefaultGameMode_C*)WorldCached->AuthorityGameMode;
	if (!gameMode || !gameMode->PlayerBikeRef)
		return;

	auto bikeClass = gameMode->PlayerBikeRef->Get();
	if (!bikeClass)
		return;

	SDK::FTransform spawnTransform;
	spawnTransform.Translation = MyPawnCached->K2_GetActorLocation();
	spawnTransform.Rotation = {0,0,0,1};
	spawnTransform.Scale3D = { 1.f, 1.f, 1.f };

	MyPlayerControllerCached->SpawnPlayerVehicle(0, bikeClass, spawnTransform, true);
	bSpawnBikeEnabled = false;
}

//void SpawnBuddy() // <-- deacon clone without logic
//{
//    if (!bSpawnBuddyEnabled)
//        return;
//
//    SDK::APawn* SpawnedPawn = WorldCached->AuthorityGameMode->SpawnDefaultPawnFor(MyPlayerControllerCached, MyPawnCached);
//    if (SpawnedPawn)
//    {
//        SDK::AController* Controller = SpawnedPawn->GetController();
//        if (Controller)
//        {
//            std::cout << "Pawn is possessed by: " << Controller->GetName() << std::endl;
//        }
//        else
//        {
//            std::cout << "Pawn has NO controller." << std::endl;
//        }
//    }
//
//    bSpawnBuddyEnabled = false;
//}

static std::vector<SDK::ABendAIController*> BuddyControllers;
void SpawnBuddy()
{
    if (!bSpawnBuddyEnabled || !WorldCached || !MyPawnCached)
        return;

    bSpawnBuddyEnabled = false;

    if (BuddyControllers.size() >= 10)
    {
        return;
    }

    int actorCount = WorldCached->PersistentLevel->Actors.Num();
    SDK::ABendAIController* selectedController = nullptr;
    SDK::ABendAIController* fallbackController = nullptr;

    for (int i = 0; i < actorCount; ++i)
    {
        SDK::AActor* actor = WorldCached->PersistentLevel->Actors[i];
        if (!actor || !actor->IsA(SDK::ABendAIController::StaticClass()))
            continue;

        auto* AIController = static_cast<SDK::ABendAIController*>(actor);
        if (!AIController->bPooledSpawnInUse || !AIController->Pawn)
            continue;

        // Skip already tracked
        if (std::find(BuddyControllers.begin(), BuddyControllers.end(), AIController) != BuddyControllers.end())
            continue;

        std::string pawnName = AIController->Pawn->GetName();
        
        // Prioritize Marauder
        if (!selectedController && pawnName.find("Marauder") != std::string::npos)
        {
            selectedController = AIController;
            break;
        }

        // Fallback: not Deer
        if (!fallbackController && pawnName.find("Deer") == std::string::npos)
        {
            fallbackController = AIController;
        }
    }

    // Prefer Marauder, fall back to non-Deer
    if (!selectedController)
        selectedController = fallbackController;

    if (!selectedController)
    {
        return;
    }

    SDK::ABendAIPawn* pawn = static_cast<SDK::ABendAIPawn*>(selectedController->K2_GetPawn());
    if (pawn)
    {
        SDK::FVector playerLocation = MyPawnCached->K2_GetActorLocation();
        SDK::FHitResult dummyHit;

        selectedController->SetAIFactionType(SDK::EAIFactionType::Buddy);
        pawn->K2_SetActorLocation(playerLocation, false, &dummyHit, true);

        BuddyControllers.push_back(selectedController);
    }
}

void BuddyTick()
{
    if (!WorldCached || !MyPawnCached || !MyPlayerControllerCached)
        return;

    SDK::FVector playerLocation = MyPawnCached->K2_GetActorLocation();

    for (auto i = BuddyControllers.begin(); i != BuddyControllers.end(); )
    {
        SDK::ABendAIController* controller = *i;
        SDK::ABendAIPawn* pawn = controller ? static_cast<SDK::ABendAIPawn*>(controller->K2_GetPawn()) : nullptr;

        if (!controller || !pawn || !pawn->IsAlive())
        {
            i = BuddyControllers.erase(i);
            continue;
        }
        ++i;
    }

    for (auto* controller : BuddyControllers)
    {
        SDK::ABendAIPawn* pawn = static_cast<SDK::ABendAIPawn*>(controller->K2_GetPawn());
        if (!pawn)
            continue;

        float dist = (pawn->K2_GetActorLocation() - playerLocation).Magnitude();
        if (dist > 2000)
        {
            SDK::FVector newLocation = playerLocation;
            newLocation.Z += 300;
            SDK::FHitResult dummyHit;
            pawn->SetFactionType(SDK::EAIFactionType::Buddy); // level changed
            pawn->K2_SetActorLocation(newLocation, false, &dummyHit, true);
        }
    }
}

void KillAllBuddies()
{
    if (!bKillBuddyEnabled)
        return;
    
    bKillBuddyEnabled = false;
    
    if (BuddyControllers.empty())
    {
        return;
    }

    for (auto* controller : BuddyControllers)
    {
        if (!controller)
            continue;

        SDK::ABendAIPawn* pawn = static_cast<SDK::ABendAIPawn*>(controller->K2_GetPawn());
        if (pawn && pawn->IsAlive())
        {
            SDK::ABendAIController* controller = static_cast<SDK::ABendAIController*>(pawn->Controller);
            if(controller)
            {
                controller->Despawn(1);
            }
        }
    }

    BuddyControllers.clear();
}