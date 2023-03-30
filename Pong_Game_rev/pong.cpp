#include "raylib.h"
#include <cstdlib>
//init the ball variables & draw the balls onto player's screen
struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

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

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
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

int main()
{
	//init game's window
	InitWindow(800, 600, "Pong")
	SetWindowState(FLAG_VSYNC_HINT); //set VSync to lock FPS

	//init ball's stats
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 7;
	ball.speedX = 175;
	ball.speedY = 220;

	//init left paddle's stats
	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 15;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;
	leftPaddle.score = 0;

	//init right paddle's stats
	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 15;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;
	rightPaddle.score = 0;

	//text
	const char* winnerText = nullptr;
	const char* spacetoRestart = "Press space to restart";

	//randomize ball starting direction
	float ballMultiplierX = randomballstarting();
	float ballMultiplierY = randomballstarting();
	
	while (!WindowShouldClose())
	{

		ball.x += ball.speedX * GetFrameTime() * ballMultiplierX;
		ball.y += ball.speedY * GetFrameTime() * ballMultiplierY;

		//if the ball touched a corner of the screen
		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		// paddle movement (W & S for Left paddle, Up & Down for Right Paddle)
		if (IsKeyDown(KEY_W))
		{
			if (leftPaddle.y > 0 + leftPaddle.height / 2) {
				leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_S))
		{
			if (leftPaddle.y < GetScreenHeight() - leftPaddle.height / 2) {
				leftPaddle.y += leftPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_UP))
		{
			if (rightPaddle.y > 0 + rightPaddle.height / 2) {
				rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			}
			
		}
		if (IsKeyDown(KEY_DOWN))
		{
			if (rightPaddle.y < GetScreenHeight() - rightPaddle.height / 2) {
				rightPaddle.y += rightPaddle.speed * GetFrameTime();
			}
		}

		//if the ball touched the paddles
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (true)
			{
				ball.speedX *= -1.1f;
				rightPaddle.speed *= 1.05f;
				leftPaddle.speed *= 1.05f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (true)
			{
				ball.speedX *= -1.1f;
				rightPaddle.speed *= 1.05f;
				leftPaddle.speed *= 1.05f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		//if the ball reached the left or right screen (win)
		if (ball.x < 0 && !winnerText)
		{
			rightPaddle.score++;
			if (rightPaddle.score >= 3) {
				winnerText = "Right Player Wins!";
				spacetoRestart = "Press space to restart";
			}
			else {
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 175;
				ball.speedY = 225;
				rightPaddle.speed = 500;
				leftPaddle.speed = 500;
				winnerText = nullptr;
				spacetoRestart = "";
				ballMultiplierX = randomballstarting();
				ballMultiplierY = randomballstarting();
			}
			
		}
		if (ball.x > GetScreenWidth() && !winnerText)
		{
			leftPaddle.score++;
			if (leftPaddle.score >= 3) {
				winnerText = "Left Player Wins!";
				spacetoRestart = "Press space to restart";
			}
			else {
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 175;
				ball.speedY = 225;
				rightPaddle.speed = 500;
				leftPaddle.speed = 500;
				winnerText = nullptr;
				spacetoRestart = "";
				ballMultiplierX = randomballstarting();
				ballMultiplierY = randomballstarting();
			}
		}

		//restart game
		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 175;
			ball.speedY = 225;
			winnerText = nullptr;
			spacetoRestart = "";
			leftPaddle.score = 0;
			rightPaddle.speed = 500;
			leftPaddle.speed = 500;
			rightPaddle.score = 0;
			ballMultiplierX = randomballstarting();
			ballMultiplierY = randomballstarting();
		}

		//start the rendering proccess
		BeginDrawing();
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
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			DrawText(spacetoRestart, GetScreenWidth() / 2 - spacetoRestartwidth / 2 , GetScreenHeight() / 2 + 50, 25, ORANGE);
		}

		//draw scores
		DrawText(TextFormat("Score: %i", leftPaddle.score), 15, 10, 25, WHITE);
		DrawText(TextFormat("Score: %i", rightPaddle.score), GetScreenWidth() - (15 + 100), 10, 25, WHITE);


		//DrawFPS(10, 10); Use this to show FPS Counter
		EndDrawing();
	}

	CloseWindow();

	return 0;
}