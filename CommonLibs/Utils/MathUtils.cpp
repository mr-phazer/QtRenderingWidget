#include "MathUtils.h"
#include <algorithm>
#include <DirectXMath.h>

namespace utils {

	constexpr float ToDegrees(float radians) {
		return radians * (180.0f / DirectX::XM_PI);
	}

	constexpr float ToRadians(float degrees) {
		return degrees * (DirectX::XM_PI / 180.0f);
	}

	//TODO: check that this actually workjs
	bool ProbablityFunction(float ratio)
	{
		return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1)) < ratio;
	}

	template <typename CONST_BUF_DATA_TYPE>
	CONST_BUF_DATA_TYPE Sign(CONST_BUF_DATA_TYPE in)
	{
		return (abs(in) / in);
	}

	/// <summary>
	///  Implemation of 'Polynomial rolling hash function'
	/// from
	/// https://cp-algorithms.com/string/string-hashing.html#calculation-of-the-hash-of-a-string 		
	/// </summary>
	/// <param name="s">string to be hased</param>
	/// <returns>64 bit hash</returns>
	static int64_t compute_string_hash(const std::wstring& s) {

		const int p = 31;
		const int m = static_cast<int>(1e9 + 9);
		int64_t hash_value = 0;
		int64_t p_pow = 1;
		for (char c : s) {
			hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
			p_pow = (p_pow * p) % m;
		}

		return hash_value;
	}
}