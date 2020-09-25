
#ifndef SUNDILE_MACROS
#define SUNDILE_MACROS

//-- Component macros
struct component {};
#define BEGIN_COMPONENT(name) \
	namespace sundile { namespace Components { struct name:component { static constexpr char const* __name = #name;
#define END_COMPONENT };}};

//-- System macros
#define BEGIN_SYSTEM(name) namespace sundile { using namespace Components; namespace Systems { namespace name {
#define END_SYSTEM }}}

//-- OS macros
#ifdef _MSC_VER
#define SUNDILE_DEBUG_BREAK assert(0);//__debug_break();
#else
#define SUNDILE_DEBUG_BREAK assert(0);
#endif

//-- GL macros
#define SUNDILE_GLSL_VERSION "#version 330 core"

#endif