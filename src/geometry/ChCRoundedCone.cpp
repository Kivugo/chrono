//
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2006 Alessandro Tasora
// Copyright (c) 2013 Project Chrono
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
// and at http://projectchrono.org/license-chrono.txt.
//

//////////////////////////////////////////////////
//
//   ChCRoundedCone.cpp
//
// ------------------------------------------------
// ------------------------------------------------
///////////////////////////////////////////////////

#include <stdio.h>

#include "ChCRoundedCone.h"

namespace chrono {
namespace geometry {

// Register into the object factory, to enable run-time
// dynamic creation and persistence
ChClassRegister<ChRoundedCone> a_registration_ChRoundedCone;

void ChRoundedCone::StreamOUT(ChStreamOutBinary& mstream) {
    // class version number
    mstream.VersionWrite(1);

    // serialize parent class too
    ChGeometry::StreamOUT(mstream);

    // stream out all member data
    mstream << center;
    mstream << rad;
    mstream << radsphere;
}

void ChRoundedCone::StreamIN(ChStreamInBinary& mstream) {
    // class version number
    int version = mstream.VersionRead();

    // deserialize parent class too
    ChGeometry::StreamIN(mstream);

    // stream in all member data
    mstream >> center;
    mstream >> rad;
    mstream >> radsphere;
}

}  // END_OF_NAMESPACE____
}  // END_OF_NAMESPACE____
