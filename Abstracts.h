#pragma once
/* Interface classes that require implementation */
class IDrawable
{
public:
	virtual void OnDraw(GLFWwindow* pWindow) const = 0;
};

class IEmulator : public IDrawable
{
public:
	virtual void LoadGame(const std::string& RomName) = 0;
	virtual void Initialize() = 0;
	virtual void EmulateCycle(float dt) = 0;
	virtual void SaveState(const std::string& SaveLocation) = 0;

	// C style - Nuklear
	virtual void OnGUI(struct nk_context* ctx) = 0;
	virtual void Shutdown() = 0;

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
