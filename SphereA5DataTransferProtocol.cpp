#include "stdafx.h"
#include "SphereA5DataTransferProtocol.h"


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
#define HEADER_SIZE 3

static const dword SPHERE_TIMEOUT = 500;

HWLIB_STDMETHODIMP DataTransferProtocol::SendRequest(IN const byte cmdId, IN const byte* const pb, IN const uint_t size) HWLIB_NOEXCEPT
{
	_ASSERT_EXPR((NULL == pb && 0 == size) || (NULL != pb && 0 < size), L"Wrong data size or null pointer");

	m_vec.resize(1 + 1 + 1 + size + 1 + 1); // STX;LEN;CMD;DATA;CRC;ETX;
	byte* p = &m_vec[0];
	const byte LEN = (byte)(0x21 + 1 + size);
	
	*p++ = ASCII_STX;
	*p++ = LEN;
	*p++ = cmdId;
	if (size) memcpy(p, pb, size), p += size; // data bytes

	byte crc = (byte)(LEN + cmdId);
	for (size_t i = 0; i < size; ++i) crc += pb[i];

	if (crc < 0x20) crc += 0x20;
	*p++ = crc;
	*p++ = ASCII_ETX;

	return m_pDeviceConnection->Write(&m_vec[0], m_vec.size(), m_nCmdTimeout);
}

HWLIB_STDMETHODIMP DataTransferProtocol::ReadResponse(IN const byte cmdId) HWLIB_NOEXCEPT
{
	hresult hr;

	m_vec.resize(HEADER_SIZE); // STX;LEN;CMD
	//const dword dwStart = ::GetTickCount();
	if (FAILED(hr = m_pDeviceConnection->Read(&m_vec[0], HEADER_SIZE, m_nCmdTimeout))) return hr;
	//const dword dwStop = ::GetTickCount();
	//_RPT1(_CRT_WARN, "DataTransferProtocol::ReadResponse(4): Timeout=%d\n", dwStop - dwStart);

	byte readResp = 0;
	do {
		if (FAILED(hr = m_pDeviceConnection->Read(&readResp, 1, 100))) return hr;
		m_vec.push_back(readResp);
	} while (ASCII_ETX != readResp);

	// считываем CRC
	byte readCrc = m_vec[m_vec.size() - 2];
	byte curCrc = (byte)CRC::CRC_ADD(&m_vec[1], m_vec.size() - 3);
	if (curCrc < 0x20) curCrc += 0x20;
	if (curCrc != readCrc) {
		return HWLIB_E_INVALID_CHECKSUM;
	}

	if (cmdId != m_vec[2])
		return HWLIB_E_FAIL;

	if (0 < (m_nResponseDataSize = m_vec.size() - HEADER_SIZE - 2))
		m_pResponseData = &m_vec[HEADER_SIZE];

	return HWLIB_S_OK;
}

HWLIB_STDMETHODIMP DataTransferProtocol::SendCommand(IN const byte cmdId, IN const byte* const pb, IN const uint_t size) HWLIB_NOEXCEPT
{
	_ASSERT(((NULL != pb) && (0 < size)) || ((NULL == pb) && (0 == size)));
	_ASSERT(NULL != m_pDeviceConnection);

	m_nCmdTimeout = SPHERE_TIMEOUT;

	hresult hr = HWLIB_E_FAIL;
	uint_t i = 0;
	do {
		if (SUCCEEDED(hr = SendRequest(cmdId, pb, size)))
		{
			m_nResponseDataSize = 0;
			m_pResponseData = NULL;
			if (SUCCEEDED(hr = ReadResponse(cmdId)))
			{
				break;
			}
			else
				Trace(_T("DataTransferProtocol::SendCommand(): ReadResponse failed: 0x%08X"), hr);
		} else
			Trace(_T("DataTransferProtocol::SendCommand(): SendRequest failed: 0x%08X"), hr);
	} while (++i < m_nMaxTryCount);
	//////////////////////////////////////////////////////////////////////////

	if (m_nMaxTryCount <= i)
		return hr;

	return HWLIB_S_OK;
}

} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers
