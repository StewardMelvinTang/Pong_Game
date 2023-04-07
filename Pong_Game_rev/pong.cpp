#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>

//bool State
bool showMainmenu = true;
bool isApplicationRunning = true;
bool isCreditMenuOpened = false;
float creditBGOpacity = 0.0f;
bool trigger_PlayGame = false;
bool trigger_TimerReseted = false;
bool stopwatchPaused = false;
float ballSpeedMultiplier = 1.0f;

//initialize color
Vector4 lightBlue = { 0.2f, 0.7f, 0.8f, 1.0f };
Vector4 lightRed = { 0.83f, 0.3f, 0.37f, 1.0f };
Vector4 buttonNormal = { 0.62f, 0.83f, 0.9f, 1.0f };
Vector4 buttonHovered = { 0.89f, 0.54f, 0.54f, 1.0f };
Vector4 darkRed = { 0.6f, 0.11f, 0.2f, 1.0f };
Color circleColor = { 255, 255, 255, 70 };
Color BG01 = ColorFromNormalized(lightBlue);
Color BG02 = ColorFromNormalized(lightRed);
const char* currentDir = GetWorkingDirectory(); //initialize current working directory
Vector4 creditBGColor;

//initialize timer delay
float delayTimer = 4.0f;
float timerCountdown_fontSize = 125;

//init the ball variables & draw the balls onto player's screen
struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;
	bool canMove = false;

	void Draw()
	{
		DrawCircle((int)x, (int)y, radius, (ballSpeedMultiplier >= 1.5f ? (x < GetScreenWidth() / 2 ? BLUE : ColorFromNormalized(darkRed)) : WHITE));
	}
};

//init the paddle variables & draw paddles onto player's screen
struct Paddle
{
	float x, y;
	float speed;
	float width, height;
	int score;
	bool scalingUp, scalingDown;
	float textScale = 1.0f;
	float textScaleSpeed = 0.075f;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, width, height };
	}

	void Draw()
	{
		//DrawRectangleRec(GetRect(), WHITE);
		DrawRectangleRounded(GetRect(), 15, 10, WHITE);
	}
};
//button structures
struct Button {
	Vector2 position,
			size;
	Color color, hoveredcolor;
	const char* text;
	float amplitude = 10.0f,
			period = 2.0f;
	bool isHovered;
	std::string identifier = "";
};




//draw credit UI 
void drawCreditMenu(Vector4 BGcolor, Button* buttons, int buttonslength) {
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromNormalized(BGcolor));

	for (int i = 0; i < buttonslength; i++) {
		if (buttons[i].identifier == "credit") {
			Rectangle btn = { buttons[i].position.x, buttons[i].position.y, buttons[i].size.x, buttons[i].size.y };
			DrawRectangleRounded(btn, 0.1f, 2, (!buttons[i].isHovered ? buttons[i].color : buttons[i].hoveredcolor));
			DrawRectangleRoundedLines(btn, 0.1f, 2, 5, BLACK);
			DrawText(buttons[i].text, buttons[i].position.x + buttons[i].size.x / 2 - MeasureText(buttons[i].text, 20) / 2, buttons[i].position.y + buttons[i].size.y / 2  - 10, 20, BLACK);

			//Check for hovered
			if (CheckCollisionPointRec(GetMousePosition(), { buttons[i].position.x, buttons[i].position.y, buttons[i].size.x, buttons[i].size.y })) {
				buttons[i].isHovered = true;

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					std::cout << "Clicked " << i << std::endl;
					if (i == 4) {
						//Return to mainmenu
						isCreditMenuOpened = false;
					}

					if (i == 5) {
						OpenURL("https://discord.gg/YD58sSuT");
					}
					if (i == 6) {
						OpenURL("https://github.com/StewardMelvinTang/Pong_Game_rev1");
					}
				}
			}
			else {
				buttons[i].isHovered = false;
			}
		}
	}

	DrawText("Developer", GetScreenWidth() / 2 - MeasureText("Developer", 50) / 2, 50, 50, BLACK);
	DrawText("Steward Melvin Tang", GetScreenWidth() / 2 - MeasureText("Steward Melvin Tang", 30) / 2, 100, 30, DARKGRAY);\
	DrawText("=======================", GetScreenWidth() / 2 - MeasureText("=======================", 20) / 2, 150, 20, DARKGRAY);
	DrawText("Created for education purposes, powered by Raylib Game Library", GetScreenWidth() / 2 - MeasureText("Created for education purposes, powered by Raylib Game Library.", 20) / 2, 175, 20, DARKGRAY);
	DrawText("Made purely using C++", GetScreenWidth() / 2 - MeasureText("Made purely using C++", 20) / 2, 200, 20, DARKGRAY);

}

