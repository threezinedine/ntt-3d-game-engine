#include "platforms/console.h"
#include <cstdio>
#include <cstring>

namespace ntt {

static void getAnsiColorCode(ConsoleColor color, bool isBackground, char* outCode, size_t outCodeSize);

void print(const char* message, ConsoleColor textColor, ConsoleColor backgroundColor, b8 isBold)
{
	char textCode[3];
	char backgroundCode[3];

	getAnsiColorCode(textColor, false, textCode, sizeof(textCode));
	getAnsiColorCode(backgroundColor, true, backgroundCode, sizeof(backgroundCode));

	char boldCode[3];
	memset(boldCode, 0, sizeof(boldCode));
	if (isBold)
	{
		snprintf(boldCode, sizeof(boldCode), "1;");
	}

	char textColorCode[6];
	memset(textColorCode, 0, sizeof(textColorCode));
	snprintf(textColorCode, sizeof(textColorCode), "%s", textCode);

	char backgroundColorCode[6];
	memset(backgroundColorCode, 0, sizeof(backgroundColorCode));
	if (backgroundColor != CONSOLE_COLOR_DEFAULT)
	{
		snprintf(backgroundColorCode, sizeof(backgroundColorCode), ";%s", backgroundCode);
	}

	printf("\033[%s%s%sm", boldCode, textColorCode, backgroundColorCode);

	printf("%s\n", message);

	printf("\033[0m"); // Reset to default colors
}

static void getAnsiColorCode(ConsoleColor color, bool isBackground, char* outCode, size_t outCodeSize)
{
	memset(outCode, 0, outCodeSize);

	switch (color)
	{
	case CONSOLE_COLOR_DEFAULT:
		break;
	case CONSOLE_COLOR_BLACK:
		snprintf(outCode, outCodeSize, isBackground ? "40" : "30");
		break;
	case CONSOLE_COLOR_RED:
		snprintf(outCode, outCodeSize, isBackground ? "41" : "31");
		break;
	case CONSOLE_COLOR_GREEN:
		snprintf(outCode, outCodeSize, isBackground ? "42" : "32");
		break;
	case CONSOLE_COLOR_YELLOW:
		snprintf(outCode, outCodeSize, isBackground ? "43" : "33");
		break;
	case CONSOLE_COLOR_BLUE:
		snprintf(outCode, outCodeSize, isBackground ? "44" : "34");
		break;
	case CONSOLE_COLOR_MAGENTA:
		snprintf(outCode, outCodeSize, isBackground ? "45" : "35");
		break;
	case CONSOLE_COLOR_CYAN:
		snprintf(outCode, outCodeSize, isBackground ? "46" : "36");
		break;
	case CONSOLE_COLOR_WHITE:
		snprintf(outCode, outCodeSize, isBackground ? "47" : "37");
		break;
	case CONSOLE_COLOR_COUNT:
		break;
	}
}

} // namespace ntt
