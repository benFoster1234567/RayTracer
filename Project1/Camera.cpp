#include "Camera.h"
#include <numbers>
#include <thread>

Camera::Camera()
	: hSize(0)
	, vSize(0)
	, fieldOfView(std::numbers::pi)
	, transform(Matrix4::identity())
	, pixelSize(0)
	, halfWidth{}
	, halfHeight{}
{
}

Camera::Camera(unsigned int hSize, unsigned int vSize, float fieldOfView)
	: hSize(hSize)
	, vSize(vSize)
	, fieldOfView(fieldOfView)
	, transform(Matrix4::identity())
	, halfWidth{}
	, halfHeight{}
{
	this->pixelSize = calculatePixelSize();
}

float Camera::calculatePixelSize()
{
	auto halfView = tan(this->fieldOfView / 2);
	float aspect = static_cast<float>(hSize) / static_cast<float>(vSize);
	
	if (aspect >= 1)
	{
		halfWidth = halfView;
		halfHeight = halfView / aspect;
	}
	else
	{
		halfWidth = halfView * aspect;
		halfHeight = halfView;
	}

	return (halfWidth * 2) / hSize;
}


Ray Camera::rayForPixel(int px, int py) const
{
	float xOffset = (px + 0.5f) * this->pixelSize;
	float yOffset = (py + 0.5f) * this->pixelSize;

	float worldx = this->halfWidth - xOffset;
	float worldy = this->halfHeight - yOffset;

	auto pixel = this->transform.inverse() * Tuples::Point(worldx, worldy, -1);
	auto origin = this->transform.inverse() * Tuples::Point(0, 0, 0);

	auto direction = Tuples::normalize(pixel - origin);

	return Ray{ origin, direction };
}




Canvas render(const Camera& camera, const World& world)
{
	auto image = Canvas(camera.hSize, camera.vSize, Colors::Color(0, 0, 0));
	for (int y{0}; y < camera.vSize; y++)
		for (int x{ 0 }; x < camera.hSize; x++)
		{
			Ray ray = camera.rayForPixel(x, y);
			Colors::Color color = colorAt(world, ray);
			image.writePixel(x, y, color);
		}

	return image;
}




ImageRenderer::ImageRenderer(Camera camera, World world)
	: camera(camera)
	, world(world)
{
	this->canvas = Canvas(camera.hSize, camera.vSize, Colors::Color(0, 0, 0));
}

//
//ImageRenderer::ImageRenderer(const Camera& camera, const World& world)
//	: camera(camera)
//	, world(world)
//{
//	this->canvas = Canvas(camera.hSize, camera.vSize, Colors::Color(0, 0, 0));
//}

Canvas ImageRenderer::render()
{

	//unsigned int numThreads = std::thread::hardware_concurrency();
	//if (numThreads == 0) numThreads = 4;

	//int rowsPerThread = camera.vSize / numThreads;

	int rowsPerThread = 64;
	int numThreads = camera.vSize / rowsPerThread;

	Canvas resultCanvas(camera.hSize, camera.vSize, Colors::Color(0, 0, 0));

	std::vector<std::thread> threads;

	for (int i{ 0 }; i < numThreads; i++)
	{
		int x1 = 0;
		int y1 = i * rowsPerThread;
		int x2 = camera.hSize;
		int y2 = (i == numThreads - 1) ? camera.vSize : (i + 1) * rowsPerThread;
		threads.emplace_back(&ImageRenderer::renderChunk, this,
			std::ref(resultCanvas), x1, y1, x2, y2);
	}

	std::cout << "Main thread continuing..." << std::endl;
	for (auto& t : threads) {
		//if (t.joinable()) {
		//	t.join();
		//}
		t.join();
	}
	std::cout << "All threads joined." << std::endl;
	return resultCanvas;
}


void ImageRenderer::renderChunk(Canvas& canvas, int startx, int starty, int endx, int endy)
{
	if (startx >= endx || starty >= endy)
	{
		std::clog << "bad input" << std::endl;
		return;
	}

	for (int y{ starty }; y < endy; y++)
		for (int x{ startx }; x < endx; x++)
		{
			Ray ray = camera.rayForPixel(x, y);
			Colors::Color color = colorAt(world, ray);
			canvas.writePixel(x, y, color);
		}
}
