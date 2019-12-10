#pragma once
#if !defined(__SphereA5_INTERFACE_H__)
#define __SphereA5_INTERFACE_H__

#pragma pack(push,8)
#pragma warning(push)
#pragma warning(disable : 4100)


namespace Controllers {
namespace CompanyName {
namespace SphereA5
{
/**
 * Интерфейс протокола SphereA5
 */
struct SphereA5Interface
{
	virtual ~SphereA5Interface(void) HWLIB_NOEXCEPT {}
}; // struct SphereA5Interface
} // namespace SphereA5
} // namespace CompanyName
} // namespace Controllers

#pragma warning(pop)
#pragma pack(pop)
#endif // __SphereA5_INTERFACE_H__
