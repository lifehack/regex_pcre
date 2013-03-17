#include <stdio.h>
#include <string.h>
#include "..\\pcre\\include\\regex.h"

#pragma comment(lib, "..\\pcre\\lib\\pcre.lib")

static char* substr(const char*str, unsigned start, unsigned end) 
{ 
	unsigned n = end - start; 
	static char stbuf[256]; 
	strncpy_s(stbuf, str + start, n); 
	stbuf[n] = 0; 
	return stbuf; 
}

int main(int argc, char *argv[])
{
	char line[1024];
	int i, result;

	char *pattern = "^\\s*originWeiboId:\\s*(\\d{16})\\s*commentUser:\\s*(\\d{10})\\s*commentId:\\s*(\\d{16})\\s*commentTime:\\s*(\\w{3} \\w{3} \\d{2} \\d{2}:\\d{2}:\\d{2} CST \\d{4})\\s*commentText:\\s*(.*)$";
	int nmatch = 6;
	regex_t reg;
	regmatch_t pmatch[6];

	char *outstr[6] = {
		"line is",
		"originWeiboId",
		"commentUser",
		"commentId",
		"commentTime",
		"commentText" 
	};

	result = regcomp(&reg, pattern, REG_EXTENDED);
	if (result != 0)
	{
		printf("comp error\n");
		return 1;
	}

	while (fgets(line, sizeof(line), stdin) != NULL)
	{
		result = regexec(&reg, line, nmatch, pmatch, 0);

		if (result == REG_NOMATCH)
			continue;
		else if (result != 0)
		{
			printf("exec error\n");
			return 1;
		}

		for(i = 0; i < nmatch && pmatch[i].rm_so != -1; i++)
		{
			printf("%s:'%s'\n", outstr[i], substr(line, pmatch[i].rm_so, pmatch[i].rm_eo));
		} 

		printf("\n\n");
	}

	return 0;
}