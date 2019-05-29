#include <string>

#include <sstream>

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace sf;

using namespace std;

int winWidth = 1024;

int winHeight = 768;

int roadWidth = 2000;

int segmentLength = 200; 

float cameraDepth = 0.8; 

int rumbleLength = 3;

void drawRect(RenderWindow &window, Color col, int pointx1, int pointy1, int width1, int pointx2, int pointy2, int width2)
{
	ConvexShape road(4);
	
	road.setFillColor(col);
	
	road.setPoint(0, Vector2f(pointx1 - width1, pointy1));
	
	road.setPoint(1, Vector2f(pointx2 - width2, pointy2));
	
	road.setPoint(2, Vector2f(pointx2 + width2, pointy2));
	
	road.setPoint(3, Vector2f(pointx1 + width1, pointy1));
	
	window.draw(road);
}

struct Segment
{
	float xWorld, yWorld, zWorld; 

	float xCamera, yCamera, zCamera;

	float xProjected, yProjected;
	
	float XScreen, YScreen, projectedWidth; 
	
	float curve, spriteX, clip, scale;
	
	Sprite sprite;

	Segment()
	{
		spriteX = curve = xWorld = yWorld = zWorld = 0;
	}

	void ProjectionToScreen(int CameraX, int CameraY, int CameraZ)
	{
		
		
		xCamera = xWorld - CameraX;
		
		yCamera = yWorld - CameraY;

		zCamera = zWorld - CameraZ;

		scale = cameraDepth / (zWorld - CameraZ);

		//scale = scale + 1;

		xProjected = xCamera * scale;
		
		yProjected = yCamera * scale;

		XScreen = (1 + xProjected * winWidth / 2)+winWidth/2;
		
		YScreen = (1 - yProjected * winHeight / 2)+winHeight/2;
		
		projectedWidth = scale * roadWidth  * winWidth / 2;
	}

	void drawSprite(RenderWindow &game)
	{
		
		Sprite s = sprite;
		
		int w = s.getTextureRect().width;
		
		int h = s.getTextureRect().height;

		float destX = XScreen + scale * spriteX * winWidth / 2;
		
		float destY = YScreen + 4;
		
		float destW = w * projectedWidth / 256;
		
		float destH = h * projectedWidth / 256;

		destX += destW * spriteX; 
		
		destY += destH * (-1);    

		float clipH = destY + destH - clip;
		
		if (clipH < 0) clipH = 0;

		if (clipH >= destH) return;
		
		s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
		
		s.setScale(destW / w, destH / h);
		
		s.setPosition(destX, destY);

		
		game.draw(s);
	}
};


