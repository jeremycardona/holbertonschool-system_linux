#ifndef OPTIONS_H
#define OPTIONS_H

#define OPT_ONE_PER_LINE 1
#define OPT_ALL 2
#define OPT_ALMOST_ALL 4
#define OPT_LONG_FORMAT 8
#define OPT_REVERSE 16
#define OPT_SORT_SIZE 32
#define OPT_SORT_TIME 64
#define OPT_RECURSIVE 128

int parse_options(int argc, char *argv[], int *options);

#endif /* OPTIONS_H */