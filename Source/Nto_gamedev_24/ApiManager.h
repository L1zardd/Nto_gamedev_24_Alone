#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApiManager.generated.h"

UCLASS()
class NTO_GAMEDEV_24_API UApiManager : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "API")
    static void RegisterTeam(FString TeamName);

    UFUNCTION(BlueprintCallable, Category = "API")
    static void SaveResources(FString Uuid, int32 ResourceAmount);

    UFUNCTION(BlueprintCallable, Category = "API")
    static void GetResources(FString Uuid);

    UFUNCTION(BlueprintCallable, Category = "API")
    static FString GenerateUUID(); // Объявление функции для генерации UUID

};