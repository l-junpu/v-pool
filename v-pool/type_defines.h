#pragma once
#ifndef TYPE_DEFINES
#define TYPE_DEFINES

namespace Type
{
	struct NoCopy
	{
		NoCopy(void) = default;
		~NoCopy(void) = default;
		NoCopy(const NoCopy&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;
	};

	struct NoMove
	{
		NoMove(void) = default;
		~NoMove(void) = default;
		NoMove(const NoMove&) = default;
		NoMove(NoMove&&) = delete;
		NoMove& operator=(NoMove&&) = delete;
	};

	template <typename T>
	struct Singleton : NoCopy, NoMove
	{
		static T& Instance() noexcept
		{
			static T t;
			return t;
		}

		T& operator()(void) noexcept
		{
			static T t;
			return t;
		}
	};
}

#endif