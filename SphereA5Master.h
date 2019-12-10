#pragma once
#if !defined(__SphereA5_MASTER_H__)
#define __SphereA5_MASTER_H__

#include "SphereA5Interface.h"
#include "SphereA5DataTransferProtocol.h"

#pragma pack(push,8)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
/**
 * 
 */
class Master 
	//: public SphereA5Interface
	: public DataTransferProtocol
{
	Master(const Master&);
	Master& operator=(const Master&);

	HWLIB_STDMETHOD(_ClearFlag)(IN const byte cmdId) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(_ManageDevice)(IN const byte paramId, IN const word milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(_SetParameters)(IN const byte cell, IN const word val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(_GetParameters)(IN const byte cell, OUT word& val) HWLIB_NOEXCEPT;

public:
	Master(void) HWLIB_NOEXCEPT {}

	HWLIB_STDMETHOD(RequestStatus)(OUT StatusInfo& statusInfo) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(RequestBoardInfo)(OUT stringA_t& boardInfo) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(Reset)(void) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(ClearResetFlag)(void) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(ClearOpticalSensorInterruptedEvents)(void) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(ReadOpticalSensorEventCounter)(OUT word& eventCounter) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(OpenLock)(IN const word milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(OpenLockInfiniteTime)(void) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(CloseLock)(void) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(EnableSiren)(IN const word milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(EnableSirenInfiniteTime)(void) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(DisableSiren)(void) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(GetOpticalSensorShortInterruptValue)(OUT word& val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(SetOpticalSensorShortInterruptValue)(IN const word val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(GetOpticalSensorLongInterruptValue)(OUT word& val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(SetOpticalSensorLongInterruptValue)(IN const word val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(GetOpticalSensorLevel)(OUT byte& val) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(SetOpticalSensorLevel)(IN const byte val) HWLIB_NOEXCEPT;

	HWLIB_STDMETHOD(GetOpticalSensorShortInterruptTimeout)(OUT word& milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(SetOpticalSensorShortInterruptTimeout)(IN const word milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(GetOpticalSensorLongInterruptTimeout)(OUT word& milliseconds) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(SetOpticalSensorLongInterruptTimeout)(IN const word milliseconds) HWLIB_NOEXCEPT;

}; // class Master
} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#pragma pack(pop)
#endif // __SphereA5_MASTER_H__
