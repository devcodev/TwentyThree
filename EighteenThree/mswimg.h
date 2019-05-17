#ifndef MSWIMG_H
#define MSWIMG_H

#include <Windows.h>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

class MSWIMG
{
public:
	MSWIMG();
	~MSWIMG();

	void GetImageBytes(void *pImgData, const wchar_t *szFilename);

private:
	IWICImagingFactory *pWicFactory;
	
};

#endif MSWIMG_H