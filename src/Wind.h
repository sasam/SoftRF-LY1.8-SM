#ifndef WIND_H
#define WIND_H

#include "SoftRF.h"

extern float wind_best_ns;
extern float wind_best_ew;
extern float wind_speed;
extern float wind_direction;

void Estimate_Wind(void);
void project_this(ufo_t *this_aircraft);
void project_that(ufo_t *fop);

#endif /* WIND_H */
