#include <Arduino.h>
#include <math.h>
#include "Wind.h"

float wind_best_ns = 0.0f;
float wind_best_ew = 0.0f;
float wind_speed = 0.0f;
float wind_direction = 0.0f;

static constexpr float KNOT_TO_MPS = 0.51444444f;

static float wrap_360(float deg)
{
  while (deg < 0.0f) deg += 360.0f;
  while (deg >= 360.0f) deg -= 360.0f;
  return deg;
}

static float wrap_180(float deg)
{
  while (deg <= -180.0f) deg += 360.0f;
  while (deg > 180.0f) deg -= 360.0f;
  return deg;
}

static void fill_projected_track(ufo_t *fop, float heading_deg, float speed_knots)
{
  float speed_ms = speed_knots * KNOT_TO_MPS;
  const float dt = 3.0f;   /* 6 točaka = 18 s unaprijed */

  for (int i = 0; i < 6; i++) {
    float t = dt * (float)(i + 1);

    float hdg = heading_deg + fop->turnrate * t;
    while (hdg < 0.0f)   hdg += 360.0f;
    while (hdg >= 360.0f) hdg -= 360.0f;

    float rad = hdg * DEG_TO_RAD;

    float vn_air = cosf(rad) * speed_ms;
    float ve_air = sinf(rad) * speed_ms;

    float vn_ground = vn_air + wind_best_ns;
    float ve_ground = ve_air + wind_best_ew;

    fop->air_ns[i] = (int16_t) lroundf(vn_ground);
    fop->air_ew[i] = (int16_t) lroundf(ve_ground);
  }
}

void Estimate_Wind(void)
{
  ufo_t *self = &ThisAircraft;
  uint32_t now = self->gnsstime_ms;

  if (now == 0) {
    return;
  }

  if (self->prevtime_ms == 0) {
    self->prevtime_ms = now;
    self->projtime_ms = now;
    self->prevcourse = self->course;
    self->prevheading = self->course;
    self->prevaltitude = self->altitude;
    self->heading = self->course;
    self->turnrate = 0.0f;
    self->airborne = (self->speed > 5.0f) ? 2 : 1;
    self->circling = 0;
    wind_best_ns = 0.0f;
    wind_best_ew = 0.0f;
    wind_speed = 0.0f;
    wind_direction = 0.0f;
    project_this(self);
    return;
  }

  float dt = 0.001f * (float)(now - self->prevtime_ms);

  if (dt <= 0.05f || dt > 10.0f) {
    self->prevtime_ms = now;
    self->projtime_ms = now;
    self->prevcourse = self->course;
    self->prevheading = self->heading;
    self->prevaltitude = self->altitude;
    project_this(self);
    return;
  }

  float dcourse = wrap_180(self->course - self->prevcourse);
  self->turnrate = dcourse / dt;

  if (self->speed < 5.0f) {
    self->airborne = 1;
    self->circling = 0;
    self->turnrate = 0.0f;
    self->heading = self->course;
    wind_best_ns = 0.0f;
    wind_best_ew = 0.0f;
  } else {
    self->airborne = 2;

    if (fabsf(self->turnrate) >= 2.0f) {
      self->airborne = 3;
      self->circling = (self->turnrate > 0.0f) ? 1 : -1;
    } else {
      self->circling = 0;
    }

    if (self->circling == 0) {
      self->heading = self->course;
    } else {
      self->heading = wrap_360(self->prevheading + self->turnrate * dt);
    }

    float speed_ms = self->speed * KNOT_TO_MPS;
    float cr = self->course * DEG_TO_RAD;
    float hr = self->heading * DEG_TO_RAD;

    float vn_ground = cosf(cr) * speed_ms;
    float ve_ground = sinf(cr) * speed_ms;
    float vn_air = cosf(hr) * speed_ms;
    float ve_air = sinf(hr) * speed_ms;

    float est_ns = vn_ground - vn_air;
    float est_ew = ve_ground - ve_air;

    if (self->circling != 0) {
      wind_best_ns = 0.8f * wind_best_ns + 0.2f * est_ns;
      wind_best_ew = 0.8f * wind_best_ew + 0.2f * est_ew;
    } else if (self->speed < 5.0f) {
      wind_best_ns = 0.0f;
      wind_best_ew = 0.0f;
    } else {
      wind_best_ns *= 0.98f;
      wind_best_ew *= 0.98f;
    }

    wind_speed = sqrtf(wind_best_ns * wind_best_ns +
                       wind_best_ew * wind_best_ew);
    wind_direction = wrap_360(atan2f(wind_best_ew, wind_best_ns) * RAD_TO_DEG);
  }

  self->prevtime_ms = now;
  self->projtime_ms = now;
  self->prevcourse = self->course;
  self->prevheading = self->heading;
  self->prevaltitude = self->altitude;

  project_this(self);
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

  uint32_t now = fop->gnsstime_ms;
  float dt = 0.0f;

  if (now != 0 && fop->prevtime_ms != 0 && now > fop->prevtime_ms) {
    dt = 0.001f * (float)(now - fop->prevtime_ms);
  }

  if (fop->heading == 0.0f) {
    fop->heading = (fop->prevheading != 0.0f) ? fop->prevheading : fop->course;
  }

  if (dt > 0.05f && dt < 10.0f) {
    if (fop->circling == 0) {
      fop->heading = fop->course;
    } else {
      float base = (fop->prevheading != 0.0f) ? fop->prevheading : fop->course;
      fop->heading = wrap_360(base + fop->turnrate * dt);
    }
  } else if (fop->circling == 0) {
    fop->heading = fop->course;
  }

  fop->projtime_ms = now;
  fill_projected_track(fop, fop->heading, fop->speed);

  fop->prevtime_ms = now;
  fop->prevcourse = fop->course;
  fop->prevheading = fop->heading;
  fop->prevaltitude = fop->altitude;
}
