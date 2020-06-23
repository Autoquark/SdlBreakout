#pragma once
#include "stdafx.h"
#include <optional>
#include "Vector2.h"
#include "Contact.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<Vector2>(const Vector2& t)
			{
				std::wstring value = L"(";
				value.append(std::to_wstring(t.x));
				value.append(L",");
				value.append(std::to_wstring(t.y));
				value.append(L")");

				return value;
			}

			template<> inline std::wstring ToString<Contact>(const Contact& t)
			{
				std::wstring value = L"(point: ";
				value.append(ToString(t.point));
				value.append(L", centroid: ");
				value.append(ToString(t.centroid));
				value.append(L", normal: ");
				value.append(ToString(t.normal));
				value.append(L")");
				return value;
			}


			template<typename T> inline std::wstring ToString(const std::optional<T>& t)
			{
				return t.has_value() ? ToString(t.value()) : L"null";
			}
		}
	}
}