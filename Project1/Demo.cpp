#include "Demo.h"

void Demo::Demo::run()
{
    while (proj.position.y > 0.0f)
    {
        int x = static_cast<int>(proj.position.x);
        int y = static_cast<int>(height - proj.position.y); 
        if (x >= 0 && x < static_cast<int>(width) &&
            y >= 0 && y < static_cast<int>(height))
        {
            canvas.writePixel(x, y, Colors::Color(1.0f, 1.0f, 1.0f));
        }
        proj = tick(env, proj);
    }

    ppm = canvas.toPpm();
	toFile("projectile");
}

void Demo::Demo::toFile(std::string filename)
{
	filename.append(".ppm");
	std::ofstream outputFile(filename);

	if (outputFile.is_open()) {
		outputFile << canvas.toPpm();
		outputFile.close();
	}
	else {
		std::cerr << "Error opening file for writing" << std::endl;
	}
}
