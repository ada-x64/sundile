#pragma once
#include "Common.h"
namespace sundile {
	namespace Utility {

		//Signum
		template <typename T> int signum(T val) {
			return (T(0) < val) - (val < T(0));
		}
	}
}