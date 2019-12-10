#pragma once
#if !defined(__SphereA5_TYPES_H__)
#define __SphereA5_TYPES_H__

#pragma pack(push,8)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
static const word DEFAULT_OPTICAL_SENSOR_SHORT_INTERRUPT_TIMEOUT = 50;
static const word DEFAULT_OPTICAL_SENSOR_LONG_INTERRUPT_TIMEOUT = 50000;
static const byte DEFAULT_OPTICAL_SENSOR_LEVEL = 200;

// Типы данных для реализации SphereA5
struct StatusInfo 
{
	bool IsOpticalSensorInterrupted;
	bool IsOpticalSensorShortInterrupted;
	bool IsOpticalSensorLongInterrupted;
	bool IsSirenEnabled;
	bool IsLockOpened;
	bool IsResetFlagOn;

	bool IsDoorSensor1Closed;
	bool IsDoorSensor2Closed;
	bool IsDoorSensor3Closed;
	bool IsDoorSensor4Closed;
	bool IsDoorSensor5Closed;
};

} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#pragma pack(pop)
#endif // __SphereA5_TYPES_H__
