// SoundOut.cpp: implementation of the CSoundOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SOUND_BUFFER_REFILLING_NUMBER	3 // original - 2

#define N_INIT_BUFF_ADDING		5

#define ANSI_NEW	1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// IMPLEMENT_DYNCREATE(CSoundOut, CWinThread)

CSoundOut::CSoundOut()
{
	m_QueuedBuffers = 0;
	m_hPlay = NULL;
	m_bPlaying = false;
	GetDataToSoundOut = NULL;
	m_pOwner = NULL;
	CreateThread();
	m_bAutoDelete = false;

#if ANSI_NEW

	// alloc every buffer
	for (int i = 0; i < SOUNDOUT_BUFF_NUMBER; i++)
	{
		m_Buff[i].allocBuffer(m_BufferSize, false);
		m_pHdr[i] = new WAVEHDR;
	}

	m_nCurrBufIndex = 0;
	m_nCurrHdrIndex = 0;

#endif
}

CSoundOut::~CSoundOut()
{
	if (m_bPlaying)
		Stop();
	::PostQuitMessage(0);
}

BOOL CSoundOut::InitInstance()
{
	m_ThreadID = ::GetCurrentThreadId();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSoundOut, CWinThread)
	//{{AFX_MSG_MAP(CSoundOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnMM_WOM_DONE)
END_MESSAGE_MAP()


#if ANSI_NEW

bool CSoundOut::Start(WAVEFORMATEX* format)
{
	MMRESULT mmReturn = 0;
	
	if (m_bPlaying || GetDataToSoundOut == NULL || m_pOwner == NULL)
	{
		// already recording!
		return FALSE;
	}
	else
	{
		for (int i = 0; i < SOUNDOUT_BUFF_NUMBER; i++)
		{
			m_Buff[i].Erase();
		}
		m_nCurrBufIndex = 0;
		
		
		if (format != NULL)		// was this function called with at specific format
			m_Format = *format;
		
		// open wavein device
		mmReturn = ::waveOutOpen( &m_hPlay, WAVE_MAPPER, &m_Format, m_ThreadID, NULL, CALLBACK_THREAD);
		
		//waveOutSetPlaybackRate(m_hPlay, 0x000F8000);
		//waveOutSetPitch(m_hPlay, 0x000F8000);
		
		if (mmReturn)
		{
			waveOutErrorMsg(mmReturn, (LPCTSTR)("in start()"));
			return FALSE;
		}
		else
		{
			m_bPlaying = true;

			for (int i = 0; i < N_INIT_BUFF_ADDING; i++)
			{
				m_Buff[m_nCurrBufIndex].ByteLen = 1600;
				
				GetDataToSoundOut(&m_Buff[m_nCurrBufIndex], m_pOwner);
				AddOutputBufferToQueue(&m_Buff[m_nCurrBufIndex]);

				m_nCurrBufIndex ++;
				if (m_nCurrBufIndex >= SOUNDOUT_BUFF_NUMBER)
					m_nCurrBufIndex = 0;
			}
		}
	}
	return TRUE;

}

#else

bool CSoundOut::Start(WAVEFORMATEX* format)
{
	MMRESULT mmReturn = 0;
	
	if (m_bPlaying || GetDataToSoundOut == NULL || m_pOwner == NULL)
	{
		// already recording!
		return FALSE;
	}
	else
	{
		if (format != NULL)		// was this function called with at specific format
			m_Format = *format;
		
		// open wavein device
		mmReturn = ::waveOutOpen( &m_hPlay, WAVE_MAPPER, &m_Format, m_ThreadID, NULL, CALLBACK_THREAD);
		
		
		//waveOutSetPlaybackRate(m_hPlay, 0x000F8000);
		//waveOutSetPitch(m_hPlay, 0x000F8000);
		
		if (mmReturn)
		{
			waveOutErrorMsg(mmReturn, (LPCTSTR)("in start()"));
			return FALSE;
		}
		else
		{
			m_bPlaying = true;

			waveOutSetPlaybackRate(m_hPlay, 8000);

			// we need at least 2 -> one for playing, one for refilling (3 is better)
			for (int i = 0; i < SOUND_BUFFER_REFILLING_NUMBER; i++)			
 			{
				CBuffer buf(/*m_Format.nBlockAlign**/m_BufferSize, false);
				GetDataToSoundOut(&buf, m_pOwner);
				AddOutputBufferToQueue(&buf);
			}
		}
	}
	return TRUE;

}

#endif

void CSoundOut::Stop()
{
	MMRESULT mmReturn = 0;

	if (m_bPlaying)
	{
		m_bPlaying = FALSE;
		mmReturn = ::waveOutReset(m_hPlay);
		if (mmReturn) waveOutErrorMsg(mmReturn, (LPCTSTR)("in stop()"));
		TRACE((LPCTSTR)("waveOutReset()\n"));
	}
}

