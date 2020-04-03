#include "Tema1.h"

#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include "Transform2D_Tema1.h"
#include "Object2D_Tema1.h"
#include <time.h>

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 1;

	// board
	boardX = 540;
	boardY = 20;

	// ball
	ballX = 640;
	ballY = 60;

	speedX = 100.0f; // viteza pe axa Ox a bilei
	speedY = 350.0f; // viteza pe axa Oy a bilei
	boardSize = 200; // lungimea platformei
	x = 170; y = 250; // pozitia in coordonatele (x, y) a primei caramizi dupa care ma ghidez
	lose = 3; // numarul de vieti

	click = 0;
	reset = 0;
	center = 1;
	bonus = 0;
	sticky = 0;

	// Peretele din stanga
	Mesh* left = Object2D_Tema1::CreateSquare("left", corner, squareSide,
		glm::vec3(0.9568627450980392, 0.4313725490196078, 0.2588235294117647), true);
	AddMeshToList(left);
	
	// Peretele din dreapta
	Mesh* right = Object2D_Tema1::CreateSquare("right", corner, squareSide, 
		glm::vec3(0.9568627450980392, 0.4313725490196078, 0.2588235294117647), true);
	AddMeshToList(right);

	// Peretele de sus
	Mesh* up = Object2D_Tema1::CreateSquare("up", corner, squareSide, 
		glm::vec3(0.9568627450980392, 0.4313725490196078, 0.2588235294117647), true);
	AddMeshToList(up);

	// Platforma
	Mesh* board = Object2D_Tema1::CreateSquare("board", corner, squareSide, 
		glm::vec3(0.2588235294117647, 0.9568627450980392, 0.5607843137254902), true);
	AddMeshToList(board);

	// Bila
	Mesh* ball = Object2D_Tema1::CreateCircle("ball", corner, 10, glm::vec3(1, 1, 1), true);
	AddMeshToList(ball);

	// Patrat primul power up - viata in plus
	Mesh* powerup1 = Object2D_Tema1::CreateSquare("powerup1", corner, squareSide / 10,
		glm::vec3(1, 1, 0), true);
	AddMeshToList(powerup1);

	// Patrat al doilea power up - platforma lipicioasa
	Mesh* powerup2 = Object2D_Tema1::CreateSquare("powerup2", corner, squareSide / 10,
		glm::vec3(1, 0, 0), true);
	AddMeshToList(powerup2);

	// Vietile
	Mesh* life1 = Object2D_Tema1::CreateCircle("life1", corner, 10, glm::vec3(1, 1, 1), true);
	AddMeshToList(life1);

	Mesh* life2 = Object2D_Tema1::CreateCircle("life2", corner, 10, glm::vec3(1, 1, 1), true);
	AddMeshToList(life2);

	Mesh* life3 = Object2D_Tema1::CreateCircle("life3", corner, 10, glm::vec3(1, 1, 1), true);
	AddMeshToList(life3);

	// Matricea de caramizi unde le colorez si retin coordonatele si initializez faptul ca nu au fost lovite.
	// Retin coordonatele caramizilor si intr-o matrice de power_up.
	// Folosesc doua culori in desenarea caramizilor: pe randurile pare mov, iar pe cele impare albastru.
	Mesh* brick[11][13];
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 12; j++)
		{
			brick_str = "brick_";
			brick_no = (i - 1) * 12 + j;
			scale[brick_no] = 1;
			
			if (i % 2) {
				brick[i][j] = Object2D_Tema1::CreateBrick(brick_str + to_string(brick_no), 
					corner, 60, 30, glm::vec3(0.66666666666666667, 0.2588235294117647, 0.9568627450980392), true);
			}
			else {
				brick[i][j] = Object2D_Tema1::CreateBrick(brick_str + to_string(brick_no),
					corner, 60, 30, glm::vec3(0.2588235294117647, 0.4313725490196078, 0.9568627450980392), true);
			}
			AddMeshToList(brick[i][j]);

			bricks_array[i][j].posX = x;
			bricks_array[i][j].posY = y;
			power_up[i][j].posX = x;
			power_up[i][j].posY = y;
			x += 80;
			bricks_array[i][j].hit = 0;
		}
		y += 40;
		x = 170;
	}
	
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// Afisez peretele din stanga
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D_Tema1::Translate(translateX, translateY + 80);
	scaleX = (float)0.2;
	scaleY = (float)6.5;
	modelMatrix *= Transform2D_Tema1::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["left"], shaders["VertexColor"], modelMatrix);

	// Afisez peretele de sus
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D_Tema1::Translate(translateX, translateY + 700);
	scaleX = (float)13;
	scaleY = (float)0.2;
	modelMatrix *= Transform2D_Tema1::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["up"], shaders["VertexColor"], modelMatrix);

	// Afisez peretele din dreapta
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D_Tema1::Translate(translateX + 1260, translateY + 80);
	scaleX = (float)0.2;
	scaleY = (float)6.5;
	modelMatrix *= Transform2D_Tema1::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["right"], shaders["VertexColor"], modelMatrix);

	// Afisez platforma
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D_Tema1::Translate(boardX, boardY);
	scaleX = (float)2;
	scaleY = (float)0.075;
	modelMatrix *= Transform2D_Tema1::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["board"], shaders["VertexColor"], modelMatrix);

	// Afisez bila
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D_Tema1::Translate(ballX, ballY);
	RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);

	// Cate vieti trebuie sa afisez pe ecran in functie de cate vieti am pierdut sau am acumulat la loc.
	// Maxim 3 vieti.
	if (lose >= 1) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(translateX + 30, translateY + 30);
		RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
	}
	if (lose >= 2) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(translateX + 60, translateY + 30);
		RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
	}
	if (lose == 3) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(translateX + 90, translateY + 30);
		RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
	}

	if (click == 1) {
		
		// Daca bila pica sub platforma, pierd o viata si imi reinitializeaza pozitia bilei si a platformei.
		if (ballY < boardY) {
			Reset();
			reset = 1;
		}

		// Testez daca bila se afla in mijlocul platformei
		if ((ballX == boardX + 100 && ballY <= boardY + 10 && ballY >= boardY) || reset == 1) {
			center = 1;
		}

		// Daca se afla, bila va pleca pe directie verticala. Altfel se va mari coordonata x a acesteia.
		if (center == 0) {
			ballX += speedX * deltaTimeSeconds;
		}

		ballY += speedY * deltaTimeSeconds;
		
		// Daca bila loveste peretele din stanga
		if (ballX - 10 <= 20) {
			speedX = -speedX;
			ballX = 30;
		}
		// Daca  bila loveste peretele dini dreapta
		else if (ballX + 10 >= 1260) {
			speedX = -speedX;
			ballX = 1250;
		}
		// Daca bila loveste peretele de sus
		if (ballY + 10 >= 700) {
			speedY = -speedY;
			ballY = 690;
		}

		// Coliziunea cu plpatforma
		if (ballY < boardY + 10 && (ballX >= boardX && ballX <= boardX + 200)) {

			if (ballX <= boardX + 100) {
				speedX = -(100.0f + ((float)(boardX + 100 - ballX) / 100));
				speedY = 350.0f + ((float)(boardX + 100 - ballX) / 100);
			}
			else if (ballX > boardX + 100) {
				speedX = 100.0f + ((float)(ballX - (boardX + 100)) / 100);
				speedY = 350.0f + ((float)(ballX - (boardX + 100)) / 100);
			}

			ballY = boardY + 10;
			reset = 0;
			center = 0;

			// Pentru power_up - platforma lipicioasa:
			if (sticky) {
				click = 0;
				sticky = 0;
				ballY = boardY + 20;
				reset = 1;
				center = 1;
			}
		}

		// Afisez caramizile
		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j <= 12; j++)
			{
				brick_str = "brick_";
				brick_no = (i - 1) * 12 + j; // al cata caramida -> pentru RenderMesh2D
				// Verific daca s-a produs coliziune
				Collision(ballX, ballY, bricks_array[i][j], speedX, speedY, bonus);
				// Primul power-up > one more chance - viata bonus
				if (bricks_array[i][j].hit == 1 && bricks_array[i][j].power_up % 15 == 0) {
						modelMatrix2 = glm::mat3(1);
						modelMatrix2 *= Transform2D_Tema1::Translate(power_up[i][j].posX + 25, power_up[i][j].posY);
						modelMatrix2 *= Transform2D_Tema1::Translate(5, 5);
						angularStep += deltaTimeSeconds;
						modelMatrix2 *= Transform2D_Tema1::Rotate(angularStep);
						modelMatrix2 *= Transform2D_Tema1::Translate(-5, -5);
						RenderMesh2D(meshes["powerup1"], shaders["VertexColor"], modelMatrix2);
						power_up[i][j].posY -= deltaTimeSeconds;

						if (power_up[i][j].posY == 0) {
							power_up[i][j].posY = -100;
						}
						if (power_up[i][j].posY <= boardY && power_up[i][j].posX >= boardX && power_up[i][j].posX <= boardX + 200) {
							power_up[i][j].posY = -100;
							power_up[i][j].posX = -100;

							if (lose < 3 && lose >= 0) {
								lose++;
							}
						}
				}
				// Al doilea power-up > so sticky - platforma lipicioasa
				else if (bricks_array[i][j].hit == 1 && bricks_array[i][j].power_up % 8 == 0) {
					modelMatrix2 = glm::mat3(1);
					modelMatrix2 *= Transform2D_Tema1::Translate(power_up[i][j].posX + 25, power_up[i][j].posY);
					modelMatrix2 *= Transform2D_Tema1::Translate(5, 5);
					angularStep += deltaTimeSeconds;
					modelMatrix2 *= Transform2D_Tema1::Rotate(angularStep);
					modelMatrix2 *= Transform2D_Tema1::Translate(-5, -5);
					RenderMesh2D(meshes["powerup2"], shaders["VertexColor"], modelMatrix2);
					power_up[i][j].posY -= deltaTimeSeconds;

					if (power_up[i][j].posY == 0) {
						power_up[i][j].posY = -100;
					}
					if (power_up[i][j].posY <= boardY && power_up[i][j].posX >= boardX && power_up[i][j].posX <= boardX + 200) {
						power_up[i][j].posY = -100;
						power_up[i][j].posX = -100;

						sticky = 1;
					}
				}
				// Verific daca a fost lovita caramida. In caz afirmativ o fac sa dispara.
				if (bricks_array[i][j].hit == 1) {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D_Tema1::Translate(bricks_array[i][j].posX, bricks_array[i][j].posY);
					modelMatrix *= Transform2D_Tema1::Translate(30, 15);
					scale[brick_no] -= 0.1;
					modelMatrix *= Transform2D_Tema1::Scale(scale[brick_no], scale[brick_no]);
					modelMatrix *= Transform2D_Tema1::Translate(-30, -15);
					RenderMesh2D(meshes[brick_str + to_string(brick_no)], shaders["VertexColor"], modelMatrix);

					if (scale[brick_no] <= 0) {
						meshes.erase(brick_str + to_string(brick_no));
						bricks_array[i][j].posX = -100;
						bricks_array[i][j].posY = -100;
					}
				}
				else {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D_Tema1::Translate(bricks_array[i][j].posX, bricks_array[i][j].posY);
					RenderMesh2D(meshes[brick_str + to_string(brick_no)], shaders["VertexColor"], modelMatrix);
				}
			}
		}
	}
	else {
		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j <= 12; j++)
			{
				brick_str = "brick_";
				brick_no = (i - 1) * 12 + j;
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D_Tema1::Translate(bricks_array[i][j].posX, bricks_array[i][j].posY);
				RenderMesh2D(meshes[brick_str + to_string(brick_no)], shaders["VertexColor"], modelMatrix);
			}
		}
	}

	// Verific daca au fost sparte toate caramizile pentru a reseta jocul, pentru a o lua de la capat.
	int win = 0;

	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 12; j++)
		{
			if (bricks_array[i][j].posX != -100) {
				win = 1;
			}
		}
	}
	if (win == 0) {
		Tema1::Init();
	}

}

