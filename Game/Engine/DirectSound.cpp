/*********************************************************************************

  *FileName: DirectSound.cpp
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/12

**********************************************************************************/

#include"DirectSound.h"

HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);


bool CreateDMSound(CSoundInterface**pObj)
{
   if(!*pObj) *pObj = new CDMSoundObject();
   else return false;
   
   return true;
}


CDMSoundObject::CDMSoundObject()
{  
	m_filePath = nullptr;
    m_pXAudio2 = nullptr;
    pMasterVoice = nullptr;
	m_audioState = nullptr;
	m_comInit = false;
	m_totalSounds = 0;
}

int CDMSoundObject::IncreaseSounds()
{
	// This function increases the m_control array.

	if (!m_audioState)
	{
		m_audioState = new AUDIO_STATE[1];
		if (!m_audioState) return UGP_FAIL;
	}
	else
	{
		AUDIO_STATE* temp;
		temp = new AUDIO_STATE[m_totalSounds + 1];
		if (!temp) return UGP_FAIL;

		memcpy(temp, m_audioState,
			sizeof(AUDIO_STATE) * m_totalSounds);

		delete[] m_audioState;
		m_audioState = temp;
	}

	return UGP_OK;
}

bool CDMSoundObject::Initialize(const char* filepath)
{
	m_filePath = filepath;

	if (!m_comInit)
	{
		// COMライブラリの初期化
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		m_comInit = true;
	}

	if (!m_pXAudio2)
	{
		//XAudio2 エンジンのインスタンスを作成します。
		if (FAILED(XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		{
			// COMライブラリの終了処理
			CoUninitialize();

			return false;
		}
	}
	      
	if (!pMasterVoice)
	{
		// マスター音声を作成します。
		if (FAILED(m_pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		{
			if (m_pXAudio2)
			{
				// XAudio2オブジェクトの開放
				m_pXAudio2->Release();
				m_pXAudio2 = NULL;
			}

			// COMライブラリの終了処理
			CoUninitialize();

			return false;
		}
	}

	return true;
}

bool CDMSoundObject::AddSound(char* soundfile, int numRepeats, int id)
{
	if (id > m_totalSounds || id < 0 || soundfile == nullptr)
	{
		return false;
	}

	// 音声データ格納配列を増加
	if (!IncreaseSounds())
		return false;

	// サウンドデータの初期化
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	char* fileName = new char[strlen(m_filePath) + strlen(soundfile) + 1];
	ZeroMemory(fileName, strlen(m_filePath) + strlen(soundfile) + 1);
	strcat(fileName, m_filePath);
	strcat(fileName, soundfile);

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return false;
	}

	// WAVEファイルのチェック
	if (FAILED(CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition)))
	{
		return false;
	}
	if (FAILED(ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition)))
	{
		return false;
	}
	if (dwFiletype != 'EVAW')
	{
		return false;
	}

	// フォーマットチェック
	if (FAILED(CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition)))
	{
		return false;
	}
	if (FAILED(ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition)))
	{
		return false;
	}

	// オーディオデータ読み込み
	if (FAILED(CheckChunk(hFile, 'atad', &m_audioState[id].SizeAudio, &dwChunkPosition)))
	{
		return false;
	}
	m_audioState[id].pDataAudio = (BYTE*)malloc(m_audioState[id].SizeAudio);
	if (FAILED(ReadChunkData(hFile, m_audioState[id].pDataAudio, m_audioState[id].SizeAudio, dwChunkPosition)))
	{
		return false;
	}

	// ソースボイスの生成
	if (FAILED(m_pXAudio2->CreateSourceVoice(&m_audioState[id].pSourceVoice, &(wfx.Format))))
	{
		return S_FALSE;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_audioState[id].SizeAudio;
	buffer.pAudioData = m_audioState[id].pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_audioState[id].Repeats = numRepeats;

	// オーディオバッファの登録
	m_audioState[id].pSourceVoice->SubmitSourceBuffer(&buffer);

	// ファイルをクローズ
	CloseHandle(hFile);

	m_totalSounds++;

	return true;

}

