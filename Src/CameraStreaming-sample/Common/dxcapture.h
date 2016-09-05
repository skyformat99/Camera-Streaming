//
// DirectShow�� �̿��� ī�޶� ������ �������� ����� �Ѵ�.
//
// Thread Safe Version
//
// �� ��ü�� ����� ��, �������� CoUninitialize(); �Լ��� ȣ������.
//
#pragma once

#include "dxcaptureutil.h"


namespace Gdiplus {
	class Bitmap;
}


namespace cvproc
{

	class cDxCapture
	{
	public:
		cDxCapture();
		virtual ~cDxCapture();

		bool Init(const int cameraIndex, const int width, const int height, const bool isThreadMode = false);
		void UpdateCurrentBuffer();
		BYTE* GetCurrentBuffer(long &size);
		BYTE* GetCloneBuffer(long &size);
		cv::Mat& GetCloneBufferToImage(const bool cpyImage = true);
		Gdiplus::Bitmap* GetCloneBufferToBitmap();
		void WriteBitmapFromCurrentBuffer(const char* fileName);
		HRESULT WriteBitmapFromBuffer(const char* fileName, const BYTE *buffer, const long bufferSize);
		HRESULT HandleGraphEvent(void);
		void Close();


	protected:
		HRESULT GetInterfaces(void);
		HRESULT CaptureVideo(const int cameraIndex = 0);
		HRESULT FindCaptureDevice(const int cameraIndex, IBaseFilter ** ppSrcFilter);
		HRESULT SetupVideoWindow(void);
		HRESULT ChangePreviewState(int nShow);

		void Msg(TCHAR *szFormat, ...);
		void CloseInterfaces(void);
		void ResizeVideoWindow(void);

		// Remote graph viewing functions
		HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
		void RemoveGraphFromRot(DWORD pdwRegister);


	public:
		enum PLAYSTATE { STOPPED, PAUSE, RUNNING, INIT };

		int m_cameraIndex;
		int m_width;
		int m_height;
		bool m_isThreadMode;
		bool m_isUpdateBuffer; // ���۰� ������Ʈ �Ǹ� true�� �ȴ�. GetCloneBuffer() ȣ��� �ʱ�ȭ �ȴ�.
		bool m_isUnpluggedInit; // ī�޶�� ������ ������ ��, �ٽ� ����Ǿ��� ��, true �� �ȴ�.

		DWORD m_dwGraphRegister;
		IVideoWindow  * m_pVW;
		IMediaControl * m_pMC;
		IMediaEventEx * m_pME;
		IGraphBuilder * m_pGraph;
		ICaptureGraphBuilder2 * m_pCapture;
		IBaseFilter *m_pGrabberF;
		ISampleGrabber *m_pGrabber;
		IBaseFilter *m_pNullF;
		PLAYSTATE m_psCurrent;
		AM_MEDIA_TYPE m_mt;
		BYTE *m_pFrameBuffer;
		long m_frameBufferSize;
		BYTE *m_pCloneFrameBuffer;
		cv::Mat m_iplImage;
		cv::Mat m_matImage;
		Gdiplus::Bitmap *m_gdiBitmap;

		HANDLE m_handle;
		DWORD m_threadId;
		bool m_threadLoop;
		CRITICAL_SECTION m_criticalSection;

		// debug
		bool m_showFrame;
	};

}