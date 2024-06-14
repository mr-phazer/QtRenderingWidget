#pragma once

#include <string>

namespace utils {

	constexpr float ToDegrees(float radians);
	constexpr float ToRadians(float degrees);

	/// <summary>
	/// Return true at a certain pronanality
	/// if input is "0.7" there is a 70% change that it will return true
	/// </summary>
	/// <param name="propbabality">ratio [0;1]</param>
	/// <returns></returns>
	bool ProbablityFunction(float ratio);

	template <typename CONST_BUF_DATA_TYPE>
	CONST_BUF_DATA_TYPE Sign(CONST_BUF_DATA_TYPE in);

	/// <summary>
	///  Implemation of 'Polynomial rolling hash function'
	/// from
	/// https://cp-algorithms.com/string/string-hashing.html#calculation-of-the-hash-of-a-string 		
	/// </summary>
	/// <param name="s">string to be hased</param>
	/// <returns>64 bit hash</returns>
	static int64_t compute_string_hash(const std::wstring& s);
}