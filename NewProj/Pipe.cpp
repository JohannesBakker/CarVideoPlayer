// Pipe.cpp: implementation of the CPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "fister.h"
#include "Pipe.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipe::CPipe()
{
	m_SoundIn.DataFromSoundIn = DataFromSoundIn;	// assign pointer to callback function
	m_SoundIn.m_pOwner = this;
	m_SoundOut.GetDataToSoundOut = GetDataToSoundOut;  // assign pointer to callback function
	m_SoundOut.m_pOwner = this;
	m_nBufPtr = 0;
}

CPipe::~CPipe()
{

}

void CPipe::DataFromSoundIn(CBuffer* buffer, void* Owner)
{
	((CPipe*) Owner)->WriteSoundDataToFile(buffer);
}

void CPipe::WriteSoundDataToFile(CBuffer* buffer)
{
	if(m_pFile)
	{
		if(!m_pFile->Write(buffer))
		{
			m_SoundIn.Stop();
			MessageBox(NULL, _T("Unable to write to file"), NULL, 0);
		}
	}

	/*
	TRACE("InTop: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());

	// add to fifo buffer
	m_FifoFull.Add(buffer);
	buffer = (CBuffer*) m_FifoEmpty.Consume();

	TRACE("InEnd: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());
	*/
}

void CPipe::GetDataToSoundOut(CBuffer* buffer, void* Owner)
{
	((CPipe*) Owner)->ReadSoundDataFromFile(buffer);
}

void CPipe::ReadSoundDataFromFile(CBuffer* buffer)
{
	memcpy(buffer->ptr.c, m_audioTrack[m_nBufPtr], 1600);
	m_nBufPtr ++;
	if(m_nBufPtr == 10) 
		m_nBufPtr = 0;
//	if(m_pFile)
//	{
//		if(!m_pFile->Read(buffer))
//		{
//			// enf of file -> tell the GUI
//			OnEndOfPlayingFile();
//			delete m_pFile;
//		}
//	}

	/*
	TRACE("OutTop: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());

	// consume from fifo buffer
	m_FifoEmpty.Add(buffer);
	buffer = (CBuffer*) m_FifoFull.Consume();

	TRACE("OutEnd: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());
	*/
}

void CPipe::StartRecordingToFile()
{
	m_pFile = new CSoundFile("sound1.wav", m_SoundIn.GetFormat());
	if(m_pFile && m_pFile->IsOK())
		m_SoundIn.Start();
}

void CPipe::StopRecordingToFile()
{
	m_SoundIn.Stop();
	// close output file
	if(m_pFile)
		delete m_pFile;
}

void CPipe::StartPlayingFromFile()
{
//	m_pFile = new CSoundFile("sound1.wav");
//	if(m_pFile && m_pFile->IsOK())
	CSoundBase base;
		m_SoundOut.Start(base.GetFormat());
//	else
//		ErrorMsg("Unable to open file");
}

void CPipe::StopPlayingFromFile()
{
	m_SoundOut.Stop();
	// close output file
	//if(m_pFile)
	//	delete m_pFile;
}

void CPipe::OnEndOfPlayingFile()
{
	// implement this function in the GUI to change things when EOF is reached
}
