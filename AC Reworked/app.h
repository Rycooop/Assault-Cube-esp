#ifndef AC_CHEAT_H
#define AC_CHEAT_H

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <map>

#include "renderer.h"
#include "window.h"
#include "game.h"


class App {
public:
	App() = default;
	~App() = default;

	bool Initialize(const HINSTANCE hInstance);
	void Run();
	void Shutdown();


private:
	Renderer* m_Renderer;
	WindowManager* m_Window;
	Game* m_Game;

	std::map<std::string, std::thread> Threads;

};

#endif //AC_CHEAT_H