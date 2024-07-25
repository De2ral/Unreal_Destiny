#include "PlatformManage/OnlineSessions/DataTable/MapSettings.h"

FMapSettings::FMapSettings()
{
	SaveGameSlotName = TEXT("OnlineSessionsConfig");
}

FString FMapSettings::GetLobbyMapPath(int32 Index) const
{
	if(Index == INDEX_NONE)
	{
		Index = FMath::RandRange(0,Lobby.Num() - 1);
	}
	checkf(Lobby.IsValidIndex(Index),TEXT("[%d] Make sure the lobby index is valid ,please set lobby map in the data table"),Index);
	
	if(Lobby.IsValidIndex(Index))
	{
		return FPackageName::ObjectPathToPackageName(Lobby[Index].ToString());
	}
	return FString();
}
