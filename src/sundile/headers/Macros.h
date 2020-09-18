
#ifndef SUNDILE_MACROS
#define SUNDILE_MACROS

//-- Statics for rtti
static std::vector<const char*> __componentNames;


//-- GUI
#ifdef SUNDILE_EXPORT
#define GUI
#define BEGIN_GUI
#define END_GUI
#define NO_GUI
#else
#define COMPONENT_SETTINGS //TODO: Replace this with any ImGui settings that should be set for Component editors
#define GUI(name, func) //\
	std::function<void()> __renderGui = __componentRenderFunctions.emplace_back([&]() { \
		using namespace ImGui; \
		COMPONENT_SETTINGS \
		Begin(#name); \
		func \
		End();\
	});
#define BEGIN_GUI(name) //\
	 std::function<void()> __renderGui = __componentRenderFunctions.emplace_back([&]() { \
		using namespace ImGui; \
		COMPONENT_SETTINGS \
		Begin(#name); 
		//Component's GUI function goes here
#define END_GUI //\
		End();\
	});

//TODO: This is ugly. Would very much prefer it to be automated. Find a way to replace it.
#define NO_GUI //\
	//std::function<void()> __renderGui = []() {};
#endif

//-- Component macros

struct component {};
static int num_components=0;
#define SUNDILE_COMPONENT(name, ...) \
	namespace sundile { namespace Components { struct name:component { \
	__VA_ARGS__ \
	const char* __name = #name; \
	};}}

#define BEGIN_COMPONENT(name) \
	namespace sundile { namespace Components { struct name:component { static constexpr char const* __name = #name;
#define END_COMPONENT };}};

//-- System macros
static int num_systems=0;
#define SUNDILE_SYSTEM(name, ...) \
	namespace sundile { using namespace Components; namespace Systems { namespace name { \
	__VA_ARGS__ \
	}}}

#define BEGIN_SYSTEM(name) namespace sundile { using namespace Components; namespace Systems { namespace name {
#define END_SYSTEM }}}


//--
//-- OS macros
//--
#ifdef _MSC_VER
#define SUNDILE_DEBUG_BREAK assert(0);//__debug_break();
#else
#define SUNDILE_DEBUG_BREAK assert(0);
#endif



#endif