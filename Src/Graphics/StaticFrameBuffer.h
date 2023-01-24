#pragma once

#include "Graphics/Color.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

#include <string>

namespace Graphics
{
	template<size_t Width, size_t Height>
	class StaticFrameBuffer : public FrameBuffer
	{
	public:
		StaticFrameBuffer(): m_backing()
		{

		}

		virtual size_t GetHeight() const override
		{
			return Height;
		}

		virtual size_t GetWidth() const override
		{
			return Width;
		}

		virtual Color* GetBuffer() override
		{
			return &m_backing[0][0];
		}

	private:
		Color m_backing[Height][Width];
	};
}
