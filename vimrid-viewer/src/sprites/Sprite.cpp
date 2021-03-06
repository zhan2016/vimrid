/* TODO: Short description of file.
 *
 * TODO: Write detailed information.
 *
 * Created: 20-Apr-2009
 * Author: Nick Bolton (njb4@aber.ac.uk)
 */

#include "Sprite.h"
#include <GL/gl.h>
#include "../VimridApplication.h"

#define _Sprite_mParent NULL
#define _Sprite_mSize 10, 10
#define _Sprite_mRenderIndex 0
#define _Sprite_mVisible 0
#define _Sprite_mShowPending true
#define _Sprite_mHidePending false

namespace vimrid
{
namespace sprites
{

Sprite::Sprite()
:
	mParent(_Sprite_mParent),
	mSize(_Sprite_mSize),
	mRenderIndex(_Sprite_mRenderIndex),
	mVisible(_Sprite_mVisible),
	mShowPending(_Sprite_mShowPending),
	mHidePending(_Sprite_mHidePending)
{
}

Sprite::Sprite(const VUInt32 &renderIndex)
:
	mParent(_Sprite_mParent),
	mSize(_Sprite_mSize),
	mRenderIndex(renderIndex),
	mVisible(_Sprite_mVisible),
	mShowPending(_Sprite_mShowPending),
	mHidePending(_Sprite_mHidePending)
{
}

Sprite::~Sprite()
{
}

void Sprite::Show()
{
	mShowPending = true;
	mHidePending = false;
}

void Sprite::Hide()
{
	mHidePending = true;
	mShowPending = false;
}

void Sprite::TriggerUpdate()
{
	/* Some virtual code calculates the position during update,
	 * and some code requires values generated by this code during
	 * the process of showing, so Update must be called first,
	 * before the sprite is shown.
	 */

	VBool wasShown = false;
	if (mShowPending)
	{
		mShowPending = false;
		mVisible = true;
		wasShown = true;
	}

	VBool wasHidden = false;
	if (mHidePending)
	{
		mHidePending = false;
		mVisible = false;
		wasHidden = true;
	}

	Update();

	if (wasShown)
	{
		OnShown();
	}

	if (wasHidden)
	{
		OnHidden();
	}
}

void Sprite::TriggerRender()
{
	Render();
}

VBool Sprite::IsVisible()
{
	if (mParent != NULL)
	{
		// Only consider visible if parent is also visible.
		return mVisible && mParent->IsVisible();
	}
	else
	{
		return mVisible;
	}
}

VBool Sprite::CollidesWith(const VPoint3f &position) const
{
	VBool pointPastPoint = position >= mPosition;
	VBool pointWithinSize = position <= (mPosition + mSize);
	return (pointPastPoint && pointWithinSize);
}

VBool Sprite::CollidesWith(const VPoint2f &position) const
{
	VPoint3f p3f;
	p3f = position;
	return CollidesWith(p3f);
}

VBool Sprite::CollidesWith(const Sprite &sprite) const
{
	VBool pointPastPoint = (sprite.GetPosition() >= mPosition);
	VBool pointWithinSize = sprite.GetPosition() <= (mPosition + mSize);
	VBool sizePastPoint = ((sprite.GetPosition() + sprite.GetSize()) >= mPosition);
	VBool sizeWithinSize = ((sprite.GetPosition() + sprite.GetSize()) <= (mPosition + mSize));
	return (pointPastPoint && pointWithinSize) || (sizePastPoint && sizeWithinSize);
}

const VimridUtility &Sprite::GetUtility() const
{
	return VimridUtility::Get();
}

void Sprite::TranslateToPosition(const VPoint3f &position) const
{
	glTranslatef(position.X, position.Y, position.Z);
}

void Sprite::TranslateToPosition(const VPoint2f &position) const
{
	// HACK: Cannot cast to 3-point, so copy instead.
	VPoint3f p3f;
	p3f = position;
	TranslateToPosition(p3f);
}

void Sprite::DrawVertex(VPoint3f point) const
{
	glVertex3f(point.X, point.Y, point.Z);
}

void Sprite::ChangeColour(const VColour4F &colour) const
{
	glColor4f(colour.Red, colour.Green, colour.Blue, colour.Alpha);
}

void Sprite::OnShown()
{
	if (HasParent())
	{
		GetParent().OnChildShown(*this);
	}
}

void Sprite::OnHidden()
{
	if (HasParent())
	{
		GetParent().OnChildHidden(*this);
	}
}

/*void Sprite::Hide()
{
	//mVisible = false;
	mHidePending = true;
	if (HasParent())
	{
		GetParent().OnChildHidden(*this);
	}
}

void Sprite::Show()
{
	//mVisible = true;
	mShowPending = true;
	if (HasParent())
	{
		GetParent().OnChildShown(*this);
	}
}*/

}
}
