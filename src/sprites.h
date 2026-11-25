#ifndef SPRITES_H
#define SPRITES_H
#include <stdint.h>
typedef struct{uint8_t role;uint32_t col;uint8_t n;const uint8_t *r;}layer_t;
typedef struct{uint8_t nl;const layer_t *l;const uint8_t *pat[3];uint8_t pn[3];}fox_t;
typedef struct{uint8_t nl;const uint8_t *const *r;const uint8_t *n;}shape_t;
typedef struct{uint8_t shape;uint32_t col[7];}cloth_t;
extern const fox_t FOX[2];
extern const shape_t SHAPES[];
extern const cloth_t SHIRTS[9];
extern const cloth_t ACCS[14];
#endif
