#pragma once
#include "Common.h"
namespace sundile {
	namespace Utility {

		//Signum
		template <typename T> int signum(T val) {
			return (T(0) < val) - (val < T(0));
		}

        template<typename T>
        void removeErase(std::vector<T>& vec, T& member) {
            vec.erase(std::remove(vec.begin(), vec.end(), member), vec.end());
        }
	}
}