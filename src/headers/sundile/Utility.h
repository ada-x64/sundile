#pragma once
#include "Common.h"
namespace sundile {
	namespace Utility {

		//Signum
		template <typename T> int signum(T val) {
			return (T(0) < val) - (val < T(0));
		}

        //https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
		template <typename T>
		std::string type_name() {
            typedef typename std::remove_reference<T>::type TR;
            std::unique_ptr<char, void(*)(void*)> own
            (
                #ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                    nullptr, nullptr),
                #else
                nullptr,
                #endif
                std::free
            );
            std::string r = own != nullptr ? own.get() : typeid(TR).name();
            if (std::is_const<TR>::value)
                r += " const";
            if (std::is_volatile<TR>::value)
                r += " volatile";
            if (std::is_lvalue_reference<T>::value)
                r += "&";
            else if (std::is_rvalue_reference<T>::value)
                r += "&&";
            return r;
        }

        template<typename T>
        void removeErase(std::vector<T>& vec, T& member) {
            vec.erase(std::remove(vec.begin(), vec.end(), member), vec.end());
        }
	}
}