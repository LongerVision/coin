/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLTextureImageElement.h>

#include <Inventor/SbName.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SoImageInterface.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//$ BEGIN TEMPLATE ElementSource(SoGLTextureImageElement)

/*!
  \var SoGLTextureImageElement::classTypeId

  This is the static class type identifier for the
  SoGLTextureImageElement class.
*/

SoType SoGLTextureImageElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLTextureImageElement::getClassTypeId(void)
{
  return SoGLTextureImageElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLTextureImageElement class.
*/
void *
SoGLTextureImageElement::createInstance(void)
{
  return (void *) new SoGLTextureImageElement;
}

/*!
  \var SoGLTextureImageElement::classStackIndex

  This is the static state stack index for the
  SoGLTextureImageElement class.
*/
int SoGLTextureImageElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLTextureImageElement class.
*/
int
SoGLTextureImageElement::getClassStackIndex(void)
{
  return SoGLTextureImageElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data in the
  SoGLTextureImageElement class.
*/

void
SoGLTextureImageElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLTextureImageElement)
  assert(SoGLTextureImageElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLTextureImageElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLTextureImageElement",
                       &SoGLTextureImageElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLTextureImageElement::classStackIndex =
      createStackIndex(SoGLTextureImageElement::classTypeId);
  }
  else {
    SoGLTextureImageElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLTextureImageElement::createInstance(void)
*/

SoGLTextureImageElement::SoGLTextureImageElement()
{
  setTypeId(SoGLTextureImageElement::classTypeId);
  setStackIndex(SoGLTextureImageElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLTextureImageElement::~SoGLTextureImageElement(void)
{
}

//! FIXME: write doc.

void
SoGLTextureImageElement::init(SoState * state)
{
  inherited::init(state);
  this->glimage = NULL;
}

//! FIXME: write doc.

void
SoGLTextureImageElement::pop(SoState * state,
                             const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLTextureImageElement *prev = (SoGLTextureImageElement*)
    prevTopElement;
  
  if (prev->glimage != NULL) {
    prev->glimage->apply();
    if (prev->model == DECAL) {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }
    else if (prev->model == MODULATE) {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
      glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, prev->blendColor.getValue());      
    }                
  }
}

//! FIXME: write doc.

void
SoGLTextureImageElement::set(SoState * const state, SoNode * const node,
                             SoGLImage *glimage, const Model model,
                             const SbColor &blendColor)
{
  SoGLTextureImageElement *elem = (SoGLTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (glimage) {
    const SoImageInterface *image = glimage->getImage();
    // keep SoTextureImageElement "up-to-date"
    inherited::set(state, node,
                   image->getSize(),
                   image->getNumComponents(),
                   image->getDataPtr(),
                   glimage->shouldClampS() ?
                   SoTextureImageElement::CLAMP :
                   SoTextureImageElement::REPEAT,
                   glimage->shouldClampT() ?
                   SoTextureImageElement::CLAMP :
                   SoTextureImageElement::REPEAT,
                   model,
                   blendColor);
    
    elem->glimage = glimage;
    glimage->apply();
    if (model == DECAL) {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }
    else if (model == MODULATE) {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else {
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
      glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blendColor.getValue());      
    }                
  }
  else {
    elem->glimage = NULL;
    inherited::setDefault(state, node);
  }
}
