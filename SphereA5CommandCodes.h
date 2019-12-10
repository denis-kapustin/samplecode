#pragma once
#if !defined(__SphereA5_COMMAND_CODES_H__)
#define __SphereA5_COMMAND_CODES_H__

#pragma pack(push,8)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
// Здесь можно описать коды команд
static const byte CMD_REQUEST_STATUS     = 'S'; 
static const byte CMD_REQUEST_BOARD_INFO = 'I'; 
static const byte CMD_READ_OPTICAL_SENSOR_EVENT_COUNTER = 'Q';
static const byte CMD_RESET              = 'Z';
static const byte CMD_CLEAR_RESET_FLAG   = 'C';
static const byte CMD_CLEAR_OPTICAL_SENSOR_INTERRUPTED_EVENTS = 'c';

static const byte CMD_MANAGE_DEVICE = 'K';
static const byte PARAM_LOCK        = '0';
static const byte PARAM_SIREN       = '1';

static const byte CMD_RW_PARAMETER      = 'P';
static const byte PARAM_READ_PARAMETER  = 'R';
static const byte PARAM_WRITE_PARAMETER = 'W';

} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#pragma pack(pop)
#endif // __SphereA5_COMMAND_CODES_H__
