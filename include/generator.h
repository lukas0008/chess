#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <threads.h>

typedef void **generator;
#define foreach(gen, item)                                                     \
  for (void **_ge = (gen), *item = generator_next(&_ge);                       \
       generator_value_is_good(); item = generator_next(&_ge))
generator generator_create(void func(void *), void *arg);
void generator_yield(void *value);
int generator_value_is_good();
void *generator_next(generator *gen);
void generator_set_stack_size(uint64_t size);
generator range(uint64_t min, uint64_t max);
#endif

#ifdef GENERATOR_IMPLEMENTATION
#define generator_rip(gen) ((gen)[0])
#define generator_rsp(gen) ((gen)[1])
#define generator_rsp_callee(gen) ((gen)[2])
#define generator_init_rdi(gen) ((gen)[3])
#define generator_stack_size(gen) ((gen)[4])
extern void _generator_pass_value(generator gen, void *value);
extern void *_generator_destroy();
extern void *_generator_next(generator *gen);
thread_local int generator_eol = 0;
thread_local generator __current_gen;
uint64_t _generator_stack_size = 1024 * 1024;
void generator_set_stack_size(uint64_t size) { _generator_stack_size = size; }
void generator_yield(void *value) {
  _generator_pass_value(__current_gen, value);
}
void *generator_next(generator *gen) {
  generator prev = __current_gen;
  __current_gen = *gen;
  void *value = _generator_next(gen);
  __current_gen = prev;
  return value;
}

int generator_value_is_good() {
  if (generator_eol) {
    generator_eol = 0;
    return 0;
  }
  return 1;
}
generator generator_create(void func(void *), void *arg) {
  generator gen =
      (generator)mmap(NULL, _generator_stack_size, PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  generator gencpy = (generator)((char *)gen + _generator_stack_size);
  gencpy -= 2;
  *(gencpy--) = gen;
  *(gencpy--) = (void *)_generator_destroy;
  generator_rsp(gen) = gencpy + 1;
  generator_rip(gen) = (void *)func;
  generator_stack_size(gen) = (void *)_generator_stack_size;
  generator_init_rdi(gen) = arg;
  return gen;
}

void *generator_destroy(generator gen) {
  munmap(gen, (uint64_t)generator_stack_size(gen));
  generator_eol = 1;
  return NULL;
}

struct range_data {
  uint64_t min;
  uint64_t max;
};

void range_impl(void *d) {
  struct range_data *rd = d;
  uint64_t min = rd->min;
  uint64_t max = rd->max;
  for (uint64_t i = min; i < max; ++i) {
    generator_yield((void *)i);
  }
  free(d);
}

generator range(uint64_t min, uint64_t max) {
  struct range_data *d = malloc(sizeof(struct range_data));
  d->min = min;
  d->max = max;
  return generator_create(range_impl, d);
}
#endif
