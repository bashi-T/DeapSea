#pragma once
#include<xaudio2.h>
#include<fstream>
#include"Commons/DX12Common.h"
#pragma comment(lib,"xaudio2.lib")

namespace MyEngine
{
	class AudioManager
	{
	public:

		void Initialize();
		void Finalize();

		struct ChunkHeader
		{
			char id[4];//チャンクのID
			int32_t size;//チャンクサイズ
		};
		struct RiffHeader//RIFFヘッダチャンク
		{
			ChunkHeader chunk;//"RIFF"
			char type[4];//"WAVE"
		};

		struct FormatChunk
		{
			ChunkHeader chunk;//"fmt"
			WAVEFORMATEX fmt;//波形フォーマット
		};
		struct SoundData
		{
			WAVEFORMATEX wfex;
			BYTE* pBuffer;
			int32_t bufferSize;
		};

		SoundData SoundLoadWave(const char* filename);
		void SoundUnload(SoundData* soundData);
		void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);
		static std::shared_ptr<AudioManager> GetInstance();

		ComPtr<IXAudio2> GetxAudio2() { return xAudio2_; }

		~AudioManager() = default;
		AudioManager(AudioManager&) = delete;
		AudioManager& operator=(AudioManager&) = delete;
	protected:
		AudioManager() = default;
	private:

		static inline std::weak_ptr<AudioManager> instance;
		HRESULT hr = NULL;
		ComPtr<IXAudio2>xAudio2_;
		IXAudio2MasteringVoice* masterVoice = nullptr;

	};
}

