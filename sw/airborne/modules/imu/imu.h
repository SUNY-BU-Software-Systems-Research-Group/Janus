/*
 * Copyright (C) 2008-2009 Antoine Drouin <poinix@gmail.com>
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file modules/imu/imu.h
 * Inertial Measurement Unit interface.
 */

#ifndef IMU_H
#define IMU_H

#include "math/pprz_algebra_int.h"
#include "math/pprz_algebra_float.h"
#include "math/pprz_orientation_conversion.h"
#include "generated/airframe.h"

#ifndef IMU_MAX_SENSORS
#define IMU_MAX_SENSORS 3
#endif

struct imu_gyro_t {
  uint8_t abi_id;
  uint32_t last_stamp;
  bool calibrated;
  struct Int32Rates scaled;
  struct Int32Rates unscaled;
  struct Int32Rates neutral;
  struct Int32Rates scale[2];
  struct Int32RMat imu_to_sensor;   ///< Rotation from imu to sensor frame
};

struct imu_accel_t {
  uint8_t abi_id;
  uint32_t last_stamp;
  bool calibrated;
  struct Int32Vect3 scaled;
  struct Int32Vect3 unscaled;
  struct Int32Vect3 neutral;
  struct Int32Vect3 scale[2];
  struct Int32RMat imu_to_sensor;   ///< Rotation from imu to sensor frame
};

struct imu_mag_t {
  uint8_t abi_id;
  bool calibrated;
  struct Int32Vect3 scaled;
  struct Int32Vect3 unscaled;
  struct Int32Vect3 neutral;
  struct Int32Vect3 scale[2];
  struct FloatVect3 current_scale;
  struct Int32RMat imu_to_sensor;   ///< Rotation from imu to sensor frame
};


/** abstract IMU interface providing fixed point interface  */
struct Imu {
  bool initialized;
  struct imu_gyro_t gyros[IMU_MAX_SENSORS];
  struct imu_accel_t accels[IMU_MAX_SENSORS];
  struct imu_mag_t mags[IMU_MAX_SENSORS];
  struct OrientationReps body_to_imu; ///< rotation from body to imu frame

  /** flag for adjusting body_to_imu via settings.
   * if FALSE, reset to airframe values, if TRUE set current roll/pitch
   */
  bool b2i_set_current;
};

/** global IMU state */
extern struct Imu imu;

/** External functions */
extern void imu_init(void);
extern void imu_set_defaults_gyro(uint8_t abi_id, const struct Int32RMat *imu_to_sensor, const struct Int32Rates *neutral, const struct Int32Rates *scale);
extern void imu_set_defaults_accel(uint8_t abi_id, const struct Int32RMat *imu_to_sensor, const struct Int32Vect3 *neutral, const struct Int32Vect3 *scale);
extern void imu_set_defaults_mag(uint8_t abi_id, const struct Int32RMat *imu_to_sensor, const struct Int32Vect3 *neutral, const struct Int32Vect3 *scale);

extern struct imu_gyro_t *imu_get_gyro(uint8_t sender_id, bool create);
extern struct imu_accel_t *imu_get_accel(uint8_t sender_id, bool create);
extern struct imu_mag_t *imu_get_mag(uint8_t sender_id, bool create);

extern void imu_SetBodyToImuPhi(float phi);
extern void imu_SetBodyToImuTheta(float theta);
extern void imu_SetBodyToImuPsi(float psi);
extern void imu_SetBodyToImuCurrent(float set);
extern void imu_ResetBodyToImu(float reset);

#if !defined IMU_BODY_TO_IMU_PHI && !defined IMU_BODY_TO_IMU_THETA && !defined IMU_BODY_TO_IMU_PSI
#define IMU_BODY_TO_IMU_PHI   0
#define IMU_BODY_TO_IMU_THETA 0
#define IMU_BODY_TO_IMU_PSI   0
#endif


#endif /* IMU_H */