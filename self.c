#include <stdio.h>

//#include "error.h"
#include "parser.h"

int main()
{
	FILE *text_stream = fopen("./test.self", "r");
	//if (text < 0)
	//	pr_error("%s (line %d). Failed to open file", __func__, __LINE__);

	parse_prog(text_stream);

    fclose(text_stream);
	return 0;
}