#if ANSI_NEW
void CSoundOut::OnMM_WOM_DONE(UINT parm1, LONG parm2)
{
		
	MMRESULT mmReturn = 0;
	
	LPWAVEHDR pHdr = (LPWAVEHDR) parm2;
	mmReturn = ::waveOutUnprepareHeader(m_hPlay, pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveOutErrorMsg(mmReturn, (LPCTSTR)("in OnWOM_DONE()"));
		return;
	}

	m_QueuedBuffers--;

	if (m_bPlaying)
	{
		int i = m_nCurrBufIndex;

		m_nCurrBufIndex ++;
		if (m_nCurrBufIndex >= SOUNDOUT_BUFF_NUMBER)
			m_nCurrBufIndex = 0;
		
		// virtual function supplyed by user
		m_Buff[i].ByteLen = 1600;
		GetDataToSoundOut(&m_Buff[i], m_pOwner);

		if (m_Buff[i].ByteLen > 0)
		{
			AddOutputBufferToQueue(&m_Buff[i]);
			TRACE((LPCTSTR)("WOM_DONE : refill buffer\n"));
			// delete old header
			delete pHdr;
			return;			
		}
		else
		{
			Stop();
		}
	}

	// we are closing the waveOut handle, 
	// all data must be deleted
	// this buffer was allocated in Start()
	delete pHdr;
	TRACE((LPCTSTR)("WOM_DONE : remove buffer\n"));

	if (m_bPlaying == false)
	{
		mmReturn = ::waveOutClose(m_hPlay);
		if (mmReturn) 
			waveOutErrorMsg(mmReturn, (LPCTSTR)("in stop()"));
		TRACE((LPCTSTR)("waveOutClose()\n"));
	}
}

int CSoundOut::AddOutputBufferToQueue(CBuffer *buffer)
{
	MMRESULT mmReturn = 0;
	
	// create the header
	LPWAVEHDR pHdr = new WAVEHDR;
	if (pHdr == NULL) return NULL;

	// new a buffer
	pHdr->lpData = buffer->ptr.c;
	pHdr->dwBufferLength = buffer->ByteLen;
	pHdr->dwFlags = 0;

	pHdr->dwFlags = WHDR_DONE;

	// prepare it
	mmReturn = ::waveOutPrepareHeader(m_hPlay,pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveOutErrorMsg(mmReturn, (LPCTSTR)("in AddOutputBufferToQueue()"));
		return m_QueuedBuffers;
	}
	// write the buffer to output queue
	mmReturn = ::waveOutWrite(m_hPlay, pHdr, sizeof(WAVEHDR));

	if (mmReturn) waveOutErrorMsg(mmReturn, (LPCTSTR)("in AddOutputBufferToQueue()"));
	// increment the number of waiting buffers
	return m_QueuedBuffers++;
}

#else

void CSoundOut::OnMM_WOM_DONE(UINT parm1, LONG parm2)
{
		
	MMRESULT mmReturn = 0;
	
	LPWAVEHDR pHdr = (LPWAVEHDR) parm2;
	mmReturn = ::waveOutUnprepareHeader(m_hPlay, pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveOutErrorMsg(mmReturn, (LPCTSTR)("in OnWOM_DONE()"));
		return;
	}

	m_QueuedBuffers--;

	if (m_bPlaying)
	{		
		CBuffer buf(pHdr->lpData, pHdr->dwBufferLength);
		
		// virtual function supplyed by user
		GetDataToSoundOut(&buf, m_pOwner);
		
		if (buf.ByteLen > 0)
		{
			AddOutputBufferToQueue(&buf);
			TRACE((LPCTSTR)("WOM_DONE : refill buffer\n"));
			// delete old header
			delete pHdr;
			return;			
		}
		else
		{
			Stop();
		}
	}

	// we are closing the waveOut handle, 
	// all data must be deleted
	// this buffer was allocated in Start()
	delete pHdr->lpData;
	delete pHdr;
	TRACE((LPCTSTR)("WOM_DONE : remove buffer\n"));

	if (m_QueuedBuffers == 0 && m_bPlaying == false)
	{
		mmReturn = ::waveOutClose(m_hPlay);
		if (mmReturn) waveOutErrorMsg(mmReturn, (LPCTSTR)("in stop()"));
		TRACE((LPCTSTR)("waveOutClose()\n"));
	}
}

int CSoundOut::AddOutputBufferToQueue(CBuffer *buffer)
{
	MMRESULT mmReturn = 0;
	
	// create the header
	LPWAVEHDR pHdr = new WAVEHDR;
	if (pHdr == NULL) return NULL;

	// new a buffer
	pHdr->lpData = buffer->ptr.c;
	pHdr->dwBufferLength = buffer->ByteLen;
	pHdr->dwFlags = 0;
	
	// prepare it
	mmReturn = ::waveOutPrepareHeader(m_hPlay,pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveOutErrorMsg(mmReturn, (LPCTSTR)("in AddOutputBufferToQueue()"));
		return m_QueuedBuffers;
	}
	// write the buffer to output queue
	mmReturn = ::waveOutWrite(m_hPlay, pHdr, sizeof(WAVEHDR));
	if (mmReturn) waveOutErrorMsg(mmReturn, (LPCTSTR)("in AddOutputBufferToQueue()"));
	// increment the number of waiting buffers
	return m_QueuedBuffers++;
}


#endif



void CSoundOut::waveOutErrorMsg(MMRESULT result, LPCTSTR addstr)
{
	return;

	// say error message
#ifdef UNICODE
	wchar_t errorbuffer[100];
#else
	char errorbuffer[100];
#endif
	waveOutGetErrorText(result, errorbuffer,100);
	ErrorMsg((LPCTSTR)("WAVEOUT:%x:%s %s"), result, errorbuffer, addstr);
}
