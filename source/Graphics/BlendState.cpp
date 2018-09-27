#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

BlendState::BlendState(const Preset preset)
{
	switch(preset)
	{
	case PRESET_ALPHA_BLEND:
		m_src = m_alphaSrc = BLEND_SRC_ALPHA;
		m_dst = m_alphaDst = BLEND_ONE_MINUS_SRC_ALPHA;
		break;
	case PRESET_OPAQUE:
	case PRESET_ADDITIVE:
		m_src = m_alphaSrc = BLEND_SRC_ALPHA;
		m_dst = m_alphaDst = BLEND_ONE;
		break;
	case PRESET_MULTIPLY:
		m_src = m_alphaSrc = BLEND_DST_COLOR;
		m_dst = m_alphaDst = BLEND_ZERO;
		break;
	case PRESET_PREMULTIPLIED_ALPHA:
		m_src = BLEND_ONE; m_alphaSrc = BLEND_ONE;
		m_dst = BLEND_ONE_MINUS_SRC_ALPHA; m_alphaDst = BLEND_ONE_MINUS_SRC_ALPHA;
		break;
	}
}

BlendState::BlendState(const BlendFactor src, const BlendFactor dst) :
	m_src(src),
	m_dst(dst),
	m_alphaSrc(src),
	m_alphaDst(dst)
{
}

BlendState::BlendState(const BlendFactor csrc, const BlendFactor cdst, const BlendFactor asrc, const BlendFactor adst) :
	m_src(csrc),
	m_dst(cdst),
	m_alphaSrc(asrc),
	m_alphaDst(adst)
{
}

END_SAUCE_NAMESPACE
