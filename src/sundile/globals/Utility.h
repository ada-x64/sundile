#pragma once
#ifndef SUNDILE_UTILITY
#define SUNDILE_UTILITY
namespace sundile {

	static std::string asset_directory;

	//Signum
	template <typename T> int signum(T val) {
		return (T(0) < val) - (val < T(0));
	}
	template<typename T>
	void removeErase(std::vector<T>& vec, T& member) {
		vec.erase(std::remove(vec.begin(), vec.end(), member), vec.end());
	}

	//Returns true if replaced
	template<typename T>
	bool addOrReplace(std::vector<T>& vec, T& element) {
		for (auto it = vec.begin(); it < vec.end(); ++it) {
			if (*it == element) {
				*it = element;
				return true;
			}
		}
		vec.push_back(element);
		return false;
	}
	template<typename T>
	bool addOrReplace(std::vector<T*>& vec, T* element) {
		for (auto it = vec.begin(); it < vec.end(); ++it) {
			if (*it == element) {
				*it = element;
				return true;
			}
		}
		vec.push_back(element);
		return false;
	}
	//Returns T value or nullptr
	template<typename T>
	bool find(std::vector<T>& vec, T& element) {
		for (auto it = vec.begin(); it < vec.end(); ++it) {
			if (*it == element) {
				return true;
			}
		}
		return false;
	}
	//Returns T value or nullptr
	template<typename T>
	bool find(std::vector<T*>& vec, T* element) {
		for (auto it = vec.begin(); it < vec.end(); ++it) {
			if (*it == element) {
				return true;
			}
		}
		return false;
	}

}
#endif