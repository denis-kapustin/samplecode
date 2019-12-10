#pragma once
#if !defined(__SphereA5_DATA_TRANSFER_PROTOCOL_H__)
#define __SphereA5_DATA_TRANSFER_PROTOCOL_H__

#include <hardware/ProtocolBase.h>

#pragma pack(push,8)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
/**
 * 
 */
class DataTransferProtocol 
	: public HardwareFramework::DataTransferProtocolBase
{
	HWLIB_STDMETHOD(SendRequest)(IN const byte cmdId, IN const byte* const pb, IN const uint_t size) HWLIB_NOEXCEPT;
	HWLIB_STDMETHOD(ReadResponse)(IN const byte cmdId) HWLIB_NOEXCEPT;
	
protected:
	DataTransferProtocol(void) HWLIB_NOEXCEPT: HardwareFramework::DataTransferProtocolBase(128) {}
//	~DataTransferProtocol(void) HWLIB_NOEXCEPT {}

	HWLIB_STDMETHOD(SendCommand)(IN const byte cmdId, IN const byte* const pb, IN const uint_t size) HWLIB_NOEXCEPT;
private:
public:
};

} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#pragma pack(pop)
#endif // __SphereA5_DATA_TRANSFER_PROTOCOL_H__
