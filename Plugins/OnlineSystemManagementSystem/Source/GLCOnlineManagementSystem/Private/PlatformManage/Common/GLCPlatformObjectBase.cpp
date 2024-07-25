#include "PlatformManage/Common/GLCPlatformObjectBase.h"
#include <PlatformManage/Common/GLCPlatformManageLibrary.h>
#include "Common/Components/GLCOnlineManageComponent.h"


UGLCPlatformObjectBase::UGLCPlatformObjectBase(const FObjectInitializer& Initializer)
{

}

AActor* UGLCPlatformObjectBase::GetObjectOwner()
{
	if (UGLCOnlineManageComponent* InManageCom = UGLCPlatformManageLibrary::GetGLCOnlineManagerComponent())
	{
		return InManageCom->GetOwner();
	}
	return nullptr;
}