//draw mainmenu UI
void drawMainMenu(Button* buttons, int buttonslength , double time) {
	for (int i = 0; i < buttonslength; i++) {
		if (buttons[i].identifier == "mainmenu") {
			Rectangle btn = { buttons[i].position.x, buttons[i].position.y + buttons[i].amplitude * sin(buttons[i].period * time), buttons[i].size.x, buttons[i].size.y };
			DrawRectangleRounded(btn, 0.1f, 2, (!buttons[i].isHovered ? buttons[i].color : buttons[i].hoveredcolor));
			DrawRectangleRoundedLines(btn, 0.1f, 2, 5, BLACK);
			DrawText(buttons[i].text, buttons[i].position.x + buttons[i].size.x / 2 - MeasureText(buttons[i].text, 20) / 2, buttons[i].position.y + buttons[i].size.y / 2 + buttons[i].amplitude * sin(buttons[i].period * time) / 2 - 10, 20, BLACK);

			//Check for hovered
			if (CheckCollisionPointRec(GetMousePosition(), { buttons[i].position.x, buttons[i].position.y, buttons[i].size.x, buttons[i].size.y })) {
				buttons[i].isHovered = true;

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					std::cout << "Clicked " << i << std::endl;
					if (i == 0) {
						//Normal
						showMainmenu = false;
						delayTimer = 4.0f;
						trigger_PlayGame = true;
						ballSpeedMultiplier = 1.0f;
					}
					if (i == 1) {
						//Hard
						showMainmenu = false;
						delayTimer = 4.0f;
						trigger_PlayGame = true;
						ballSpeedMultiplier = 1.5f;
						
					}
					if (i == 2) {
						//Credits
						isCreditMenuOpened = true;
						creditBGOpacity = 0.0f;
					}
					if (i == 3) {
						isApplicationRunning = false;
					}
				}
			}
			else {
				buttons[i].isHovered = false;
			}
		}
	}
	
}

//determine the ball to randomize target direction upon start (random bool)
float randomballstarting() {
	float ballstartingmultiplier;
	bool randomedlocation = rand() % 2 == 0;
	if (randomedlocation) {
		ballstartingmultiplier = 1.0f;
	}
	else {
		ballstartingmultiplier = -1.0f;
	}
	return ballstartingmultiplier;
}

//safe increase the ball speed (with limit)
float increaseBallSpeed(Ball ball, float speedmultiplier) {
	if (ball.speedX < 900 * ballSpeedMultiplier && ball.speedX > -900 * ballSpeedMultiplier) {
		return ball.speedX *= speedmultiplier;
	}
	else
	{
		return ball.speedX * -1;
	}
}



