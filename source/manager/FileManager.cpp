#include <manager/FileManager.h>
#include <assert.h>
#include <core/Log.h>
#if defined(ANDROID)
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
extern struct android_app* g_androidState;
#endif

namespace engine
{
#if defined(_WIN32)
	FileManager::FileManager(const char* fileName, FileOpenMode mode)
		: Stream()
		, m_mode(mode)
	{
		if (m_mode == READ_ONLY)
		{
			m_file = fopen(fileName, "rb");
		}
		else if (m_mode == READ_WRITE)
		{
			m_file = fopen(fileName, "wb");
		}
		else
		{
			assert(m_mode == READ_ONLY || m_mode == READ_WRITE);
		}

		if (!m_file)
		{
			LOGE("[%s] File %s could not be opened", __FUNCTION__, fileName);
			assert(m_file != 0);
		}
	}

	FileManager::~FileManager()
	{
		if (m_file)
		{
			fclose(m_file);
		}
	}

	void FileManager::write(const void* p, int n)
	{
		assert(m_mode == READ_WRITE);
		size_t s = fwrite(p, 1, n, m_file);
		assert(s == size_t(n));
	}

	int FileManager::read(void* p, int n)
	{
		assert(m_mode == READ_ONLY || m_mode == READ_WRITE);
		assert(available() >= n);
		size_t s = fread(p, 1, n, m_file);
		assert(s == size_t(n));
		return n;
	}

	int	FileManager::available() const
	{
		long current = ftell(m_file);
		assert(current >= 0);
		fseek(m_file, 0, SEEK_END);
		long end = ftell(m_file);
		assert(end >= 0);
		fseek(m_file, current, SEEK_SET);
		int res = end - current;
		assert(res >= 0);
		return res;
	}
#endif

#if defined(ANDROID)
	FileManager::FileManager(const char* fileName, FileOpenMode mode)
		: Stream()
		, m_mode(mode)
	{
		if (m_mode == READ_ONLY)
		{

			const int BUFFER_SIZE = 512;
			char buffer[BUFFER_SIZE];
			AAssetManager* assetManager = g_androidState->activity->assetManager;
			m_file = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
		}
		else
		{
			assert(m_mode == READ_ONLY);
		}

		if (!m_file)
		{
			LOGE("[%s] File %s could not be opened", __FUNCTION__, fileName);
			assert(m_file != 0);
		}

		m_available = AAsset_getLength(m_file);
	}

	FileManager::~FileManager()
	{
		if (m_file)
		{
			AAsset_close(m_file);
		}
	}

	void FileManager::write(const void* p, int n)
	{
		assert(m_mode == READ_WRITE);
		(void)p;
		(void)n;
	}

	int FileManager::read(void* p, int n)
	{
		assert(m_mode == READ_ONLY);
		assert(available() >= n);

		long s = AAsset_read(m_file, p, n);
		assert(s == n);
		m_available -= n;
		return n;
	}

	int	FileManager::available() const
	{
		return m_available;
	}
#endif
}