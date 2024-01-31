#pragma once

#pragma once
#include <string>
#include <string.h>
#include <unordered_map>

#include "..\Helpers\NoCaseUMapCompare.h"

/// <summary>
///  Map using  case insensitive strings as a keys
/// For string
/// </summary>
/// <typeparam name="VALUE"></typeparam>
template <typename VALUE>
using StringkeyMap
=  std::unordered_map<
	std::wstring,
    VALUE,
	CaseInsensitiveUnorderedMapComparer::Hash,
	CaseInsensitiveUnorderedMapComparer::Compare>;

/// <summary>
///  Map using  case insensitive strings as a keys
/// For wstring
/// </summary>
/// <typeparam name="VALUE"></typeparam>
template <typename VALUE>
using WStringkeyMap = std::unordered_map<
	std::wstring,
    VALUE,
	CaseInsensitiveUnorderedMapComparer::Hash,
	CaseInsensitiveUnorderedMapComparer::Compare>;



