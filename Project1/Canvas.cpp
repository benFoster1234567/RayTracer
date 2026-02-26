#include "Canvas.h"

std::string Canvas::toPpm()
{
	std::string str = "";
	str.append("P3").append("\n");
	str.append(std::to_string(width))
		.append(" ")
		.append(std::to_string(height))
		.append("\n");
	str.append(std::to_string(255)).append("\n");

	for (size_t i{ 0 }; i < width * height; i++)
	{
		const auto& c = pixels[i];

		auto colorToPpmScale = [](float val) -> int
			{
				val = std::clamp(val, 0.0f, 1.0f);
				return static_cast<int>(std::round(val * 255.0f));
			};

		int red = colorToPpmScale(c.r);
		str.append(std::to_string(red)).append(" ");
		int green = colorToPpmScale(c.g);
		str.append(std::to_string(green)).append(" ");
		int blue = colorToPpmScale(c.b);
		str.append(std::to_string(blue)).append(" ");

		if ((i + 1) % width == 0)
		{
			str.append("\n");
		}

	}
	//std::cout << str << std::endl;

	return str;
}

void Canvas::toFile(std::string filename)
{
	filename.append(".ppm");
	std::ofstream outputFile(filename);

	if (outputFile.is_open()) {
		outputFile << this->toPpm();
		outputFile.close();
	}
	else {
		std::cerr << "Error opening file for writing" << std::endl;
	}
}