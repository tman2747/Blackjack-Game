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


void playBlackjack(Deck& deck, Player& player, Player& dealer)
{
		dealer.drawCard(deck);

		// The dealer's card is face up, the player can see it.
		std::cout << "The dealer is showing: " << dealer.score() << '\n';

		// Create the player and give them 2 cards.
		player.drawCard(deck);
		player.drawCard(deck);
		std::cout << "You have: " << player.score() << '\n';

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
		std::cout << "window was NULL :(" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize glad :(" << std::endl;
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
	Player player{};
	Player dealer{};
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
		//if (true)
		//	ImGui::ShowDemoWindow(&showdemo);


		{
			// BlackJackWindow
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
			windowFlags += ImGuiWindowFlags_NoMove;
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
			if (ImGui::Button("Stand"))
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

	// maybe switch this to a if ? : thingy (its The conditional operator (or Ternary operator) chapter5.5)


	return 0;
}


// *****Code from orignal blackjack before we put it into classes ****** 
//
//using Deck = std::array<Card, 52>;
//using Index = Deck::size_type;
//
//// Maximum score before losing.
//constexpr int g_maximumScore{ 21 };
//
//// Minimum score that the dealer has to have.
//constexpr int g_minimumDealerScore{ 17 };
//
//
//
//bool playerWantsHit()
//{
//    while (true)
//    {
//        std::cout << "(h) to hit, or (s) to stand: ";
//
//        char ch{};
//        std::cin >> ch;
//
//        switch (ch)
//        {
//        case 'h':
//            return true;
//        case 's':
//            return false;
//        }
//    }
//}
//
//// Returns true if the player went bust. False otherwise.
//bool playerTurn(const Deck& deck, Index& nextCardIndex, Player& player)
//{
//    while (true)
//    {
//        if (player.score > g_maximumScore)
//        {
//            // This can happen even before the player had a choice if they drew 2
//            // aces.
//            std::cout << "You busted!\n";
//            return true;
//        }
//        else
//        {
//            if (playerWantsHit())
//            {
//                int cardValue{ getCardValue(deck.at(nextCardIndex++)) };
//                player.score += cardValue;
//                std::cout << "You were dealt a " << cardValue << " and now have " << player.score << '\n';
//            }
//            else
//            {
//                // The player didn't go bust.
//                return false;
//            }
//        }
//    }
//}
//
//// Returns true if the dealer went bust. False otherwise.
//bool dealerTurn(const Deck& deck, Index& nextCardIndex, Player& dealer)
//{
//    // Draw cards until we reach the minimum value.
//    while (dealer.score < g_minimumDealerScore)
//    {
//        int cardValue{ getCardValue(deck.at(nextCardIndex++)) };
//        dealer.score += cardValue;
//        std::cout << "The dealer turned up a " << cardValue << " and now has " << dealer.score << '\n';
//
//    }
//
//    // If the dealer's score is too high, they went bust.
//    if (dealer.score > g_maximumScore)
//    {
//        std::cout << "The dealer busted!\n";
//        return true;
//    }
//
//    return false;
//}
//
//bool playBlackjack(const Deck& deck)
//{
//    // Index of the card that will be drawn next. This cannot overrun
//    // the array, because a player will lose before all cards are used up.
//    Index nextCardIndex{ 0 };
//
//    // Create the dealer and give them 1 card.
//    Player dealer{ getCardValue(deck.at(nextCardIndex++)) };
//
//    // The dealer's card is face up, the player can see it.
//    std::cout << "The dealer is showing: " << dealer.score << '\n';
//
//    // Create the player and give them 2 cards.
//    Player player{ getCardValue(deck.at(nextCardIndex)) + getCardValue(deck.at(nextCardIndex + 1)) };
//    nextCardIndex += 2;
//
//    std::cout << "You have: " << player.score << '\n';
//
//    if (playerTurn(deck, nextCardIndex, player))
//    {
//        // The player went bust.
//        return false;
//    }
//
//    if (dealerTurn(deck, nextCardIndex, dealer))
//    {
//        // The dealer went bust, the player wins.
//        return true;
//    }
//
//    return (player.score > dealer.score);
//}
//
//int main()
//{
//    auto deck{ createDeck() };
//
//    shuffleDeck(deck);
//
//    if (playBlackjack(deck))
//    {
//        std::cout << "You win!\n";
//    }
//    else
//    {
//        std::cout << "You lose!\n";
//    }
//
//    return 0;
//}