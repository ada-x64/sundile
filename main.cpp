#include "Renderer.h"
#include "sundile.h"

int main(void)
{
	//Define and initialize renderer
	Renderer R;
	R.init();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(R.window))
	{
		//Render
		R.render();
	}

	glfwTerminate();
	return 0;
}