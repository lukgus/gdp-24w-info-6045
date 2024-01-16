#pragma once

class Window
{
public:
	Window(const char* title, int width, int height) 
		: m_Title(title)
		, m_Width(width)
		, m_Height(height)
	{ }
	~Window() { }

	const char* m_Title;
	int m_Width;
	int m_Height;
};