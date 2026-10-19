#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define vec_init(vec) _vec_init((void *)(vec), sizeof(**vec))
#define vec_push(vec, item) _vec_push((void *)(vec), item)
#define vec_push_many(vec, items, n) _vec_push_many((void *)(vec), items, n)
#define vec_len(vec) _vec_len((void *)(vec))
#define vec_pop(vec, out) _vec_pop((void *)(vec), out)
#define vec_print(vec) _vec_print((void *)(vec))
#define vec_reset(vec) _vec_reset((void *)(vec))
#define vec_free(vec) _vec_free((void *)(vec))

void _vec_init(void **vec, size_t type_size);
void _vec_push(void **vec, void *item);
void _vec_push_many(void **vec, void *items, size_t count);
void _vec_pop(void **vec, void *out);
size_t _vec_len(void **vec);
void _vec_print(void **vec);
void _vec_reset(void **vec);
void _vec_free(void **vec);

void **vec_iter(void *vec);

#ifdef DYNARRAY_IMPLEMENTATION

struct arr_header {
  size_t size;
  size_t length;
  size_t type_size;
  void *data;
};

struct vec_iter_data {
  size_t item_size;
  size_t length;
  void *vec;
};

/* generator is a void **; keep dynarray.h usable without an include-order
 * dependency on generator.h. */
void **generator_create(void func(void *), void *arg);
void generator_yield(void *value);

static void vec_iter_impl(void *data) {
  struct vec_iter_data *iter = data;

  for (size_t i = 0; i < iter->length; ++i) {
    generator_yield((char *)iter->vec + i * iter->item_size);
  }

  free(iter);
}

void **vec_iter(void *vec) {
  struct arr_header *header = (struct arr_header *)vec - 1;
  struct vec_iter_data *iter = malloc(sizeof(*iter));
  iter->item_size = header->type_size;
  iter->length = header->length;
  iter->vec = vec;
  return generator_create(vec_iter_impl, iter);
}

void _vec_init(void **vec, size_t type_size) {
  struct arr_header *header =
      (struct arr_header *)malloc(sizeof(struct arr_header));
  header->size = 0;
  header->type_size = type_size;
  header->length = 0;
  *vec = header + 1;
  header->data = *vec;
}

void _vec_push(void **vec, void *item) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  size_t osize = header->size;
  if (header->size == 0) {
    header->size = 4;
  } else if (header->length >= header->size) {
    header->size *= 2;
  }
  if (header->size != osize) {
    *vec = (struct arr_header *)realloc(header,
                                        sizeof(struct arr_header) +
                                            header->size * header->type_size) +
           1;
    header = (*(struct arr_header **)vec) - 1;
    header->data = *vec;
  }

  memcpy((char *)*vec + header->length * header->type_size, item,
         header->type_size);
  header->length++;
}

size_t _vec_len(void **vec) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  return header->length;
}

void _vec_push_many(void **vec, void *items, size_t count) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  for (size_t i = 0; i < count; i++) {
    _vec_push(vec, (char *)items + (i * header->type_size));
  }
}

void _vec_pop(void **vec, void *out) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  header->length--;
  if (out != NULL)
    memcpy(out, (char *)*vec + header->length * header->type_size,
           header->type_size);
}

void _vec_print(void **vec) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  // printf("size: %zu length: %zu pointer: %p\n", header->size, header->length,
  //        header->data);
}

void _vec_free(void **vec) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  free(header);
}

void _vec_reset(void **vec) {
  struct arr_header *header = (*(struct arr_header **)vec) - 1;
  header->length = 0;
}

#endif
