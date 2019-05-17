#include "mswimg.h"
#include <Shlwapi.h>
#include <wchar.h>

MSWIMG::MSWIMG()
{
	CoInitialize(NULL);

	// Create the COM imaging factory
	HRESULT hr = CoCreateInstance(
    CLSID_WICImagingFactory,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&pWicFactory));
}

MSWIMG::~MSWIMG()
{
	if(pWicFactory) pWicFactory->Release();
	CoUninitialize();
}

void MSWIMG::GetImageBytes(void *pImgData, const wchar_t *szFilename)
{
	wchar_t ext[8];
	lstrcpy(ext, PathFindExtensionW(szFilename));
	if(wmemcpy(ext, L"dds", 3)==0){
		
	}else if(wmemcpy(ext, L"png", 3)==0){

	}else if(wmemcpy(ext, L"bmp", 3)==0){

	}
	
	
}