#include "soraguifont.hpp"

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"

#include "SoraStringConv.h"
#include "SoraFont.h"
#include "SoraColor.h"

namespace gcn
{
    SoraGUIFont::SoraGUIFont(const std::wstring& lpsFontName, int nFaceSize)
    {
		sora = sora::SoraCore::Instance();
        mSoraFont = (sora::SoraFont*)sora->createFont(lpsFontName, nFaceSize);
    }

    SoraGUIFont::~SoraGUIFont()
    {
		sora->releaseFont(mSoraFont);
        mSoraFont = NULL;
		//delete mSoraFont;
    }

    sora::SoraFont *SoraGUIFont::getFont() const
    {
        return mSoraFont;
    }

    int SoraGUIFont::getWidth(const std::string &text) const
    {
		if(text.size() > 0)
            return mSoraFont?(static_cast<int>(mSoraFont->getStringWidth(sora::s2ws(text).c_str())) + 1):0;
		return 1;
    }

	int SoraGUIFont::getWidthW(const std::wstring &text) const
	{
		if(text.size() > 0)
			return mSoraFont?(static_cast<int>(mSoraFont->getStringWidth(text.c_str())) + 1):0;
		return 1;
	}
  
    int SoraGUIFont::getWidth(char character) const
    {
        char buf[2];

        sprintf(buf, "%c", character);

		if(mSoraFont)
			return (int)mSoraFont->getStringWidth(sora::s2ws(buf).c_str());
		return 0;
    }

    int SoraGUIFont::getHeight() const
    {
        return  mSoraFont?static_cast<int>(mSoraFont->getHeight()+2):0;
    }

    int SoraGUIFont::getStringIndexAt(const std::string& text, int x) const
    {
        unsigned int i;
        int size = 0;

        for (i = 0; i < text.size(); ++i)
        {
            size += getWidth(text.at(i));

            if (size > x)
            {
                return i;
            }
        }

        return text.size();
    }

	int SoraGUIFont::getStringIndexAtW(const std::wstring& text, int x) const
	{
		unsigned int i;
		int size = 0;

		for (i = 0; i < text.size(); ++i)
		{
			size += getWidthW(std::wstring(1,text.at(i)));

			if (size > x)
			{
				return i;
			}
		}

		return text.size();
	}

    void SoraGUIFont::drawString(Graphics *graphics, const std::string &text, int x, int y)
    {
       /* ClipRectangle const &top = graphics->getCurrentClipArea();

        x += top.xOffset;
        y += top.yOffset;*/

        Color color = graphics->getColor();
		
		if(mSoraFont) {
			mSoraFont->setColor(CARGB(color.a, color.r, color.g, color.b));
			mSoraFont->render((float)x, (float)y, sora::s2ws(text).c_str());
		}
    }

	void SoraGUIFont::drawStringW(Graphics *graphics, const std::wstring &text, int x, int y) {
		Color color = graphics->getColor();

		if(mSoraFont) {
			mSoraFont->setColor(CARGB(color.a, color.r, color.g, color.b));
			mSoraFont->render((float)x, (float)y, text.c_str());
		}
	}
}
