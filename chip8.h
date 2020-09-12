#pragma once
#include "Abstracts.h"
class Chip8 : public IEmulator
{
public:
	Chip8();

	// BEGIN IEmulator
	virtual void init() override;
	virtual void deinit() override;
	virtual void load_rom(const std::string& romName) override;
	virtual void save_state(const std::string& saveLocation) override;
	virtual void update(float dt) override;
	virtual void on_gui(struct nk_context* ctx) override;
	// END IEmulator

	// BEGIN IDrawable
	virtual void on_draw(GLFWwindow* pWindow)const override;
	// END IDrawable

	void set_keys(GLFWwindow* window, int key, int scannode, int action, int mods);

	unsigned char const* get_gfx() const
	{
		return m_Gfx;
	}
	unsigned char* get_registers()
	{
		return m_V;
	}
private:
	void create_display();

	void update_cntrs();
	
	void execute_op_code(unsigned short opCode);


	/* Computer architecture */
	// Chip 8 has 35 operation codes which are 2 bytes long
	const float  TIMER_RATE = 1.0f / 60.0f;
	float MAX_EMULATE_RATE = 1.0f / 200.0f;
	double m_DPI = 1;

	float m_EmulateTimer = MAX_EMULATE_RATE;
	float m_UpdateTimer = TIMER_RATE;
	std::string m_CurrentGame = "PONG";
	mutable bool m_DrawFlag = false;

	/* Debug information drawing */
	enum ShowFlags : int
	{
		ShowFlags_ShowRegisters = 0x01,
		ShowFlags_ShowSettings = 0x01 << 1,
		ShowFlags_ShowStacks = 0x01 << 2,
		ShowFlags_ShowMemory = 0x01 << 3
	};
	int m_ShowFlags = (ShowFlags_ShowRegisters | ShowFlags_ShowSettings | ShowFlags_ShowStacks | ShowFlags_ShowMemory);

	bool m_LogOpCodes = false;

	DisplayData m_DisplayData;

	unsigned short m_Opcode;
	// Chip8 has 4K memory
	unsigned char m_Memory[4096];

	// 15 8-bit general purpose registers
	unsigned char m_V[16];

	// Index register and program counter
	unsigned short m_I;
	unsigned short m_Pc;
	// 0x000-0x1FF -> chip 8 interpreter
	// 0x050-0x0A0 -> built in 4x5 pixel font set
	// 0x200-0xFFF -> Program ROM and work RAM

	// Display is 64 x 32 
	unsigned char m_Gfx[64*32];
	unsigned short m_Stack[16];
	unsigned short m_Sp; // stack pointer

	static bool m_Keys[16];

	int m_DelayTimer = 0;
	int m_SoundTimer = 0;

	static std::map<char, char> m_Keymap;
};
