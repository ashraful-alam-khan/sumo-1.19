/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.dev/sumo
// Copyright (C) 2001-2023 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    NIVissimSingleTypeParser_Haltestellendefinition.cpp
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Wed, 18 Dec 2002
///
//
/****************************************************************************/
#include <config.h>

#include <iostream>
#include <utils/common/StringUtils.h>
#include <netimport/vissim/NIImporter_Vissim.h>
#include <netimport/vissim/tempstructs/NIVissimClosures.h>
#include "NIVissimSingleTypeParser_Haltestellendefinition.h"


// ===========================================================================
// method definitions
// ===========================================================================
NIVissimSingleTypeParser_Haltestellendefinition::NIVissimSingleTypeParser_Haltestellendefinition(NIImporter_Vissim& parent)
    : NIImporter_Vissim::VissimSingleTypeParser(parent) {}


NIVissimSingleTypeParser_Haltestellendefinition::~NIVissimSingleTypeParser_Haltestellendefinition() {}


bool
NIVissimSingleTypeParser_Haltestellendefinition::parse(std::istream& from) {
    std::string tag;
    while (tag != "strecke") {
        tag = myRead(from);
    }
    while (tag != "laenge") {
        tag = myRead(from);
    }
    from >> tag;
    tag = readEndSecure(from);
    while (tag == "einsteiger" && tag != "DATAEND") {
        while (tag != "linie") {
            tag = myRead(from);
        }
        while (tag != "einsteiger" && tag != "DATAEND") {
            tag = readEndSecure(from);
        }
    }
    return true;
}


/****************************************************************************/
