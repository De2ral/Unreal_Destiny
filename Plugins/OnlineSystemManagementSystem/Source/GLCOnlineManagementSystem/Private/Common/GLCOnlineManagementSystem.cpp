
#include "Common/GLCOnlineManagementSystem.h"
#include <PlatformManage/Common/PlatformManageType.h>

#define LOCTEXT_NAMESPACE "FGLCOnlineManagementSystemModule"



void FGLCOnlineManagementSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGLCOnlineManagementSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FOnlineSubsystemDataManage::Get()->Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGLCOnlineManagementSystemModule,GLCOnlineManagementSystem)