void Tema1::Reset()
{
	click = 0;
	ballX = 640;
	ballY = 60;
	boardX = 540;
	boardY = 20;
	speedX = 100.0f;
	speedY = 350.0f;

	if (lose == 3) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(-100, -100);
		RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
	}
	else if (lose == 2) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(-100, -100);
		RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
	}
	else if (lose == 1) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D_Tema1::Translate(-100, -100);
		RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
	}

	lose--;

	// Daca nu mai am vieti si am pierdut iar bila, pierd jocul si il iau de la capat.
	if (lose < 0) {
		Tema1::Init();
	}
}

// Tratez coliziunile. Testez daca exista coliziune si reflect bila din caramida. La un anumit numar de caramizi sparte
// gasesc un power-up.
void Tema1::Collision(int ballX, int ballY, brick_coords &brick, float &speedX, float &speedY, int &bonus) {
	if (ballX + 10 >= brick.posX && ballX - 10 <= brick.posX + 60 && ballY + 10 >= brick.posY && ballY - 10 <= brick.posY + 30) {
		brick.hit = 1;
		if (((ballX + 10 >= brick.posX) && (ballX + 10 <= brick.posX + 10)) && ballY + 10 >= brick.posY && ballY - 10 <= brick.posY + 30) {
			speedX = -speedX;
		}
		if ((ballY + 10 >= brick.posY && ballY + 10 <= brick.posY + 10) && ballX + 10 >= brick.posX && ballX - 10 <= brick.posX + 60) {
			speedY = -speedY;
		}
		if ((ballX - 10 <= brick.posX + 60 && ballX - 10 >= brick.posX + 50) && ballY + 10 >= brick.posY && ballY - 10 <= brick.posY + 30) {
			speedX = -speedX;
		}
		if (ballY - 10 <= brick.posY + 30 && ballY - 10 >= brick.posY + 20 && ballX + 10 >= brick.posX && ballX - 10 <= brick.posX + 60) {
			speedY = -speedY;
		}
		bonus++;
		brick.power_up = bonus;
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	boardX = mouseX - 100;
	if (click == 0) {
		ballX = mouseX;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		speed = 200;
		click = 1;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}