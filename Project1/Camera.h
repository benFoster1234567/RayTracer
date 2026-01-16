#pragma once
#include "Common.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Ray.h"
#include "Canvas.h"

struct Camera
{
	unsigned int hSize{};
	unsigned int vSize{};
	float fieldOfView{};
	Matrix4 transform{};
	float halfWidth{};
	float halfHeight{};

	float pixelSize{};

	Camera();
	Camera(unsigned int hSize
		, unsigned int vSize
		, float fieldOfView
	);

	float calculatePixelSize();
	
	Ray rayForPixel(int px, int py) const;

};

class ImageRenderer
{
private:
	Camera camera{};
	Canvas canvas{};
	World world{};
	void renderChunk(Canvas& canvas, int startx, int starty, int endx, int endy);

public:

	ImageRenderer(Camera camera, World world);
	//ImageRenderer(const Camera& camera, const World& world);
	Canvas render();

};

Canvas render(const Camera& camera, const World& world);