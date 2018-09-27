#ifndef SAUCE_FONT_H
#define SAUCE_FONT_H

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

class SpriteBatch;

class FontLoader;
class Texture2D;

struct SAUCE_API CharDescr
{
	CharDescr() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0), page(0) {}

	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;

	std::vector<int> kerningPairs;
};

enum FontTextEncoding
{
	NONE,
	UTF8,
	UTF16
};


enum FontAlign
{
	FONT_ALIGN_LEFT,
	FONT_ALIGN_CENTER,
	FONT_ALIGN_RIGHT,
	FONT_ALIGN_JUSTIFY
};

class SAUCE_API Font
{
public:
	Font(const string& filepath);
	~Font();

	void setTextEncoding(FontTextEncoding encoding);
	void setDepth(const float depth) { m_depth = depth;  }
	void setColor(const Color &color) { m_color = color; }
	Color getColor() { return m_color; }

	float getStringWidth(const string &text, int count = 0);
	float getStringHeight(const string &text);

	/**
	 * \fn	void Font::draw(SpriteBatch *spriteBatch, float x, float y, const string &text, FontAlign mode = FONT_ALIGN_LEFT);
	 *
	 * \brief	Draws \p text to the screen.
	 *
	 * \param [in,out]	spriteBatch	The sprite batch to draw the text with.
	 * \param	x				   	The x coordinate.
	 * \param	y				   	The y coordinate.
	 * \param	text			   	The text to draw.
	 * \param	mode			   	The font alignment mode.
	 */

	void draw(SpriteBatch *spriteBatch, float x, float y, const string &text, FontAlign mode = FONT_ALIGN_LEFT);

	/**
	 * \fn	void Font::draw(SpriteBatch *spriteBatch, const Vector2F &pos, const string &text, FontAlign mode = FONT_ALIGN_LEFT)
	 *
	 * \brief	Draws \p text to the screen.
	 *
	 * \param [in,out]	spriteBatch	The sprite batch to draw the text with.
	 * \param	pos				   	The position.
	 * \param	text			   	The text to draw.
	 * \param	mode			   	The font alignment mode.
	 */

	void draw(SpriteBatch *spriteBatch, const Vector2F &pos, const string &text, FontAlign mode = FONT_ALIGN_LEFT) { draw(spriteBatch, pos.x, pos.y, text, mode); }

	/**
	 * \fn	void Font::drawBox(SpriteBatch *spriteBatch, float x, float y, float width, const string &text, int count, FontAlign mode = FONT_ALIGN_LEFT);
	 *
	 * \brief	Draw text to fit within a box. The text will be split when a line exceedes \p width.
	 *
	 * \param [in,out]	spriteBatch	The sprite batch to draw the text to.
	 * \param	x				   	The x coordinate.
	 * \param	y				   	The y coordinate.
	 * \param	width			   	The max width of a line.
	 * \param	text			   	The text to draw.
	 * \param	count			   	Number of characters of \p text to draw.
	 * \param	mode			   	The font alignment mode.
	 * \todo Setting \p mode to anything other than FONT_ALIGN_LEFT does nothing at the moment.
	 */

	void drawBox(SpriteBatch *spriteBatch, float x, float y, float width, const string &text, int count = -1, FontAlign mode = FONT_ALIGN_LEFT);
	void drawBox(SpriteBatch *spriteBatch, const Vector2F &pos, float width, const string &text, int count = -1, FontAlign mode = FONT_ALIGN_LEFT) { drawBox(spriteBatch, pos.x, pos.y, width, text, count, mode); }

	struct BoxLine
	{
		string text;
		float xoffset;
		float yoffset;
		float spacing;
		enum BreakType { NONE, SOFT, HARD } breakType;

	};

	list<BoxLine> getBoxLines(float width, const string &text, int count = -1, FontAlign mode = FONT_ALIGN_LEFT);

	void drawInternal(SpriteBatch *spriteBatch, float x, float y, const string &text, int count, float spacing = 0);

	void setHeight(float h);
	float getHeight() const;

	float getBottomOffset() const;
	float getTopOffset() const;

protected:
	friend class FontLoader;

	float adjustForKerningPairs(int first, int second);
	CharDescr *getChar(int id);

	int getTextLength(const string &text);
	int getTextChar(const string &text, int pos, int *nextPos = 0);
	int findTextChar(const char *text, int start, int length, int ch);

	short m_fontHeight; // total height of the font
	short m_base;       // y of base line
	short m_scaleW;
	short m_scaleH;
	CharDescr m_defChar;
	bool m_hasOutline;

	// Technically, position, text and alignment mode can be a part of FontInstance
	float m_scale; // Instance
	float m_depth; // Instance
	Color m_color; // Instance
	FontTextEncoding m_encoding;

	map<int, CharDescr*> m_chars;
	vector<shared_ptr<Texture2D>> m_pages;
};

template SAUCE_API class shared_ptr<Font>;

class FontResourceDesc : public ResourceDesc
{
public:
	FontResourceDesc(const string &name, const string &path, const bool premultiplyAlpha) :
		ResourceDesc(RESOURCE_TYPE_FONT, name),
		m_premultiplyAlpha(premultiplyAlpha),
		m_path(path)
	{
	}

	void *create() const;

private:
	const bool m_premultiplyAlpha;
	const string m_path;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_FONT_H
