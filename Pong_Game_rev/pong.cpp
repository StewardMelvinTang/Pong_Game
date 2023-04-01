#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>


//init the ball variables & draw the balls onto player's screen
struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;
	bool canMove = false;

	void Draw()
	{
		DrawCircle((int)x, (int)y, radius, WHITE);
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

//initialize color
Vector4 lightBlue = { 0.2f, 0.7f, 0.8f, 1.0f };
Vector4 lightRed = {0.83f, 0.3f, 0.37f, 1.0f};
Color circleColor = { 255, 255, 255, 70 };
Color BG01 = ColorFromNormalized(lightBlue);
Color BG02 = ColorFromNormalized(lightRed);
const char* currentDir = GetWorkingDirectory(); //initialize current working directory

//initialize timer delay
float delayTimer = 2.0f;

int main()
{
	//init game's window
	InitWindow(1300, 800, "Pong");
	SetWindowState(FLAG_VSYNC_HINT); //set VSync to lock FPS
	InitAudioDevice(); //initialize the audio device to play sounds

	//init CTime for stopwatch
	int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
	clock_t start_time, current_time;
	bool stopwatch_active = true;
	start_time = clock();

	//init ball's stats
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 10;
	ball.speedX = 200 * randomballstarting();
	ball.speedY = 240 * randomballstarting();
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
	Sound music = LoadSound(TextFormat("%s/resources/music.wav", currentDir));
	Sound goal_sound = LoadSound(TextFormat("%s/resources/goal.wav", currentDir));
	Sound bounce_sound = LoadSound(TextFormat("%s/resources/bounce.wav", currentDir));
	Sound hit_paddle_sound = LoadSound(TextFormat("%s/resources/hitpaddle.wav", currentDir));
	Sound game_ended_sound = LoadSound(TextFormat("%s/resources/gameended.wav", currentDir));
	SetSoundVolume(bounce_sound, 0.4f); //set bounce sound to 50%
	SetSoundVolume(hit_paddle_sound, 0.4f);
	SetSoundVolume(goal_sound, 0.4f);
	SetSoundVolume(game_ended_sound, 0.4f);
	SetSoundVolume(music, 0.25f); //set volume to 25%
	PlaySound(music);
	
	
	while (!WindowShouldClose())
	{
		delayTimer -= GetFrameTime();
		if (delayTimer <= 0) {
			ball.canMove = true;
			current_time = 0;
		}

		if (ball.canMove) {
			ball.x += ball.speedX * GetFrameTime();
			ball.y += ball.speedY * GetFrameTime();
		}


		//if the ball touched a corner of the screen, bounce it
		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
			PlaySound(bounce_sound);

		}
		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
			PlaySound(bounce_sound);

		}

		// paddle movement (W & S for Left paddle, Up & Down for Right Paddle)
		if (IsKeyDown(KEY_W) && !winnerText)
		{
			if (leftPaddle.y > 0 + leftPaddle.height / 2) {
				leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_S) && !winnerText)
		{
			if (leftPaddle.y < GetScreenHeight() - leftPaddle.height / 2) {
				leftPaddle.y += leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_UP) && !winnerText)
		{
			if (rightPaddle.y > 0 + rightPaddle.height / 2) {
				rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_DOWN) && !winnerText)
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
				ball.speedX *= -1.1f;
				rightPaddle.speed *= 1.05f;
				leftPaddle.speed *= 1.05f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
				PlaySound(hit_paddle_sound);
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				rightPaddle.speed *= 1.05f;
				leftPaddle.speed *= 1.05f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
				PlaySound(hit_paddle_sound);
			}
		}

		//if the ball reached the left or right screen (win)
		if (ball.x < 0 && !winnerText)
		{
			rightPaddle.score++;
			if (rightPaddle.score >= 5) {
				winnerText = "Red Player Wins!";
				spacetoRestart = "Press space to restart";
				PlaySound(game_ended_sound);
			}
			else {
				rightPaddle.scalingUp = true;
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 200 * randomballstarting();
				ball.speedY = 240 * randomballstarting();
				rightPaddle.speed = 600;
				leftPaddle.speed = 600;
				winnerText = nullptr;
				spacetoRestart = "";
				PlaySound(goal_sound);
			}
			
		}
		if (ball.x > GetScreenWidth() && !winnerText)
		{
			leftPaddle.score++;
			if (leftPaddle.score >= 5) {
				winnerText = "Blue Player Wins!";
				spacetoRestart = "Press space to restart";
				PlaySound(game_ended_sound);
			}
			else {
				leftPaddle.scalingUp = true;
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 200 * randomballstarting();
				ball.speedY = 240 * randomballstarting();
				rightPaddle.speed = 600;
				leftPaddle.speed = 600;
				winnerText = nullptr;
				spacetoRestart = "";
				ballMultiplierX = randomballstarting();
				ballMultiplierY = randomballstarting();
				PlaySound(goal_sound);
			}
		}

		//restart game
		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 200 * randomballstarting();
			ball.speedY = 240 * randomballstarting();
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
		}

		//start the rendering proccess and draw backgrounds
		BeginDrawing();
		DrawRectangle(0, 0, GetScreenWidth() / 2, GetScreenHeight(), BG01);
		DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), BG02);
		DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 125, circleColor);
		DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 5, LIGHTGRAY);
		ClearBackground(BLACK); //clear the background to display a solid background color
		
		
		//call draw functions
		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();

		//check if winnertext is valid, then draw texts onto the screen
		if (winnerText)
		{
			int textWidth = MeasureText(winnerText, 60);
			int spacetoRestartwidth = MeasureText(spacetoRestart, 25);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, WHITE);
			DrawText(spacetoRestart, GetScreenWidth() / 2 - spacetoRestartwidth / 2 , GetScreenHeight() / 2 + 50, 25, WHITE);
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

		//music looping system
		if (!IsSoundPlaying(music)) {
			PlaySound(music);
		}

		//stopwatch system and elapsed time
		if (stopwatch_active && ball.canMove && !winnerText) {
			current_time = clock();
			int elapsed_time = current_time - start_time;
			hours = elapsed_time / (60 * 60 * CLOCKS_PER_SEC);
			elapsed_time -= hours * 60 * 60 * CLOCKS_PER_SEC;
			minutes = elapsed_time / (60 * CLOCKS_PER_SEC);
			elapsed_time -= minutes * 60 * CLOCKS_PER_SEC;
			seconds = elapsed_time / CLOCKS_PER_SEC;
			elapsed_time -= seconds * CLOCKS_PER_SEC;
			milliseconds = elapsed_time * 1000 / CLOCKS_PER_SEC;
			
			//draw the stopwatch to the screen with MM:SS format
			std::string time_str = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
			DrawText(time_str.c_str(), GetScreenWidth() / 2 - (MeasureText(time_str.c_str(), 45)) / 2, 5, 45, WHITE);
		}

		//draw scores
		DrawText(TextFormat("Score: %i", leftPaddle.score), 15, 10, 35 * leftPaddle.textScale, WHITE);
		DrawText(TextFormat("Score: %i", rightPaddle.score), GetScreenWidth() - (35 + 135), 10, 35 * rightPaddle.textScale, WHITE);


		//DrawFPS(10, 10); Use this to show FPS Counter
		EndDrawing();
	}
	CloseAudioDevice();
	CloseWindow();

	return 0;
}