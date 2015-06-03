//	Created by yuerain
#include "FilterSprite.h"
/*颜色混合
	R	G	B	A
R	1	0	0	0
G	0	1	0	0
B	0	0	1	0
A	0	0	0	1
*/
//没有特效
const GLfloat FilterSprite::ES_NONE[16] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};

//黑白化
const GLfloat FilterSprite::ES_BLACK_AND_WHITE[16] = {
	0.5f, 0.5f, 0.5f, -0.2f,
	0.5f, 0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, -0.2f,
	0.0f, 0.0f, 0.0f, 1.0f
};

//老照片
const GLfloat FilterSprite::ES_OLD_PHOTO[16] = {
	0.5f, 0.5f, 0.5f, -0.2f,
	0.5f, 0.5f, 0.5f, 0.2f,
	0.5f, 0.5f, 0.5f, -0.2f,
	0.0f, 0.1f, 0.0f, 1.0f
};

//反相
const GLfloat FilterSprite::ES_INVERT[16] = {
	-1.0f, 0.0f,  0.0f,  1.0f,
	0.0f,  -1.0f, 0.0f,  1.0f,
	0.0f,  0.0f,  -1.0f, 1.0f,
	0.0f,  0.0f,  0.0f,  1.0f
};

//灼伤（偏红）
const GLfloat FilterSprite::ES_BURNS[16] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.6f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.6f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f
};

//中毒（偏绿）
const GLfloat FilterSprite::ES_POISONING[16] = {
	0.6f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.6f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f
};

//寒冷（偏蓝）
const GLfloat FilterSprite::ES_COLD[16] = {
	0.6f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.6f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 1.0f
};

FilterSprite::FilterSprite(void)
{
	m_uSpriteFilter = Mat4::IDENTITY;
}

FilterSprite::~FilterSprite()
{

}

FilterSprite* FilterSprite::create()
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::create(const std::string& filename)
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::create(const std::string& filename, const Rect& rect)
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	if (sprite && sprite->initWithFile(filename, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::createWithTexture(Texture2D *pTexture)
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	Rect rect = Rect::ZERO;
	rect.size = pTexture->getContentSize();
	if (sprite && sprite->initWithTexture(pTexture, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	if (sprite && sprite->initWithTexture(texture, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
	FilterSprite *sprite = new (std::nothrow) FilterSprite();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

FilterSprite* FilterSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	return createWithSpriteFrame(frame);
}

bool FilterSprite::initWithTexture(Texture2D* pTexture, const Rect& tRect){
	do{
		CC_BREAK_IF(!Sprite::initWithTexture(pTexture, tRect));

		GLchar* pszFragSource =
			"#ifdef GL_ES														\n \
			precision mediump float;											\n \
			#endif																\n \
			uniform sampler2D u_texture;										\n \
			varying vec2 v_texCoord;											\n \
			varying vec4 v_fragmentColor;										\n \
			uniform mat4 fiterMat;												\n \
			void main(void)														\n \
			{																	\n \
				vec4 value = v_fragmentColor*texture2D(u_texture, v_texCoord);	\n \
				gl_FragColor = fiterMat*value;									\n \
			}";

		auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, pszFragSource);
		auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
		setGLProgramState(glprogramstate);

		CHECK_GL_ERROR_DEBUG();

		return true;
	} while (0);
	return false;
}

void  FilterSprite::setFilterMat(cocos2d::Mat4 matrixArray)
{
	m_uSpriteFilter = matrixArray;
}

void FilterSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(FilterSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void FilterSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgramState = getGLProgramState();
	glProgramState->setUniformMat4("fiterMat", m_uSpriteFilter);
	glProgramState->apply(transform);

	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	GL::bindTexture2D(_texture->getName());
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

#define kQuadSize sizeof(_quad.bl)
	size_t offset = (size_t)&_quad;

	// vertex
	int diff = offsetof(V3F_C4B_T2F, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(V3F_C4B_T2F, texCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(V3F_C4B_T2F, colors);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CHECK_GL_ERROR_DEBUG();
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
}