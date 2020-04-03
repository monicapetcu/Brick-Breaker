#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>


class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	struct LogicSpace
	{
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};

	struct brick_coords {
		int posX;
		int posY;
		GLboolean hit;
		int power_up;
	};


	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void Reset();
	void Collision(int ballX, int ballY, brick_coords &brick, float &speedX, float &speedY, int &bonus);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	LogicSpace logicSpace;

	glm::mat3 modelMatrix, modelMatrix2;
	float translateX, translateY;
	float scale[122];
	float scaleX, scaleY;
	float angularStep;
	int boardX, boardY;
	int ballX, ballY;
	int speed;
	float speedX, speedY;
	int rows, columns;
	int x = 0, y = 0;
	brick_coords bricks_array[11][13], power_up[11][13];
	int click;
	int lose;
	std::string brick_str;
	int brick_no;
	int reset;
	int center;
	int boardSize;
	int bonusX, bonusY;
	int bonus;
	int bonus_life1, bonus_life2, bonus_life3;
	int sticky;

};