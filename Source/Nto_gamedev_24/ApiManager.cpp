#include "ApiManager.h"
#include "Http.h"
#include "Json.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"
#include "Engine/Engine.h"

void UApiManager::RegisterTeam(FString TeamName)
{
    int64 Nonce = FDateTime::UtcNow().ToUnixTimestamp();
    FString TalantId = "496570 ";
    FString Signature = FMD5::HashAnsiString(*(TalantId + FString::Printf(TEXT("%lld"), Nonce)));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField("team_name", TeamName);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                FString ResponseString = Response->GetContentAsString();
                TSharedPtr<FJsonObject> JsonResponse;
                TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

                if (FJsonSerializer::Deserialize(Reader, JsonResponse))
                {
                    FString Uuid = JsonResponse->GetStringField("uuid");
                    UE_LOG(LogTemp, Log, TEXT("Team registered: %s, UUID: %s"), *TeamName, *Uuid);
                    // Сохраните Uuid для дальнейшего использования
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to register team: %s"), *Response->GetContentAsString());
            }
        });

    HttpRequest->SetURL("https://2025.nti-gamedev.ru/api/games/");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Nonce", FString::Printf(TEXT("%lld"), Nonce));
    HttpRequest->SetHeader("Talant-Id", TalantId);
    HttpRequest->SetHeader("Signature", Signature);
    HttpRequest->SetContentAsString(JsonString);
    HttpRequest->ProcessRequest();
}

void UApiManager::SaveResources(FString Uuid, int32 ResourceAmount)
{
    UE_LOG(LogTemp, Log, TEXT("Resources saved for UUID %s: %d"), *Uuid, ResourceAmount);
}


void UApiManager::GetResources(FString Uuid)
{
    UE_LOG(LogTemp, Log, TEXT("Resources requested for UUID %s"), *Uuid);
}


FString UApiManager::GenerateUUID()
{
    FGuid NewGuid = FGuid::NewGuid();
    return NewGuid.ToString();
}