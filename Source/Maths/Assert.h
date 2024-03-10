#pragma once

namespace TV
{
	namespace Assert
	{
		inline void DoAssert(bool bCondition)
		{
			if (!bCondition)
			{
				__debugbreak();
			}
		}

		inline bool DoEnsure(bool bCondition)
		{
			DoAssert(bCondition);
			return bCondition;
		}
	}
}

#define check(condition) TV::Assert::DoAssert(condition)
#define ensure(condition) TV::Assert::DoEnsure(condition)