#include "stdafx.h"
#include "SphereA5Master.h"
#include "SphereA5CommandCodes.h"

#define SEND_COMMAND_WITH_DATA(cmdId, timeout, pb, size) HardwareFramework::AutoOpenCloseDeviceConnection pdc(m_pDeviceConnection);\
	SetCmdTimeout(timeout);\
	hresult hr; if (FAILED(hr = SendCommand(cmdId, pb, size))) return hr;
#define SEND_COMMAND(cmdId, timeout) SEND_COMMAND_WITH_DATA(cmdId, timeout, NULL, 0)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
/**
  * Таймаут получения ответа на команду, милисек
  */
static const dword RESP_TIMEOUT = 500;


const byte Encode(IN const byte b) HWLIB_NOEXCEPT
{
	const byte b1(b & 0x0f);
	return (b1 < 10 ? b1 + 0x30 : b1 + 0x41 - 10);
}

const byte Decode(IN const byte b) HWLIB_NOEXCEPT
{
	if ((0x30 <= b) && (b <= 0x39)) return b - 0x30;
	if ((0x41 <= b) && (b <= 0x47)) return b - 0x41 + 10;
	return 0xff;
}

//////////////////////////////////////////////////////////////////////////
HWLIB_STDMETHODIMP Master::RequestStatus(OUT StatusInfo& statusInfo) HWLIB_NOEXCEPT
{
	SEND_COMMAND(CMD_REQUEST_STATUS, RESP_TIMEOUT)
	CHECK_RESPONSE_DATA_SIZE(2 == m_nResponseDataSize, L"RequestStatus: Unexpected length")

	statusInfo.IsOpticalSensorInterrupted      = (0 == (m_pResponseData[0] & 0x01));
	statusInfo.IsOpticalSensorShortInterrupted = (0 != (m_pResponseData[0] & 0x02));
	statusInfo.IsOpticalSensorLongInterrupted  = (0 != (m_pResponseData[0] & 0x04));
	statusInfo.IsSirenEnabled = (0 != (m_pResponseData[0] & 0x08));
	statusInfo.IsLockOpened   = (0 != (m_pResponseData[0] & 0x10));
	statusInfo.IsResetFlagOn  = (0 != (m_pResponseData[0] & 0x40));

	statusInfo.IsDoorSensor1Closed = (0 == (m_pResponseData[1] & 0x01));
	statusInfo.IsDoorSensor2Closed = (0 == (m_pResponseData[1] & 0x02));
	statusInfo.IsDoorSensor3Closed = (0 == (m_pResponseData[1] & 0x04));
	statusInfo.IsDoorSensor4Closed = (0 == (m_pResponseData[1] & 0x08));
	statusInfo.IsDoorSensor5Closed = (0 == (m_pResponseData[1] & 0x10));
	return HWLIB_S_OK;
}

HWLIB_STDMETHODIMP Master::RequestBoardInfo(OUT stringA_t& boardInfo) HWLIB_NOEXCEPT
{
	SEND_COMMAND(CMD_REQUEST_BOARD_INFO, RESP_TIMEOUT)
	CHECK_RESPONSE_DATA_SIZE(0 != m_nResponseDataSize, L"RequestBoardInfo: Unexpected length")

	boardInfo.assign(m_pResponseData, m_pResponseData + m_nResponseDataSize);
	return HWLIB_S_OK;
}

HWLIB_STDMETHODIMP Master::Reset(void) HWLIB_NOEXCEPT
{
	SEND_COMMAND(CMD_RESET, RESP_TIMEOUT)
	CHECK_RESPONSE_DATA_SIZE(1 == m_nResponseDataSize, L"Reset: Unexpected length")
	return (ASCII_PLUS == *m_pResponseData ? HWLIB_S_OK : HWLIB_E_FAIL);
}

HWLIB_STDMETHODIMP Master::_ClearFlag(IN const byte cmdId) HWLIB_NOEXCEPT
{
	SEND_COMMAND(cmdId, RESP_TIMEOUT)
	CHECK_RESPONSE_DATA_SIZE(1 == m_nResponseDataSize, L"_ClearFlag: Unexpected length")

	return (ASCII_PLUS == *m_pResponseData ? HWLIB_S_OK : HWLIB_E_FAIL);
}

HWLIB_STDMETHODIMP Master::ClearResetFlag(void) HWLIB_NOEXCEPT
{
	return _ClearFlag(CMD_CLEAR_RESET_FLAG);
}

HWLIB_STDMETHODIMP Master::ClearOpticalSensorInterruptedEvents(void) HWLIB_NOEXCEPT
{
	return _ClearFlag(CMD_CLEAR_OPTICAL_SENSOR_INTERRUPTED_EVENTS);
}

HWLIB_STDMETHODIMP Master::ReadOpticalSensorEventCounter(OUT word& eventCounter) HWLIB_NOEXCEPT
{
	//SEND_COMMAND(CMD_READ_OPTICAL_SENSOR_EVENT_COUNTER, RESET_RESP_TIMEOUT)
	SEND_COMMAND_WITH_DATA(CMD_READ_OPTICAL_SENSOR_EVENT_COUNTER, RESP_TIMEOUT, &ASCII_R, 1)
	CHECK_RESPONSE_DATA_SIZE(4 == m_nResponseDataSize, L"ReadOpticalSensorEventCounter: Unexpected length")

	eventCounter =
		(Decode(m_pResponseData[0]) << 12) +
		(Decode(m_pResponseData[1]) << 8) +
		(Decode(m_pResponseData[2]) << 4) +
		Decode(m_pResponseData[3]);
	return HWLIB_S_OK;
}

