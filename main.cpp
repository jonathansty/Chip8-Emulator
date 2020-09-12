#include "stdafx.h"
#include "chip8.h"

#include "nuklear.h"
#include "nuklear_glfw_gl4.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

const std::string GAME = "PONG";
GLFWwindow* g_pWindow = nullptr;
const int WIDTH = 640 * 2;
const int HEIGHT = 320 * 2;
std::shared_ptr<IEmulator> s_current_emulator;

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
	g_pWindow = glfwCreateWindow(WIDTH, HEIGHT, "Chip 8", nullptr, nullptr);


	if (!g_pWindow)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(g_pWindow);
	glewExperimental = 1;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to setup GLEW " << std::endl;
		return false;
	}
	return true;
}
static void on_file_drop_callback(GLFWwindow* window, int count, const char** paths)
{
	if (count > 0)
	{
		if (s_current_emulator != nullptr)
			s_current_emulator->load_rom(paths[0]);
	}
}

static void on_set_key_callback(GLFWwindow* window, int key, int scannode, int action, int mods)
{
	if (s_current_emulator)
	{
		s_current_emulator->set_keys(window, key, scannode, action, mods);
	}

}

void nk_gui_update(nk_context* nk_ctx)
{

}

void Draw(const IDrawable& d)
{
	d.on_draw(g_pWindow);
}
/* Main entry point */
int main(const int* argc, const char** argv)
{
	// Setup the basic window features
	init();

	// Create the Chip8 emulator by default
	s_current_emulator = std::make_shared<Chip8>();
	s_current_emulator->init();

	// Setup the games path
	std::string games = "res/Games/";
	s_current_emulator->load_rom(games + GAME);

	// #todo : Make the callback find out what emulator is running
	glfwSetKeyCallback(g_pWindow, on_set_key_callback);
	glfwSetDropCallback(g_pWindow, on_file_drop_callback);

	/* Init NK GUI */
	nk_context* nk_ctx = nk_glfw3_init(g_pWindow, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
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
	while (!glfwWindowShouldClose(g_pWindow))
	{
		/* deltaTime calculation */
		float prevTime = currTime;
		currTime = (float)glfwGetTime();
		float dt = currTime - prevTime;

		glfwPollEvents();
		nk_glfw3_new_frame();
		Logger::NewFrame();

		/* Emulator updating*/
		s_current_emulator->update(dt);

		/* NK gui updating */
		s_current_emulator->on_gui(nk_ctx);
		glClear(GL_COLOR_BUFFER_BIT);

		Draw(*s_current_emulator);
		
		// Don't forget state reset
		nk_glfw3_render(NK_ANTI_ALIASING_ON);
		glfwSwapBuffers(g_pWindow);
	}

	/* Cleanup font atlas*/
	s_current_emulator->deinit();

	nk_glfw3_shutdown();
	glfwTerminate();
}

