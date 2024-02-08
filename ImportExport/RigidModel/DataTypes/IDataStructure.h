#pragma once

struct IDataStructure
{
	/// <summary>
	/// Checks fields in the derived struct, to see if any have invalid values,
	/// </summary>
	/// <returns>false if there are invalid values, else trye</returns>
	virtual bool IsContentValid() = 0;
};
