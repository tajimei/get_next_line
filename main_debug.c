#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;
	int		count;

	/* ===== テスト1: ファイルから読み取り ===== */
	printf("=== Test 1: Read from file ===\n");
	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Could not open test.txt\n");
		return (1);
	}
	count = 1;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: %s", count, line);
		free(line);
		count++;
	}
	close(fd);

	/* ===== テスト2: NULLが正しく返るか ===== */
	printf("\n=== Test 2: Returns NULL at EOF ===\n");
	fd = open("test.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
	if (line == NULL)
		printf("OK: NULL returned at EOF\n");
	close(fd);

	/* ===== テスト3: 空ファイル ===== */
	printf("\n=== Test 3: Empty file ===\n");
	fd = open("empty.txt", O_RDONLY);
	if (fd < 0)
		printf("Error: Could not open empty.txt\n");
	else
	{
		line = get_next_line(fd);
		if (line == NULL)
			printf("OK: NULL returned for empty file\n");
		else
		{
			printf("NG: Expected NULL but got: %s\n", line);
			free(line);
		}
		close(fd);
	}

	/* ===== テスト4: 無効なfd ===== */
	printf("\n=== Test 4: Invalid fd ===\n");
	line = get_next_line(-1);
	if (line == NULL)
		printf("OK: NULL returned for fd = -1\n");
	else
	{
		printf("NG: Expected NULL but got: %s\n", line);
		free(line);
	}

	/* ===== テスト5: 標準入力 ===== */
	printf("\n=== Test 5: Standard input (type a line and press Enter) ===\n");
	line = get_next_line(0);
	if (line)
	{
		printf("Read from stdin: %s", line);
		free(line);
	}

	return (0);
}