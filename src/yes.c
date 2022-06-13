/**
 * @file cat.c
 * @author 5IGI0
 * @brief This is a basic `yes` implementation.
 * @version 0.1
 * @date 2022-06-12
 * 
 * @note I didn't implemtented -- (yes -- --help)
 * 
 * @copyright Copyright (c) 2022
 **/
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	for (size_t i = 1; i < (unsigned)argc; i++) {
		if (strcmp("--help", argv[i]) == 0) {
			printf("Usage: %s [STRING]...\n", argv[0]);
			return 0;
		}
	}

	if (argc == 1) {
		while (1)
			fwrite("y\n", 1, 2, stdout);
	} else {
		while (1) {
			for (size_t i = 1; i < (unsigned)argc; i++) {
				fputs(argv[i], stdout);
				fputs((i == (unsigned)argc-1) ? "\n" : " ", stdout);
			}
		}
	}
}