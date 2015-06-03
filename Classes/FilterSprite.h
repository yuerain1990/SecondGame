//	Created by yuerain
//	精灵滤镜效果FilterSprite
#ifndef __FilterSpirte_h
#define __FilterSpirte_h

#include "cocos2d.h"

USING_NS_CC;


class FilterSprite : public Sprite{

public:

	static const GLfloat ES_NONE[16];				//没有特效
	static const GLfloat ES_BLACK_AND_WHITE[16];	//黑白化
	static const GLfloat ES_OLD_PHOTO[16];			//老照片
	static const GLfloat ES_INVERT[16];				//反相
	static const GLfloat ES_BURNS[16];				//灼伤(偏红)
	static const GLfloat ES_POISONING[16];			//中毒(偏绿)
	static const GLfloat ES_COLD[16];				//寒冷(偏蓝)

	FilterSprite();
	virtual ~FilterSprite();

	static FilterSprite* create();
	static FilterSprite* create(const std::string& filename);
	static FilterSprite* create(const std::string& filename, const Rect& rect);


	static FilterSprite* createWithTexture(Texture2D *pTexture);
	static FilterSprite* createWithTexture(Texture2D *pTexture, const Rect& rect, bool rotated = false);
	static FilterSprite* createWithSpriteFrame(SpriteFrame *pSpriteFrame);
	static FilterSprite* createWithSpriteFrameName(const std::string& spriteFrameName);

	bool initWithTexture(Texture2D* pTexture, const Rect& tRect);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	void setFilterMat(cocos2d::Mat4 matrixArray);
	//to-do 提供一个设置滤镜的方法
protected:
	CustomCommand _customCommand;
private:
	cocos2d::Mat4   m_uSpriteFilter;
};

#endif