// Pipe.h: interface for the CPipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIPE_H__71D36BFB_9EC3_4FEB_91BB_0178A52B8B7B__INCLUDED_)
#define AFX_PIPE_H__71D36BFB_9EC3_4FEB_91BB_0178A52B8B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "soundin.h"
#include "soundout.h"
#include "soundfile.h"
#include "type.h"

#define N_AUDIO_TRACNK_NUMBER	N_BUFF_NUMBER

#define N_AUDIO_TRACK_MIN		0
#define N_AUDIO_TRACK_MAX		(N_AUDIO_TRACNK_NUMBER - 1)


class CPipe
{
public:
	CPipe();
	virtual ~CPipe();

	virtual void OnEndOfPlayingFile();

public:
	void StopPlayingFromFile();
	void StartPlayingFromFile();
	void StopRecordingToFile();
	void StartRecordingToFile();
	void WriteSoundDataToFile(CBuffer* buffer);
	void ReadSoundDataFromFile(CBuffer* buffer);
	static void DataFromSoundIn(CBuffer* buffer, void* Owner);
	static void GetDataToSoundOut(CBuffer* buffer, void* Owner);
	int IncreaseVal(int nCurrVal, int nMin, int nMax, int nStep);
	int DecreaseVal(int nCurrVal, int nMin, int nMax, int nStep);

	void InitAudioTrack();

	

private:
//	CPtrFIFO m_FIFOFull;	// FIFO holding pointers to buffers with sound
//	CPtrFIFO m_FIFOEmpty;	// FIFO holding pointers to empty buffers that can be reused

	CSoundFile* m_pFile;
	CSoundOut	m_SoundOut;
	CSoundIn	m_SoundIn;

public:
	int		m_nBufPtr;
	LPVOID		m_audioTrack[N_AUDIO_TRACNK_NUMBER];
};

#endif // !defined(AFX_PIPE_H__71D36BFB_9EC3_4FEB_91BB_0178A52B8B7B__INCLUDED_)
