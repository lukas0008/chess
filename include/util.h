#define panic(s)                                                               \
  {                                                                            \
    dprintf(1, "%s\n", s);                                                     \
    exit(-1);                                                                  \
  }
