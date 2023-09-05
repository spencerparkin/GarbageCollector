#pragma once

#if defined GC_EXPORT
#	define GC_API			__declspec(dllexport)
#elif defined GC_IMPORT
#	define GC_API			__declspec(dllimport)
#else
#	define GC_API
#endif