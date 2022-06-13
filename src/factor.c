/**
 * @file cat.c
 * @author 5IGI0
 * @brief This is a basic `factor` implementation.
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 **/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char *ERR_MSG = "%s: '%s' is not a valid positive integer\n";
const char *SPACES = "\f\n\r\t\v ";

void factorize(unsigned int start) {
	unsigned int tmp = start;
	unsigned int stop = sqrt(tmp) + 1;

	printf("%u: ", tmp);
	for (size_t i = 2; i <= stop; i += 1 + (i != 2)) {
		while (tmp % i == 0) {
			printf("%s%zu", (tmp == start) ? "" : " ", i);
			tmp /= i;
		}
	}

	if (tmp > 1)
		printf("%s%u", (tmp == start) ? "" : " ", tmp);
	putc('\n', stdout);
}

int validate_number(const char *str) {
	long tmp = strtol(str, NULL, 10);

	return (
		str[0] &&
		str[strspn(str, "0123456789")] == 0 &&
		tmp > 0 &&
		tmp <= ((unsigned int)-1)
	);
}

int main(int argc, char **argv) {
	for (size_t i = 1; i < (unsigned int)argc; i++) {
		if (!strcmp("--help", argv[i])) {
			printf("Usage: %s [NUMBER]...\n", argv[0]);
			return 0;
		}
	}
	
	if (argc != 1) {
		for (size_t i = 1; i < (unsigned int)argc; i++) {
			if (!validate_number(argv[i]))
				fprintf(stderr, ERR_MSG, argv[0], argv[i]); // would escape strings
			else
				factorize(strtol(argv[i], NULL, 0));
		}
	} else {
		size_t bufflen = 0;
		char *line, *ptr, *saveptr;

		while (getline(&line, &bufflen, stdin) > 0) {
			ptr = strtok_r(line, SPACES, &saveptr);
			do {
				if (!validate_number(ptr))
					fprintf(stderr, ERR_MSG, argv[0], ptr);
				else
					factorize(strtol(ptr, NULL, 0));
			} while ((ptr = strtok_r(NULL, SPACES, &saveptr)));
		}

		free(line);
	}
}