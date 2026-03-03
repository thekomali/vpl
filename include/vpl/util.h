// do..while block is used not to provide looping but, it's
// a trick used to consider all the statments in one block
#define SWAP(a, b) \
  do { \
    __typeof__(a) _tmp = (a); \
    (a) = (b);  \
    (b) = _tmp; \
  } while (0)


    /**
   * @brief Enum to identify the type of value in the array
   */
  typedef enum {INT, DOU, STR} etype_t;


  /**
   * @brief Struct to represent a single value
   */
  typedef struct {
    // determines the value present in the union
    etype_t etype;

    union {
      int    ival;
      double dval;
      char   *sval;
    } value;
  } element_t;


  /**
   * @brief Struct to represent a single node in the single linked list
   */
  typedef struct node{
    element_t data;         ///< data memeber of the node
    struct node *next;      ///< pointer refers to next node
  } node_t;


  /**
   * @brief Struct to represent a single node in the doubly linked list
   */
  typedef struct dnode{
    element_t data;         ///< data memeber of the node
    struct dnode *next;      ///< pointer refers to next node
    struct dnode *prev;      ///< pointer refers to previous node
  } dnode_t;