HWLIB_STDMETHODIMP Master::_ManageDevice(IN const byte paramId, IN const word milliseconds) HWLIB_NOEXCEPT
{
	const byte data[] = { paramId
		, Encode(milliseconds >> 12)
		, Encode(milliseconds >> 8)
		, Encode((byte)milliseconds >> 4)
		, Encode((byte)milliseconds)
	};
	SEND_COMMAND_WITH_DATA(CMD_MANAGE_DEVICE, RESP_TIMEOUT, data, sizeof(data))
	CHECK_RESPONSE_DATA_SIZE(1 == m_nResponseDataSize, L"_ManageDevice: Unexpected length")

	return (ASCII_PLUS == *m_pResponseData ? HWLIB_S_OK : HWLIB_E_FAIL);
}

HWLIB_STDMETHODIMP Master::OpenLock(IN const word milliseconds) HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_LOCK, milliseconds / 100);
}

HWLIB_STDMETHODIMP Master::OpenLockInfiniteTime(void) HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_LOCK, 0xffff);
}

HWLIB_STDMETHODIMP Master::CloseLock(void) HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_LOCK, 0x0000);
}

HWLIB_STDMETHODIMP Master::EnableSiren(IN const word milliseconds) HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_SIREN, milliseconds / 100);
}

HWLIB_STDMETHODIMP Master::EnableSirenInfiniteTime() HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_SIREN, 0xffff);
}

HWLIB_STDMETHODIMP Master::DisableSiren(void) HWLIB_NOEXCEPT
{
	return _ManageDevice(PARAM_SIREN, 0x0000);
}

HWLIB_STDMETHODIMP Master::_SetParameters(IN const byte cell, IN const word val) HWLIB_NOEXCEPT
{
	const byte data[] = { PARAM_WRITE_PARAMETER, cell
		, Encode(val >> 12)
		, Encode(val >> 8)
		, Encode((byte)val >> 4)
		, Encode((byte)val)
	};
	SEND_COMMAND_WITH_DATA(CMD_RW_PARAMETER, RESP_TIMEOUT, data, sizeof(data))
	CHECK_RESPONSE_DATA_SIZE(1 == m_nResponseDataSize, L"_SetParameters: Unexpected length")

	return (ASCII_PLUS == *m_pResponseData ? HWLIB_S_OK : HWLIB_E_FAIL);
}

HWLIB_STDMETHODIMP Master::_GetParameters(IN const byte cell, OUT word& val) HWLIB_NOEXCEPT
{
	const byte data[] = { PARAM_READ_PARAMETER, cell };
	SEND_COMMAND_WITH_DATA(CMD_RW_PARAMETER, RESP_TIMEOUT, data, sizeof(data))
	CHECK_RESPONSE_DATA_SIZE(4 == m_nResponseDataSize, L"_GetParameters: Unexpected length")

	val =
	(Decode(m_pResponseData[0]) << 12) +
	(Decode(m_pResponseData[1]) << 8) +
	(Decode(m_pResponseData[2]) << 4) +
	Decode(m_pResponseData[3]);
	return HWLIB_S_OK;
}

HWLIB_STDMETHODIMP Master::GetOpticalSensorShortInterruptValue(OUT word& val) HWLIB_NOEXCEPT
{
	return _GetParameters('0', val);
}

HWLIB_STDMETHODIMP Master::SetOpticalSensorShortInterruptValue(IN const word val) HWLIB_NOEXCEPT
{
	return _SetParameters('0', val);
}

HWLIB_STDMETHODIMP Master::GetOpticalSensorLongInterruptValue(OUT word& val) HWLIB_NOEXCEPT
{
	return _GetParameters('1', val);
}

HWLIB_STDMETHODIMP Master::SetOpticalSensorLongInterruptValue(IN const word val) HWLIB_NOEXCEPT
{
	return _SetParameters('1', val);
}

HWLIB_STDMETHODIMP Master::GetOpticalSensorLevel(OUT byte& val) HWLIB_NOEXCEPT
{
	word v(0);
	const hresult hr = _GetParameters('2', v);
	val = (byte)(v & 0xff);
	return hr;
}

HWLIB_STDMETHODIMP Master::SetOpticalSensorLevel(IN const byte val) HWLIB_NOEXCEPT
{
	return _SetParameters('2', val);
}

HWLIB_STDMETHODIMP Master::GetOpticalSensorShortInterruptTimeout(OUT word& milliseconds) HWLIB_NOEXCEPT
{
	const hresult hr = _GetParameters('0', milliseconds);
	milliseconds /= 3;
	return hr;
}

HWLIB_STDMETHODIMP Master::SetOpticalSensorShortInterruptTimeout(IN const word milliseconds) HWLIB_NOEXCEPT
{
	return _SetParameters('0', milliseconds * 3);
}

HWLIB_STDMETHODIMP Master::GetOpticalSensorLongInterruptTimeout(OUT word& milliseconds) HWLIB_NOEXCEPT
{
	const hresult hr = _GetParameters('1', milliseconds);
	milliseconds /= 3;
	return hr;
}

HWLIB_STDMETHODIMP Master::SetOpticalSensorLongInterruptTimeout(IN const word milliseconds) HWLIB_NOEXCEPT
{
	return _SetParameters('1', milliseconds * 3);
}

} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#undef SEND_COMMAND_WITH_DATA
#undef SEND_COMMAND
