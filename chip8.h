#pragma once
#include "Abstracts.h"
class Chip8 : public IEmulator
{
public:
	Chip8();

	// BEGIN IEmulator
	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void LoadGame(const std::string& romName) override;
	virtual void SaveState(const std::string& saveLocation) override;
	virtual void EmulateCycle(float dt) override;
	virtual void OnGUI(struct nk_context* ctx) override;
	// END IEmulator

	// BEGIN IDrawable
	virtual void OnDraw(GLFWwindow* pWindow)const override;
	// END IDrawable

	static  void SetKeys(GLFWwindow* window, int key, int scannode, int action, int mods);

	unsigned char const* GetGraphics() const
	{
		return m_Gfx;
	}
	unsigned char* GetRegisters()
	{
		return m_V;
	}
private:
	void CreateDisplay();

	void UpdateCounters();
	
	void ExecuteOpCode(unsigned short opCode);


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
		ShowRegisters = 0x01,
		ShowSettings = 0x01 << 1,
		ShowStacks = 0x01 << 2,
		ShowMemory = 0x01 << 3
	};
	int m_ShowFlags = (ShowRegisters | ShowSettings | ShowStacks | ShowMemory);

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
	unsigned short m_Sp;

	static bool m_Keys[16];

	int m_DelayTimer = 0;
	int m_SoundTimer = 0;

	static std::map<char, char> m_Keymap;
};
