/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SbProjector SbProjector.h Inventor/projectors/SbProjector.h
  \brief The SbProjector class is the abstract base projector class.
  \ingroup projectors

  Projectors are used in the Coin library for mapping 2D coordinates
  (typically from the position of the mouse cursor in the rendering
  window) to 3D "world" coordinates.

  Mapping 2D coordinates to 3D coordinates is something which is done
  extensively in the dragger classes, to provide the user with a
  convenient and natural way of interacting with the 3D geometry of
  scenes.

  The application programmer should normally not need to care about
  the projector classes, unless there are special needs in the
  application.

  \sa SoDragger
*/


#include <Inventor/projectors/SbProjector.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbPlane.h>
#include <assert.h>

/*!
  \fn SbProjector::~SbProjector()

  Destructor is protected, as this is an abstract class.
 */

/*!
  \fn virtual SbVec3f SbProjector::project(const SbVec2f & point)

  Project the 2D \a point from normalized viewport coordinates to a 3D
  point. The mapping will be done in accordance with the type of the
  projector.
 */
/*!
  \fn virtual SbProjector * SbProjector::copy(void) const

  Construct and return a copy of this projector. The caller is
  responsible for destructing the new instance.
 */

/*!
  \var SbProjector::viewVol

  The viewVol definition.
*/
/*!
  \var SbProjector::worldToWorking

  The matrix which converts from world coordinates to coordinates in
  the projector's local coordinate system.
*/
/*!
  \var SbProjector::workingToWorld

  The matrix which converts from coordinates in the projector's local
  coordinate system to world coordinates.
*/


/*!
  The constructor initializes the workingspace matrix to an identity
  matrix.
 */
SbProjector::SbProjector(void)
{
  this->worldToWorking.makeIdentity();
  this->workingToWorld.makeIdentity();
}

/*!
  Set the viewing volume the projections will take place in.

  \sa getViewVolume()
 */
void
SbProjector::setViewVolume(const SbViewVolume & vol)
{
  this->viewVol = vol;
}

/*!
  Return the current viewing volume used by the projections.

  \sa setViewVolume()
 */
const SbViewVolume &
SbProjector::getViewVolume(void) const
{
  return this->viewVol;
}

/*!
  Sets the matrix used for converting from the projector's coordinate
  system to the world coordinate system.
 */
void
SbProjector::setWorkingSpace(const SbMatrix & space)
{
  this->workingToWorld = space;
  this->worldToWorking = space.inverse();
}

/*!
  Returns projector-to-world matrix.

  \sa setWorkingSpace()
 */
const SbMatrix &
SbProjector::getWorkingSpace(void) const
{
  return this->workingToWorld;
}

/*!
  From the 2D \a point in normalized screenspace coordinates,
  calculate the line passing through the scene.

  Typically used for tracking intersection points for the mouse
  cursor.
 */
SbLine
SbProjector::getWorkingLine(const SbVec2f & point) const
{
  SbLine l;
  this->viewVol.projectPointToLine(point, l);
  this->worldToWorking.multLineMatrix(l, l);
  return l;
}

/*!  
  Finds the unit cube vanishing distance for the current projector
  view volume.  The view volume must be a perspective view
  volume.  
  
  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 1.1.0
*/
float 
SbProjector::findVanishingDistance(void) const
{
  const SbViewVolume & vv = this->viewVol;

  // FIXME: find a proper algorithm to calculate the vanishing
  // distance. Now we just use an incremental algorithm to detect when
  // the projected box is less than one pixel on a screen with height
  // = 1024 pixels. pederb, 2001-10-11
  assert(vv.getProjectionType() == SbViewVolume::PERSPECTIVE);
  float depth = vv.getDepth();
  
  // used to break out if we get too many iterations. Will probably
  // never happen, but it's here just in case something bad happens.
  int cnt = 0; 
  SbBox3f box(-0.5f, -0.5f, depth, 0.5f, 0.5f, depth);
  SbVec2f siz = vv.projectBox(box);
  while (cnt < 32 && (siz[1] > (1.0f / 1024.0f))) {
    depth *= 2.0f;
    box.getMin()[2] = depth;
    box.getMax()[2] = depth;
    siz = vv.projectBox(box);
    cnt++;
  }
  return depth;
}

/*! 
  Verifies that \a projpt is a valid projection for the current view
  volume. For perspective view volumes, it does this by checking that
  the projection point is in front of the eye plane. For orthographic
  projections, this method always returns \e TRUE.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 1.1.0
*/
SbBool 
SbProjector::verifyProjection(const SbVec3f & projpt) const
{
  if (this->viewVol.getProjectionType() == SbViewVolume::PERSPECTIVE) {
    SbPlane eyeplane = this->viewVol.getPlane(0.0f); 
    SbVec3f wrld;
    this->workingToWorld.multVecMatrix(projpt, wrld);
    if (eyeplane.isInHalfSpace(wrld)) return FALSE;
  }
  return TRUE;
}

