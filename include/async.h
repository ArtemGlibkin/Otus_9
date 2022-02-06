#ifndef _ASYNC_BULK
#define _ASYNC_BULK

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
	#define BULK_DLL_EXPORT __declspec(dllexport)
#else
	#define BULK_DLL_EXPORT __attribute__((visibility("default")))
#endif

	int BULK_DLL_EXPORT connect_bulk(int block_size);
	void BULK_DLL_EXPORT receive(int context, const char* buffer, int size);
	void BULK_DLL_EXPORT disconnect(int context);
#ifdef __cplusplus
}
#endif
#endif