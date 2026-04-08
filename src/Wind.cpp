#include <Arduino.h>
#include <math.h>
#include "Wind.h"

float wind_best_ns = 0.0f;
float wind_best_ew = 0.0f;
float wind_speed = 0.0f;
float wind_direction = 0.0f;

static constexpr float KNOT_TO_MPS = 0.51444444f;

static void fill_projected_track(ufo_t *fop, float heading_deg, float speed_knots)
{
  float speed_ms = speed_knots * KNOT_TO_MPS;
  float rad = heading_deg * DEG_TO_RAD;

  float vn = cosf(rad) * speed_ms;
  float ve = sinf(rad) * speed_ms;

  for (int i = 0; i < 6; i++) {
    fop->air_ns[i] = (int16_t) lroundf(vn);
    fop->air_ew[i] = (int16_t) lroundf(ve);
  }
}

void Estimate_Wind(void)
{
  /* Minimalni, sigurni početak:
     još ne procjenjujemo stvarni vjetar,
     samo držimo neutralan wind model. */
  wind_best_ns = 0.0f;
  wind_best_ew = 0.0f;
  wind_speed = 0.0f;
  wind_direction = 0.0f;
}

void project_this(ufo_t *this_aircraft)
{
  if (this_aircraft == NULL) {
    return;
  }

  this_aircraft->prevtime_ms = this_aircraft->gnsstime_ms;
  this_aircraft->projtime_ms = this_aircraft->gnsstime_ms;

  this_aircraft->prevcourse = this_aircraft->course;
  this_aircraft->prevheading = this_aircraft->heading;
  this_aircraft->prevaltitude = this_aircraft->altitude;

  if (this_aircraft->heading == 0.0f && this_aircraft->course != 0.0f) {
    this_aircraft->heading = this_aircraft->course;
  }

  fill_projected_track(this_aircraft, this_aircraft->heading, this_aircraft->speed);
}

void project_that(ufo_t *fop)
{
  if (fop == NULL) {
    return;
  }

  fop->prevtime_ms = fop->gnsstime_ms;
  fop->projtime_ms = fop->gnsstime_ms;

  fop->prevcourse = fop->course;
  fop->prevheading = fop->heading;
  fop->prevaltitude = fop->altitude;

  if (fop->heading == 0.0f && fop->course != 0.0f) {
    fop->heading = fop->course;
  }

  fill_projected_track(fop, fop->heading, fop->speed);
}
