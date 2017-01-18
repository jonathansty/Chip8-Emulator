#include "stdafx.h"
#include "chip8.h"

#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

const std::string GAME = "PONG";
GLFWwindow* pWindow = nullptr;
const int WIDTH = 640 * 2;
const int HEIGHT = 320 * 2;
Chip8* pChip8 = nullptr;

/* Error callback */
void error_callback(int code, const char* descr)
{
	std::cout << "Error " << code << ": " << descr << std::endl;
}
/* initializing glfw glew and creating the window */
bool init()
{
	glfwSetErrorCallback(error_callback);
	/* Start initialization */
	if (!glfwInit())
		return false;

	/* Set minimum required version */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	/* Create the window */
	pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Chip 8", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(pWindow);
	glewExperimental = 1;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to setup GLEW " << std::endl;
		return false;
	}
	return true;
}
void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	if (count > 0)
	{
		if (pChip8 != nullptr)
			pChip8->loadGame(paths[0]);
	}
}

void nk_gui_update(nk_context* nk_ctx)
{
	

}
void Draw(const IDrawable& d)
{
	d.onDraw(pWindow);
}
/* Main entry point */
int main(const int* argc, const char** argv)
{
	init();


	Chip8 myChip;
	pChip8 = &myChip;
	myChip.initialize();
	std::string games = "res/Games/";
	myChip.loadGame(games + GAME);

	glfwSetKeyCallback(pWindow, Chip8::setKeys);
	glfwSetDropCallback(pWindow, drop_callback);

	/* Init NK GUI */
	nk_context* nk_ctx = nk_glfw3_init(pWindow, NK_GLFW3_INSTALL_CALLBACKS);
	float alpha = 0.8f;
	nk_ctx->style.window.fixed_background = nk_style_item_color(nk_rgba_f(0.1f,0.12f,0.1f, alpha));
	nk_ctx->style.window.background = nk_rgba_f(0.1f, 0.12f, 0.1f, alpha);
	{
		nk_font_atlas* atlas;
		nk_glfw3_font_stash_begin(&atlas);
		//nk_font *default = nk_font_atlas_add_from_file(atlas, "res/arial.ttf", 13.0f, 0);
		nk_glfw3_font_stash_end();
	}

	//
	glfwSwapInterval(0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	float currTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(pWindow))
	{
		/* deltaTime calculation */
		float prevTime = currTime;
		currTime = (float)glfwGetTime();
		float dt = currTime - prevTime;

		glfwPollEvents();
		nk_glfw3_new_frame();
		Logger::NewFrame();



		/* Emulator updating*/
		myChip.emulateCycle(dt);

		/* NK gui updating */
		myChip.onGui(nk_ctx);
		glClear(GL_COLOR_BUFFER_BIT);
		/* Drawing emulator */
		Draw(myChip);
		
		// Don't forget state reset
		nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
		glfwSwapBuffers(pWindow);
	}

	/* Cleanup font atlas*/
	myChip.shutdown();
	nk_glfw3_shutdown();
	glfwTerminate();
}

