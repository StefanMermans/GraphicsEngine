#ifndef DIRECT_X_UTILS_H
#define DIRECT_X_UTILS_H

#include <d3d12.h>

namespace DXUtils {
	inline static void throwIfFailed(HRESULT hr)
	{
		if (FAILED(hr)) {
			throw std::exception("HRESULT code: " + hr);
		}
	}
}

#endif // !DIRECT_X_UTILS_H
