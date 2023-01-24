#pragma once

#include "Graphics/Color.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

#include <Windows.h>
#include <string>
#include <cstddef>

class WindowsFrameBuffer : public Graphics::FrameBuffer
{
public:
	WindowsFrameBuffer(const size_t Width, const size_t Height) :
		m_width(Width),
		m_height(Height),
		m_buffer()
	{

	}

	void Init()
	{
		int BitmapMemorySize = (m_width * m_height) * sizeof(Graphics::Color);
		m_buffer = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		Assert(m_buffer);
	}

	virtual size_t GetHeight() const override
	{
		return m_height;
	}

	virtual size_t GetWidth() const override
	{
		return m_width;
	}

	virtual Graphics::Color* GetBuffer() override
	{
		return (Graphics::Color*)m_buffer;
	}

private:
	size_t m_width;
	size_t m_height;
	void* m_buffer;
};
