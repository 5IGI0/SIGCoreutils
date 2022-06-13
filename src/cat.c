/**
 * @file cat.c
 * @author 5IGI0
 * @brief This is a basic `cat` implementation.
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>

#define FLAG_E 1
#define FLAG_B 2
#define FLAG_N 4
#define FLAG_S 8
#define FLAG_V 16
#define FLAG_T 32

const char *help_msg =
"Usage: %s [OPTION]... [FILE...]\n"
"\n"
"Options:\n"
" -b, --number-nonblank  number nonempty output lines, overrides -n\n"
" -E, --show-ends        display $ at end of each line\n"
" -n, --number           number all output lines\n"
" -s, --squeeze-blank    suppress repeated empty output lines\n"
" -T, --show-tabs        display TAB characters as ^I\n"
" -v, --show-nonprinting use ^ and M- notation, except for LFD and TAB\n"
"     --help             display this help and exit\n"
" -A, --show-all         equivalent to -vET\n"
" -t                     equivalent to -vT\n"
" -e                     equivalent to -vE\n";

int parse_args(int argc, const char **argv) {
	int ret = 0;
	int c;
	int opt_index;
	const struct option options[] = {
		{"show-all", no_argument, 0, 'A'},
		{"number-nonblank", no_argument, 0, 'b'},
		{"", no_argument, 0, 'e'},
		{"show-ends", no_argument, 0, 'E'},
		{"squeeze-blank", no_argument, 0, 's'},
		{"", no_argument, 0, 't'},
		{"showtabs", no_argument, 0, 'T'},
		{"show-nonprinting", no_argument, 0, 'v'},
		{"number", no_argument, 0, 'n'},
		{"", no_argument, 0, 'u'},
		{"help", no_argument, 0, 1},
		{0,0,0,0}
	};

	while ((c = getopt_long(argc, (void *)argv, "AbeEstTvn", options, &opt_index)) >= 0) {
		switch (c)
		{
			case 'A': ret |= FLAG_V | FLAG_E | FLAG_T; break;
			case 'b': ret |= FLAG_B; break;
			case 'e': ret |= FLAG_E | FLAG_V; break;
			case 'E': ret |= FLAG_E; break;
			case 'n': ret |= FLAG_N; break;
			case 's': ret |= FLAG_S; break;
			case 't': ret |= FLAG_T | FLAG_V; break;
			case 'T': ret |= FLAG_T; break;
			case 'v': ret |= FLAG_V; break;
			case 'u': break;
			case 1:   printf(help_msg, argv[0], argv[0]); return -1;
			default:  fputs("please, use --help for help message\n", stderr); return -2;
		}
	}
	
	return ret;
}

void print_special_char(int chr) {
	if (chr < ' ') {
		char tmp[] = "^A";
		tmp[1] = '@' + chr;
		fputs(tmp, stdout);
	} else if (chr == 127)
		fputs("^?", stdout);
	else if (chr > '~' && chr < 160) {
		char tmp[] = "M-^A";
		tmp[3] = '@' + (chr - '~') - 2;
		fputs(tmp, stdout);
	} else if (chr < 255){
		char tmp[] = "M-A";
		tmp[2] = ' ' + (chr - 160);
		fputs(tmp, stdout);
	} else
		fputs("M-^?", stdout);
}

void print_char(int flags, int chr) {
	static unsigned long line_count = 0;
	static int newline = 1, emptyline = 0;

	if (emptyline && chr == '\n')
		return;
	else if (emptyline && chr != '\n')
		emptyline = 0;
	if (newline == 1 && chr == '\n')
		emptyline = 1;

	if (
		(flags & FLAG_N || (flags & FLAG_B && chr != '\n')) &&
		newline
	)
		printf("%6lu ", ++line_count);
	else if (flags & FLAG_B && chr == '\n')
		fputs("      ", stdout);

	newline -= newline;

	if (flags & FLAG_E && chr == '\n')
		putc('$', stdout);
	if (flags & FLAG_T && chr == '\t')
		fputs("^I", stdout);
	else if (flags & FLAG_V && (chr < '\t' || (chr > '\n' && chr < ' ') || chr > '~'))
		print_special_char(chr);
	else
		putc(chr, stdout);

	newline += (chr == '\n');
}

int main(int argc, const char **argv) {
	int	flags = parse_args(argc, argv);
	int	stdin_read = 0;

	if (flags < 0)
		exit(-(flags + 1));

	for (size_t i = optind; i < (unsigned)argc || i == (unsigned)optind; i++) {
		FILE *fp;

		if (i == (unsigned)argc || strcmp("-", argv[i]) == 0) {// no argument or stdin
			if (stdin_read)
				continue;
			fp = stdin;
			stdin_read = 1;
		} else
			fp = fopen(argv[i], "rb");

		if (fp == NULL) {
			perror(argv[i]);
			continue;
		}

		errno = 0;
		while (1) {
			int chr = fgetc(fp);
			if (chr < 0) {
				if (errno != 0)
					perror(argv[i]);
				break;
			}
			print_char(flags, chr);
		}

		fclose(fp);
	}
}