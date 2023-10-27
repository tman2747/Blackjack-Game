#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937
#include "Card.h" // might not be needed in main
#include "Deck.h" // might not be needed in main
#include "Player.h"

//OPENGL /UI INCLUDES

#include <stdio.h>
#include "glad.h"
#include "GLFW/glfw3.h"

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Logging.h"

Logging Log;

void playBlackjack(Deck& deck, Player& player, Player& dealer)
{
		dealer.drawCard(deck);

		// The dealer's card is face up, the player can see it.
		Log.debugLog(Logging::info, "Dealer is showing: " + std::to_string(dealer.score()));

		// Create the player and give them 2 cards.
		player.drawCard(deck);
		player.drawCard(deck);
		Log.debugLog(Logging::info, "you have: " + std::to_string(player.score()));
		//std::cout << "You have: " << player.score() << '\n';

}


bool playagain()
{
	std::cout << "would you like to play again ('y' for yes anything else for no): ";
	char choice{};
	std::cin >> choice;


	if (choice == 'y' || choice == 'Y')
	{
		system("cls");
		return true;
	}

	return false;
}


// opengl functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	std::cout << "Width: " << width << " height: " << height << std::endl;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
void Buttondebug()
{
	std::cout << "Button Pressed" << std::endl;
}

int main()
{
	// opengl glfw glew
	if (!glfwInit())
	{
		Log.debugLog(Logging::error, "GLFW failed to Init");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Window hint setting main window to be non resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1329, 688, "Blackjack", NULL, NULL);
	if (window == NULL)
	{
		Log.debugLog(Logging::error, "Window was NULL");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log.debugLog(Logging::error, "Failed to Inizialize Glad");
		glfwTerminate();
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);


	// Iamgui / blackjack state
	Deck deck{};
	deck.shuffle();
	Player player(Log);
	Player dealer(Log);
	playBlackjack(deck, player, dealer);
	bool gameover = false;

	while (!glfwWindowShouldClose(window))
	{
		//CHECK for input
		processInput(window);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		bool showdemo = true;
		if (true)
			ImGui::ShowDemoWindow(&showdemo);

		bool no_titlebar = false;
		{
			// BlackJackWindow
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
			//windowFlags += ImGuiWindowFlags_NoMove;
			windowFlags += ImGuiWindowFlags_NoResize;

			const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 0, main_viewport->WorkPos.y + 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(1337, 695), ImGuiCond_FirstUseEver);

			ImGui::Begin("BlackJack",NULL,windowFlags);
			ImGui::SetCursorPos(ImVec2(644, 28));
			ImGui::Text("Blackjack V2");
			ImGui::SetCursorPos(ImVec2(636, 60));
			ImGui::Text("Dealer Score: %i", dealer.score());
			ImGui::SetCursorPos(ImVec2(636, 580));
			ImGui::Text("Player Score: %i", player.score());
			ImGui::SetCursorPos(ImVec2(646, 614));
			if (ImGui::Button("Hit") && !player.isBust() && !gameover)
			{
				player.drawCard(deck);
				std::cout << "You have: " << player.score() << '\n';

				if (player.isBust())
				{
					//player bust
					std::cout << "you bust" << std::endl;
					gameover = true;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Stand") && !gameover)
			{
				// if dealer busts
				dealer.dealerWantsHit(deck);
				gameover = true;
			}
			if (gameover)
			{
				if ((player.score() > dealer.score() || dealer.score() > 21) && player.score() <= 21)
				{
					ImGui::SetCursorPos(ImVec2(660, 300));
					ImGui::Text("You win!");
				}
				else
				{
					ImGui::SetCursorPos(ImVec2(660, 300));
					ImGui::Text("You Lose!");
				}
				ImGui::SetCursorPos(ImVec2(650, 330));
				if (ImGui::Button("Play again?"))
				{
					gameover = false;
					player.reset(deck, dealer);
					playBlackjack(deck, player, dealer);
				}
			}
			ImGui::End();
		}

		//rendering
		//background
		glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);




		//swap buffer and poll for events
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);

	}

	glfwTerminate();



	return 0;
}

