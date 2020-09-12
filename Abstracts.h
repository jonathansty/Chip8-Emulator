#pragma once
/* Interface classes that require implementation */
class IDrawable
{
public:
	virtual void on_draw(GLFWwindow* pWindow) const = 0;
};

class IEmulator : public IDrawable
{
public:
	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual void load_rom(const std::string& RomName) = 0;
	virtual void update(float dt) = 0;
	virtual void save_state(const std::string& SaveLocation) = 0;

	virtual void set_keys(GLFWwindow* window, int key, int scannode, int action, int mods) {};

	// C style - Nuklear
	virtual void on_gui(struct nk_context* ctx) = 0;
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