int main()
{
	RenderWindow game(VideoMode(winWidth, winHeight), "Car Racing!");


	
	game.setFramerateLimit(60);

	Texture text[50];

	Texture Car[50];
	
	Sprite object[50];

	Sprite Cars[50];
	
	for (int i = 1; i <= 20; i++)
	{
		text[i].loadFromFile("images/" + to_string(i) + ".png");
		
		text[i].setSmooth(true);
		
		object[i].setTexture(text[i]);
	}

	/*for (int i = 10; i <= 20; i++)
	{
		Car[i].loadFromFile("images/" + to_string(i) + ".png");

		Car[i].setSmooth(true);

		Cars[i].setTexture(text[i]);
	}
	*/

	Texture bg;
	
	bg.loadFromFile("images/bg.png");
	
	bg.setRepeated(true);
	
	Sprite sBackground(bg);
	
	sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
	
	sBackground.setPosition(-2000, 0);

	Texture tree;

	tree.loadFromFile("images/tree.png");

	Sprite Stree;

	Stree.setTexture(tree);

	Texture Obstacle;
	Obstacle.loadFromFile("images/10.png");
	Sprite Obs;
	Obs.setTexture(Obstacle);
	


	Texture Car1;
	Car1.loadFromFile("images/player_straight.png");
	Sprite CarNorm;
	CarNorm.setTexture(Car1);
	CarNorm.setPosition(winWidth / 2 - CarNorm.getGlobalBounds().width / 2, winHeight - CarNorm.getGlobalBounds().height * 1.5);
	CarNorm.scale(2.5,2.5);
	
	//CarNorm.setPosition(winWidth / 2 - CarNorm.getGlobalBounds().width / 2, winHeight - CarNorm.getGlobalBounds().height * 1.5);
	//game.draw(CarNorm);


	/*Texture Car2;
	Car2.loadFromFile("images/player_right.png");
	Sprite CarRight;
	CarRight.setTexture(Car1);*/

	/*Texture Car3;
	Car3.loadFromFile("images/player_left.png");
	Sprite CarLeft;
	CarLeft.setTexture(Car1);*/


	vector<Segment> segments;

	for (int i = 0; i < 1600; i++)
	{
		Segment segment;
		
		segment.zWorld = i * segmentLength;
		
		if (i > 300 && i < 700) segment.curve = 0.5;
		
		if (i > 1100) segment.curve = -0.7;
		
		if (i < 300 && i % 20 == 0) 
		{ 
			segment.spriteX = -2.5; segment.sprite = object[5]; 
		
		}

		if (i % 40 == 0)
		{
			segment.spriteX = 2.5; segment.sprite = Stree;
		}
		
		if (i % 80 == 0)
		{
			segment.spriteX = 0.3; segment.sprite = object[11];
		}
		
		if (i % 120 == 0)
		{
			segment.spriteX = -0.5; segment.sprite = object[12];
		}
		

		
		if (i % 10 == 0)
		{
			segment.spriteX = 2.0; segment.sprite = Stree;
		}

		if (i % 10 == 0)
		{
			segment.spriteX = -2.5; segment.sprite = Stree;
		}
		
		if (i % 17 == 0) 
		{ 
			segment.spriteX = 2.0; segment.sprite = object[6];
		}
		
		if (i > 300 && i % 20 == 0) 
		{ 
			segment.spriteX = -2.5; segment.sprite = object[4]; 
		}
		
		if (i > 800 && i % 20 == 0) 
		{ 
			segment.spriteX = -2.5; segment.sprite = object[1]; 
		}
		
		if (i == 400) 
		{ 
			segment.spriteX = -2.5; segment.sprite = object[7]; 
		}

		if (i > 750) segment.yWorld = sin(i / 20.0) * 1500;

		segments.push_back(segment);
	}

	int N = segments.size();
	
	float playerX = 0;

	float carX = 0;
	
	int position = 0;
	
	int CarHeight = 1500;

	Clock clock;

	while (game.isOpen())
	{
		
		Event evnt;
		
		while (game.pollEvent(evnt))
		{
			if (evnt.type == Event::Closed)
			{
				game.close();

			}
			if (evnt.type == Event::Resized)
			{
				game.close();

			}
		}

		int speed = 0;

		if (Keyboard::isKeyPressed(Keyboard::Right)) 
		{ 
			playerX += 0.1; 
			CarNorm.move(0.5, 0);
			cout << CarNorm.getPosition().x;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerX -= 0.1;
			CarNorm.move(-0.5, 0);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			speed = 200;
			CarNorm.move(0, 0);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			speed = -200;
			CarNorm.move(0,0);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Tab))
		{
			speed *= 1.5;
			CarNorm.move(0,0);
		}

		/*if (CarNorm.getPosition().x >(winWidth/2)+1000)

		{
			speed = speed - 100;
		}
		*/

		
		
		//if (Keyboard::isKeyPressed(Keyboard::W)) CarHeight += 100;
		
		//if (Keyboard::isKeyPressed(Keyboard::S)) CarHeight -= 100;

		position += speed;
		
		
		

		
		
		while (position >= N * segmentLength) position -= N * segmentLength;
		
		while (position < 0) position += N * segmentLength;

		

		if (playerX > 0.2)

		{
			speed = speed - 0.1;
		}

		if (playerX < -0.2)

		{
			speed = speed - 0.1;
		}
		
		
		

		game.clear(Color(105, 205, 4));
		
		game.draw(sBackground);
		
		int startPosition = position / segmentLength;
		
		int cameraHeight = segments[startPosition].yWorld + CarHeight;
		
		if (speed > 0) sBackground.move(-segments[startPosition].curve * 2, 0);
		
		if (speed < 0) sBackground.move(segments[startPosition].curve * 2, 0);

		int maxy = winHeight;
		
		float x = 0, dx = 0;

		for (int n = startPosition; n < startPosition + 300; n++)
		{
			Segment &p2 = segments[n%N];
			
			p2.ProjectionToScreen(playerX*roadWidth - x, cameraHeight, startPosition*segmentLength - (n >= N ? N * segmentLength : 0));
			
			x += dx;
			
			dx += p2.curve;

			p2.clip = maxy;
			
			if (p2.YScreen >= maxy) continue;
			
			maxy = p2.YScreen;

			Color grass = (n / rumbleLength) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
			
			Color rumble = (n / rumbleLength) % 2 ? Color::Red : Color::White;
			
			Color road = (n / rumbleLength) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Segment p1 = segments[(n - 1) % N]; 

			game.draw(CarNorm);
			
			drawRect(game, grass, 0, p2.YScreen, winWidth, 0, p1.YScreen, winWidth);
			
			drawRect(game, rumble, p2.XScreen, p2.YScreen, p2.projectedWidth*1.2, p1.XScreen, p1.YScreen, p1.projectedWidth*1.2);
			
			drawRect(game, road, p2.XScreen, p2.YScreen, p2.projectedWidth, p1.XScreen, p1.YScreen, p1.projectedWidth);
		}

		
		for (int n = startPosition + 300; n > startPosition; n--)

		{
			
			segments[n%N].drawSprite(game);
		
		}
		
		if (position >= 200000)

		{
			//RenderWindow TextShow(VideoMode(640, 480), "Game Over");

			int time = clock.getElapsedTime().asMilliseconds();
			
			cout << time;
			
			Font font;
			
			stringstream ss;
			
			ss << time;
			
			font.loadFromFile("arial.ttf");
			
			Text text;
			
			Text text2;
			
			text.setFont(font);
			
			text.setString("Track Length Finished and Elapsed Time");
			
			text2.setFont(font);
			
			text2.setString(ss.str());
			
			text.setCharacterSize(30);
			
			text2.setCharacterSize(30);
			
			text.setFillColor(sf::Color::Red);
			
			text.setStyle(sf::Text::Bold | sf::Text::Underlined);
			
			text2.setCharacterSize(100);
			
			text2.setFillColor(sf::Color::Red);
			
			text2.setStyle(sf::Text::Bold | sf::Text::Underlined);
			
			text.setPosition(Vector2f(100, 100));
			
			text2.setPosition(Vector2f(100, 200));
			
			//clock.restart;
			
			game.draw(text);
			game.draw(text2);

			

			


		}

		game.display();
	}
	/*if (CarNorm.getGlobalBounds().intersects(Stree.getGlobalBounds()))
	{
		cout << "Collision Detected";
	}
	else cout << "No Collsion"; */

	return 0;
}
