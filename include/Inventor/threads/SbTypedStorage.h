#ifndef COIN_SBTYPEDSTORAGE_H
#define COIN_SBTYPEDSTORAGE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/threads/storage.h>

// *************************************************************************

template <class Type>
class SbTypedStorage {
public:
  SbTypedStorage(unsigned int size) { this->storage = cc_storage_construct(size); }
  SbTypedStorage(unsigned int size, void (*constr)(void *), void (*destr)(void *))
    { this->storage = cc_storage_construct_etc(size, constr, destr); }
  ~SbTypedStorage(void) { cc_storage_destruct(this->storage); }

  Type get(void) { return (Type) cc_storage_get(this->storage); }

private:
  cc_storage * storage;
};

// *************************************************************************

#endif // ! COIN_SBTYPEDSTORAGE_H
