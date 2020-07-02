#pragma once
#include "Model.h"
#include "Utility.h"

//This is a dummy struct right now. Will eventually replace it with a better way to define _dep.
namespace sundile {
	namespace Components {
		template <typename ...types>
		struct _dep {};
	}

}