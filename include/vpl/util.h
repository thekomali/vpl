// do..while block is used not to provide looping but, it's
// a trick used to consider all the statments in one block
#define SWAP(a, b) \
  do { \
    __typeof__(a) _tmp = (a); \
    (a) = (b);  \
    (b) = _tmp; \
  } while (0)