bool CDMSoundObject::SetupSoundParameters(int id, float dopplerFactor,float rolloffFactor, float minDist, float maxDist)
{
   //// Set listener.
   //m_listenerParams.flDopplerFactor = dopplerFactor;
   //m_listenerParams.flRolloffFactor = rolloffFactor;
   //m_audioListener->SetAllParameters(&m_listenerParams,DS3D_IMMEDIATE);

   //// Set buffer.
   //m_bufferParams.flMinDistance = minDist;
   //m_bufferParams.flMaxDistance = maxDist;
   //m_audioBuffer->SetAllParameters(&m_bufferParams,DS3D_IMMEDIATE);

   return true;
}


void CDMSoundObject::Play(int id)
{
	// セグメント再生(再生中なら停止)
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_audioState[id].SizeAudio;
	buffer.pAudioData = m_audioState[id].pDataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_audioState[id].Repeats;

	// 状態取得
	m_audioState[id].pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_audioState[id].pSourceVoice->Stop(0);

		// オーディオバッファの削除
		m_audioState[id].pSourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_audioState[id].pSourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	m_audioState[id].pSourceVoice->Start(0);
}


void CDMSoundObject::UpdateSoundPosition(int id,float x, float y, float z)
{


}

//=============================================================================
// セグメント停止(ラベル指定) -(全て)
//=============================================================================
void CDMSoundObject::Stop(int id)
{
	XAUDIO2_VOICE_STATE xa2state;

	if (id >= 0)
	{
		// 状態取得
		m_audioState[id].pSourceVoice->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_audioState[id].pSourceVoice->Stop(0);

			// オーディオバッファの削除
			m_audioState[id].pSourceVoice->FlushSourceBuffers();
		}
	}
	else
	{
		// 全て一時停止
		for (int nCntSound = 0; nCntSound < m_totalSounds; nCntSound++)
		{
			if (m_audioState[nCntSound].pSourceVoice)
			{
				// 全て一時停止
				m_audioState[nCntSound].pSourceVoice->Stop(0);
			}
		}
	}

}

void CDMSoundObject::Shutdown()
{
   // Release all resources.

	for (int cnt = 0; cnt < m_totalSounds; cnt++)
	{
		if (m_audioState[cnt].pSourceVoice)
		{
			// 一時停止
			m_audioState[cnt].pSourceVoice->Stop(0);

			// ソースボイスの破棄
			m_audioState[cnt].pSourceVoice->DestroyVoice();
			m_audioState[cnt].pSourceVoice = NULL;

			// オーディオデータの開放
			free(m_audioState[cnt].pDataAudio);
			m_audioState[cnt].pDataAudio = NULL;
		}
	}
	

	// マスターボイスの破棄
	if (pMasterVoice)
	{
		pMasterVoice->DestroyVoice();
		pMasterVoice = NULL;
	}

	// XAudio2オブジェクトの開放
	if (m_pXAudio2)
	{
		SAFE_RELEASE(m_pXAudio2);
	}

	// COMライブラリの終了処理
	CoUninitialize();
	m_comInit = false;
}
//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}



CDirectMusicSystem::CDirectMusicSystem()
{
   m_soundList = NULL;
}

bool CDirectMusicSystem::Initialize(const char* filepath)
{
	return true;
}


bool CDirectMusicSystem::AddSound(const char *filename,int numRepeats, int *id)
{
	return true;
}


bool CDirectMusicSystem::SetupSoundParameters(int id,float dopplerFactor,float rolloffFactor,float min, float max)
{
	return true;
}


void CDirectMusicSystem::Play(int id)
{

}


void CDirectMusicSystem::UpdateSoundPosition(int id, float x,float y, float z)
{

}


void CDirectMusicSystem::Stop(int id)
{

}


void CDirectMusicSystem::Shutdown()
{

}