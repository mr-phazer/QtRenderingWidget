#pragma once

struct IDataStructure
{
	// TODO: maybe make, so the TFactory itself calls this on "Create()": and each factory has a standard "is invalid" message, and throws
	/// <summary>
	/// Checks fields in the derived struct, to see if any have invalid values,
	/// </summary>
	/// <returns>false if there are invalid values, else trye</returns>
	virtual bool IsContentValid() = 0;
};
