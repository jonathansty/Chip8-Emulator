#pragma once
/* Interface classes that require implementation */
class IDrawable
{
public:
	virtual void onDraw(GLFWwindow* pWindow) const = 0;
};

class BaseEmulator : public IDrawable
{
public:
	virtual void loadGame(const std::string& romName) = 0;
	virtual void initialize() = 0;
	virtual void emulateCycle(float dt) = 0;
	virtual void saveState(const std::string& saveLocation) = 0;
	// C style - Nuklear
	virtual void onGui(struct nk_context* ctx) = 0;
	virtual void shutdown() = 0;
	// TODO: Dispatch callbacks to right emulator
	// static void setKeys(GLFWwindow* window, int key, int scannode, int action, int mods);
		
};

/* DisplayData containing buffer data etc */
struct DisplayData
{
	GLuint shaderProgram;
	GLuint fragmentShader;
	GLuint vertexShader;
	GLuint vbo;
	GLuint vao;
	GLuint ebo;
	GLuint texture;
};
inline void Free(const DisplayData& d)
{
	glDeleteProgram(d.shaderProgram);
	glDeleteShader(d.fragmentShader);
	glDeleteShader(d.vertexShader);
	glDeleteBuffers(1, &d.vbo);
	glDeleteBuffers(1, &d.vao);
}
