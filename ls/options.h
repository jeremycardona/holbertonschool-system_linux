#ifndef OPTIONS_H
#define OPTIONS_H

#define OPT_LONG_FORMAT  0x01
#define OPT_ALL          0x02
#define OPT_ALMOST_ALL   0x04
#define OPT_REVERSE      0x08
#define OPT_SORT_TIME    0x10
#define OPT_SORT_SIZE    0x20
#define OPT_ONE_PER_LINE 0x40

int parse_options(int argc, char *argv[], int *options);

#endif /* OPTIONS_H */