int main()
{
	//init game's window
	InitWindow(1300, 800, "Pong");
	SetWindowState(FLAG_VSYNC_HINT); //set VSync to lock FPS
	InitAudioDevice(); //initialize the audio device to play sounds

	//mainmenu button init
	double BTNtime = 0.0;
	Button buttons[7];
	buttons[0].identifier = "mainmenu";
	buttons[0].position = { float(GetScreenWidth()) / 2 - 100, float(GetScreenHeight())/ 2 };
	buttons[0].size = { 200, 60 };
	buttons[0].text = "Normal";
	buttons[0].color = ColorFromNormalized(buttonNormal);
	buttons[0].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[0].amplitude = 5.0f;
	buttons[0].period = 2.0f;

	buttons[1].identifier = "mainmenu";
	buttons[1].position = { float(GetScreenWidth()) / 2 - 100, float(GetScreenHeight()) / 2 + 80 };
	buttons[1].size = { 200, 60 };
	buttons[1].text = "Hard";
	buttons[1].color = ColorFromNormalized(buttonNormal);
	buttons[1].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[1].amplitude = 5.0f;
	buttons[1].period = 2.0f;

	buttons[2].identifier = "mainmenu";
	buttons[2].position = { float(GetScreenWidth()) / 2 - 100, float(GetScreenHeight()) / 2 + 160 };
	buttons[2].size = { 200, 60 };
	buttons[2].text = "Credits";
	buttons[2].color = ColorFromNormalized(buttonNormal);
	buttons[2].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[2].amplitude = 5.0f;
	buttons[2].period = 2.0f;

	buttons[3].identifier = "mainmenu";
	buttons[3].position = { float(GetScreenWidth()) / 2 - 100, float(GetScreenHeight()) / 2 + 240 };
	buttons[3].size = { 200, 60 };
	buttons[3].text = "Quit Game";
	buttons[3].color = ColorFromNormalized(buttonHovered);
	buttons[3].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[3].amplitude = 5.0f;
	buttons[3].period = 2.0f;

	buttons[4].identifier = "credit";
	buttons[4].position = { 25, 25 };
	buttons[4].size = { 60, 60 };
	buttons[4].text = "Back";
	buttons[4].color = ColorFromNormalized(buttonNormal);
	buttons[4].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[4].amplitude = 0.0f;
	buttons[4].period = 0.0f;

	buttons[5].identifier = "credit";
	buttons[5].position = { float(GetScreenWidth()) / 2 - 200, float(GetScreenHeight()) - 100 };
	buttons[5].size = { 200, 60 };
	buttons[5].text = "Join Discord";
	buttons[5].color = ColorFromNormalized(buttonNormal);
	buttons[5].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[5].amplitude = 5.0f;
	buttons[5].period = 2.0f;

	buttons[6].identifier = "credit";
	buttons[6].position = { float(GetScreenWidth()) / 2 + 25, float(GetScreenHeight()) - 100 };
	buttons[6].size = { 200, 60 };
	buttons[6].text = "Github";
	buttons[6].color = ColorFromNormalized(buttonNormal);
	buttons[6].hoveredcolor = ColorFromNormalized(buttonHovered);
	buttons[6].amplitude = 5.0f;
	buttons[6].period = 2.0f;

	//init CTime for stopwatch
	int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
	clock_t start_time, current_time;
	bool stopwatch_active = true;
	start_time = clock();
	int elapsed_time = 0;
	std::string time_str = "00:00";

	//init ball's stats
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = (ballSpeedMultiplier >= 1.5f ? 7 : 10);
	ball.speedX = 240 * randomballstarting() * ballSpeedMultiplier;
	ball.speedY = 240 * randomballstarting() * ballSpeedMultiplier;
	float ballMultiplierX = randomballstarting();
	float ballMultiplierY = randomballstarting();

	//init left paddle's stats
	Paddle leftPaddle;
	leftPaddle.x = 65;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 13;
	leftPaddle.height = 160;
	leftPaddle.speed = 600;
	leftPaddle.score = 0;
	leftPaddle.scalingUp = false;
	leftPaddle.scalingDown = false;

	//init right paddle's stats
	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 65;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 13;
	rightPaddle.height = 160;
	rightPaddle.speed = 600;
	rightPaddle.score = 0;
	rightPaddle.scalingUp = false;
	rightPaddle.scalingDown = false;

	//text
	const char* winnerText = nullptr;
	const char* spacetoRestart = "Press space to restart";

	//play looping music
	Sound goal_sound = LoadSound(TextFormat("%s/resources/goal.wav", currentDir));
	Sound bounce_sound = LoadSound(TextFormat("%s/resources/bounce.wav", currentDir));
	Sound hit_paddle_sound = LoadSound(TextFormat("%s/resources/hitpaddle.wav", currentDir));
	Sound game_ended_sound = LoadSound(TextFormat("%s/resources/gameended.wav", currentDir));
	Sound countdown_sound = LoadSound(TextFormat("%s/resources/countdown.wav", currentDir));
	Sound music;
	Sound mainmenuMusic = LoadSound(TextFormat("%s/resources/music_mainmenu.wav", currentDir));

	//initialize game logo
	Texture2D logoTexture = LoadTexture("resources/ponggame_icon.png");
	int logoWidth = 350; //logo size X
	int logoHeight = 340; //logo size Y
	//Calculate the source rectangle (the entire texture)
	Rectangle sourceRec = { 0.0f, 0.0f, (float)logoTexture.width, (float)logoTexture.height };
	


	//randomize music index to play
	srand(time(NULL)); //init random seed
	int randomizedmusic = rand() % 4 + 1;
	std::cout << "Music : " << randomizedmusic << std::endl;
	switch (randomizedmusic) {
		case 1: 
			music = LoadSound(TextFormat("%s/resources/music01.wav", currentDir));
			break;
		case 2: 
			music = LoadSound(TextFormat("%s/resources/music02.wav", currentDir));
			break;
		case 3: 
			music = LoadSound(TextFormat("%s/resources/music03.wav", currentDir));
			break;
		case 4:
			music = LoadSound(TextFormat("%s/resources/music04.wav", currentDir));
			break;
	}
	

	SetSoundVolume(bounce_sound, 0.4f); //set bounce sound to 50%
	SetSoundVolume(hit_paddle_sound, 0.4f);
	SetSoundVolume(goal_sound, 0.4f);
	SetSoundVolume(game_ended_sound, 0.4f);
	SetSoundVolume(music, 0.35f); //set volume to 35%
	SetSoundVolume(mainmenuMusic, 0.35f); //set mainmenu volume to 35%
	PlaySound(mainmenuMusic);

	while (!WindowShouldClose() && isApplicationRunning)
	{
		BeginDrawing();
		//draw backgrounds assets
		DrawRectangle(0, 0, GetScreenWidth() / 2, GetScreenHeight(), BG01);
		DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), BG02);
		DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 125, circleColor);
		DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 5, LIGHTGRAY);
		ClearBackground(BLACK); //clear the background to display a solid background color

		//call draw functions
		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();

		BTNtime += GetFrameTime();

		if (ball.canMove) {
			ball.x += ball.speedX * GetFrameTime();
			ball.y += ball.speedY * GetFrameTime();
		}


		//if the ball touched a corner of the screen, bounce it
		if (ball.y < 0 && !winnerText)
		{
			ball.y = 0;
			ball.speedY *= -1;
			PlaySound(bounce_sound);

		}
		if (ball.y > GetScreenHeight() && !winnerText)
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
			PlaySound(bounce_sound);

		}

		// paddle movement (W & S for Left paddle, Up & Down for Right Paddle)
		if (IsKeyDown(KEY_W) && !winnerText && !showMainmenu)
		{
			if (leftPaddle.y > 0 + leftPaddle.height / 2) {
				leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_S) && !winnerText && !showMainmenu)
		{
			if (leftPaddle.y < GetScreenHeight() - leftPaddle.height / 2) {
				leftPaddle.y += leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_UP) && !winnerText && !showMainmenu)
		{
			if (rightPaddle.y > 0 + rightPaddle.height / 2) {
				rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_DOWN) && !winnerText && !showMainmenu)
		{
			if (rightPaddle.y < GetScreenHeight() - rightPaddle.height / 2) {
				rightPaddle.y += rightPaddle.speed * GetFrameTime();
			}
		}

		//if the ball touched the paddles
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0 )
			{
				ball.speedX = increaseBallSpeed(ball, -1.2f);
				std::cout << ball.speedX << std::endl;
				rightPaddle.speed *= 1.12f;
				leftPaddle.speed *= 1.12f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
				PlaySound(hit_paddle_sound);
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX = increaseBallSpeed(ball, -1.2f);
				std::cout << ball.speedX << std::endl;
				rightPaddle.speed *= 1.12f;
				leftPaddle.speed *= 1.12f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
				PlaySound(hit_paddle_sound);
			}
		}

		//if the ball reached the left or right screen (win)
		if (ball.x < 0 && !winnerText)
		{
			rightPaddle.score++;
			if (rightPaddle.score >= (ballSpeedMultiplier == 1.5f ? 10 : 5)) {
				winnerText = "Red Player Wins!";
				spacetoRestart = "Press space to restart";
				PlaySound(game_ended_sound);
			}
			else {
				rightPaddle.scalingUp = true;
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 240 * randomballstarting() * ballSpeedMultiplier;
				ball.speedY = 240 * randomballstarting() * ballSpeedMultiplier;
				rightPaddle.speed = 600;
				leftPaddle.speed = 600;
				winnerText = nullptr;
				spacetoRestart = "";
				PlaySound(goal_sound);
				ball.canMove = false;
				delayTimer = 4.0f;
			}
			
		}
		if (ball.x > GetScreenWidth() && !winnerText)
		{
			leftPaddle.score++;
			if (leftPaddle.score >= (ballSpeedMultiplier == 1.5f ? 10 : 5)) {
				winnerText = "Blue Player Wins!";
				spacetoRestart = "Press space to restart";
				PlaySound(game_ended_sound);
			}
			else {
				leftPaddle.scalingUp = true;
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 240 * randomballstarting() * ballSpeedMultiplier;
				ball.speedY = 240 * randomballstarting() * ballSpeedMultiplier;
				rightPaddle.speed = 600;
				leftPaddle.speed = 600;
				winnerText = nullptr;
				spacetoRestart = "";
				ballMultiplierX = randomballstarting();
				ballMultiplierY = randomballstarting();
				PlaySound(goal_sound);
				ball.canMove = false;
				delayTimer = 4.0f;
			}
		}

		//restart game
		if (winnerText && IsKeyPressed(KEY_SPACE) && !showMainmenu)
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 240 * randomballstarting() * ballSpeedMultiplier;
			ball.speedY = 240 * randomballstarting() * ballSpeedMultiplier;
			winnerText = nullptr;
			spacetoRestart = "";
			leftPaddle.score = 0;
			rightPaddle.speed = 600;
			leftPaddle.speed = 600;
			rightPaddle.score = 0;
			ballMultiplierX = randomballstarting();
			ballMultiplierY = randomballstarting();
			start_time = clock();
			current_time = 0;
			PlaySound(bounce_sound);
			ball.canMove = false;
			delayTimer = 4.0f;
			trigger_TimerReseted = false;
			leftPaddle.y = GetScreenHeight() / 2;
			rightPaddle.y = GetScreenHeight() / 2;
			
		}

		if (winnerText && IsKeyPressed(KEY_H) && !showMainmenu) {
			showMainmenu = true;
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 240 * randomballstarting() * ballSpeedMultiplier;
			ball.speedY = 240 * randomballstarting() * ballSpeedMultiplier;
			winnerText = nullptr;
			spacetoRestart = "";
			leftPaddle.score = 0;
			rightPaddle.speed = 600;
			leftPaddle.speed = 600;
			rightPaddle.score = 0;
			ballMultiplierX = randomballstarting();
			ballMultiplierY = randomballstarting();
			start_time = clock();
			ball.canMove = false;
			delayTimer = 4.0f;
			current_time = 0;
			trigger_TimerReseted = false;
			leftPaddle.y = GetScreenHeight() / 2;
			rightPaddle.y = GetScreenHeight() / 2;
			if (IsSoundPlaying(music)) {
				UnloadSound(music);
				PlaySound(mainmenuMusic);
			}
		}

		//stopwatch system and elapsed 
		if (stopwatch_active && !showMainmenu && !winnerText && ball.canMove) {
			current_time = clock();
			elapsed_time = current_time - start_time;
			hours = elapsed_time / (60 * 60 * CLOCKS_PER_SEC);
			elapsed_time -= hours * 60 * 60 * CLOCKS_PER_SEC;
			minutes = elapsed_time / (60 * CLOCKS_PER_SEC);
			elapsed_time -= minutes * 60 * CLOCKS_PER_SEC;
			seconds = elapsed_time / CLOCKS_PER_SEC;
			elapsed_time -= seconds * CLOCKS_PER_SEC;
			milliseconds = elapsed_time * 1000 / CLOCKS_PER_SEC;

			if (!timerCountdown_fontSize < 25) {
				//draw the stopwatch to the screen with MM:SS format
				time_str = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
				DrawText(time_str.c_str(), GetScreenWidth() / 2 - (MeasureText(time_str.c_str(), 45)) / 2, 5, 45, WHITE);
			}
		}



		//scores animation text
		if (leftPaddle.scalingUp) {
			leftPaddle.textScale += leftPaddle.textScaleSpeed;
			if (leftPaddle.textScale >= 2.0f) {
				leftPaddle.textScale = 2.0f;
				leftPaddle.scalingUp = false;
				leftPaddle.scalingDown = true;
			}
		}
		if (leftPaddle.scalingDown) {
			leftPaddle.textScale -= leftPaddle.textScaleSpeed;
			if (leftPaddle.textScale <= 1.0f) {
				leftPaddle.scalingDown = false;
				leftPaddle.textScale = 1.0f;
			}
		}
		if (rightPaddle.scalingUp) {
			rightPaddle.textScale += rightPaddle.textScaleSpeed;
			if (rightPaddle.textScale >= 2.0f) {
				rightPaddle.textScale = 2.0f;
				rightPaddle.scalingUp = false;
				rightPaddle.scalingDown = true;
			}
		}
		if (rightPaddle.scalingDown) {
			rightPaddle.textScale -= rightPaddle.textScaleSpeed;
			if (rightPaddle.textScale <= 1.0f) {
				rightPaddle.scalingDown = false;
				rightPaddle.textScale = 1.0f;
			}
		}
		
		//draw scores text
		if (!showMainmenu) {
			DrawText(TextFormat("Score: %i", leftPaddle.score), 15, 10, 35 * leftPaddle.textScale, WHITE);
			DrawText(TextFormat("Score: %i", rightPaddle.score), GetScreenWidth() - (35 + 135), 10, 35 * rightPaddle.textScale, WHITE);
		}

		if (delayTimer <= -1) {
			ball.canMove = true;

			if (!trigger_TimerReseted) {
				start_time = clock();
				current_time = 0;
				trigger_TimerReseted = true;
			}
			
		}
		else {
			delayTimer -= GetFrameTime();
			
			if (delayTimer <= 1.03f && delayTimer >= 1.0f || delayTimer <= 2.03f && delayTimer >= 2.0f || delayTimer <= 3.03f && delayTimer >= 3.0f) {
				timerCountdown_fontSize = 125;
				PlaySound(countdown_sound);
			}
			if (delayTimer <= 0.1f) {
				SetSoundPitch(countdown_sound, 5);
				PlaySound(countdown_sound);
				//PlaySound(music);
			}
			
			timerCountdown_fontSize -= 0.75f;
			if (!showMainmenu) {
				std::string timerCountdown_text = (delayTimer <= 0.1f ? "GO!" : TextFormat("%.0f", (ceil(delayTimer) == 4 ? 3.0f : ceil(delayTimer))));
				DrawTextEx(GetFontDefault(), timerCountdown_text.c_str(), Vector2{ (float)GetScreenWidth() / 2 - MeasureText(timerCountdown_text.c_str(), timerCountdown_fontSize) / 2, (float)GetScreenHeight() / 2 - timerCountdown_fontSize / 2 }, timerCountdown_fontSize, 2, ORANGE);
			}
		}

		// ======== MAIN MENU ==========
		if (showMainmenu) {
			delayTimer = 100.0f;
			ball.canMove = false;
			//Calculate the destination recangle (centered on the screen with the desired size)
			Rectangle destRec = { (float)GetScreenWidth() / 2 - logoWidth / 2 - 5.0f * sin(2.0f * BTNtime), 
				(float)GetScreenHeight() / 2 - logoHeight - 5.0f * sin(2.0f * BTNtime), 
				(float)logoWidth + 10.0f * sin(2.0f * BTNtime),
				(float)logoHeight + 10.0f * sin(2.0f * BTNtime) };
			DrawTexturePro(logoTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, WHITE);
			drawMainMenu(buttons, (int)sizeof(buttons) / sizeof(Button) ,BTNtime);

		}

		if (isCreditMenuOpened) {
			if (creditBGOpacity < 1.0f) {
				creditBGOpacity += 0.1f;
				creditBGColor = { 1.0f, 1.0f, 1.0f, creditBGOpacity};
				std::cout << TextFormat("Credit BG Opacity : ",creditBGOpacity) << std::endl;
			}
			drawCreditMenu(creditBGColor, buttons, sizeof(buttons) / sizeof(Button));
		}

		if (trigger_PlayGame) {
			WaitTime(0.75);
			delayTimer = 4.0f;
			ball.canMove = false;
			if (IsSoundPlaying(mainmenuMusic)) {
				UnloadSound(mainmenuMusic);
			}
			trigger_PlayGame = false;
		}

		//music looping system
		if (!IsSoundPlaying(music) && delayTimer <= 0) {
			PlaySound(music);
		}
		if (!IsSoundPlaying(mainmenuMusic) && delayTimer > 0 && showMainmenu) {
			PlaySound(mainmenuMusic);
		}

		//draw Hard Mode text
		if (ballSpeedMultiplier > 1.0f) {
			DrawText("HARD MODE", GetScreenWidth() / 2 - MeasureText("HARD MODE", 25) / 2, GetScreenHeight() - 35, 25, GRAY);
		}
		//check if winnertext is valid, then draw texts onto the screen
		if (winnerText)
		{
			std::cout << time_str << std::endl;
			int textWidth = MeasureText(winnerText, 60);
			int spacetoRestartwidth = MeasureText(spacetoRestart, 25);
			DrawText(TextFormat("Match Time : %s", time_str.c_str()), GetScreenWidth() / 2 - MeasureText(TextFormat("Match Time : %s", time_str.c_str()), 25) / 2, GetScreenHeight() / 2 - 50, 25, WHITE);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, WHITE);
			DrawText(spacetoRestart, GetScreenWidth() / 2 - spacetoRestartwidth / 2, GetScreenHeight() / 2 + 50, 25, WHITE);
			DrawText("Press H to Return to the MainMenu", GetScreenWidth() / 2 - MeasureText("Press H to Return to the MainMenu", 25) / 2, GetScreenHeight() / 2 + 80, 25, WHITE);
		}

		//DrawFPS(10, 10); Use this to show FPS Counter
		EndDrawing();
	}

	UnloadTexture(logoTexture);
	CloseAudioDevice();
	CloseWindow();

	return 0;
}