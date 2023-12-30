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
/// @file    GNEPersonTrip.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jun 2019
///
// A class for visualizing person trips in Netedit
/****************************************************************************/

#include <utils/gui/windows/GUIAppEnum.h>
#include <netedit/changes/GNEChange_Attribute.h>
#include <netedit/GNENet.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewNet.h>
#include <utils/gui/div/GUIDesigns.h>

#include "GNEPersonTrip.h"

// ===========================================================================
// method definitions
// ===========================================================================

GNEPersonTrip*
GNEPersonTrip::buildPersonTrip(GNENet* net, GNEDemandElement* personParent,
                               GNEEdge* fromEdge, GNEAdditional* fromTAZ, GNEJunction* fromJunction, GNEAdditional* fromBusStop, GNEAdditional* fromTrainStop,
                               GNEEdge* toEdge, GNEAdditional* toTAZ, GNEJunction* toJunction, GNEAdditional* toBusStop, GNEAdditional* toTrainStop,
                               double arrivalPosition, const std::vector<std::string>& types, const std::vector<std::string>& modes,
                               const std::vector<std::string>& lines) {
    // declare icon an tag
    const auto iconTag = getPersonTripTagIcon(fromEdge, toEdge, fromTAZ, toTAZ, fromJunction, toJunction,
                         fromBusStop, toBusStop, fromTrainStop, toTrainStop);
    // declare containers
    std::vector<GNEJunction*> junctions;
    std::vector<GNEEdge*> edges;
    std::vector<GNEAdditional*> additionals;
    // continue depending of input parameters
    if (fromEdge) {
        edges.push_back(fromEdge);
    } else if (fromTAZ) {
        additionals.push_back(fromTAZ);
    } else if (fromJunction) {
        junctions.push_back(fromJunction);
    } else if (fromBusStop) {
        additionals.push_back(fromBusStop);
    } else if (fromTrainStop) {
        additionals.push_back(fromTrainStop);
    }
    if (toEdge) {
        edges.push_back(toEdge);
    } else if (toTAZ) {
        additionals.push_back(toTAZ);
    } else if (toJunction) {
        junctions.push_back(toJunction);
    } else if (toBusStop) {
        additionals.push_back(toBusStop);
    } else if (toTrainStop) {
        additionals.push_back(toTrainStop);
    }
    return new GNEPersonTrip(net, iconTag.first, iconTag.second, personParent, junctions, edges, additionals, arrivalPosition, types, modes, lines);
}


GNEPersonTrip::GNEPersonTrip(SumoXMLTag tag, GNENet* net) :
    GNEDemandElement("", net, GLO_PERSONTRIP, tag, GUIIconSubSys::getIcon(GUIIcon::PERSONTRIP_EDGE),
                     GNEPathManager::PathElement::Options::DEMAND_ELEMENT, {}, {}, {}, {}, {}, {}),
GNEDemandElementPlan(this, -1, -1) {
    // reset default values
    resetDefaultValues();
}


GNEPersonTrip::~GNEPersonTrip() {}


GNEMoveOperation*
GNEPersonTrip::getMoveOperation() {
    return getPlanMoveOperation();
}


GUIGLObjectPopupMenu*
GNEPersonTrip::getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent) {
    return getPlanPopUpMenu(app, parent);
}


void
GNEPersonTrip::writeDemandElement(OutputDevice& device) const {
    // open tag
    device.openTag(SUMO_TAG_PERSONTRIP);
    // write plan attributes
    writePlanAttributes(device);
    // write modes
    if (myModes.size() > 0) {
        device.writeAttr(SUMO_ATTR_MODES, myModes);
    }
    // write lines
    if (myLines.size() > 0) {
        device.writeAttr(SUMO_ATTR_LINES, myLines);
    }
    // write vTypes
    if (myVTypes.size() > 0) {
        device.writeAttr(SUMO_ATTR_VTYPES, myVTypes);
    }
    // close tag
    device.closeTag();
}


GNEDemandElement::Problem
GNEPersonTrip::isDemandElementValid() const {
    return isPlanPersonValid();
}


std::string
GNEPersonTrip::getDemandElementProblem() const {
    return getPersonPlanProblem();
}


void
GNEPersonTrip::fixDemandElementProblem() {
    // currently the only solution is removing PersonTrip
}


SUMOVehicleClass
GNEPersonTrip::getVClass() const {
    return SVC_PEDESTRIAN;
}


const RGBColor&
GNEPersonTrip::getColor() const {
    return myNet->getViewNet()->getVisualisationSettings().colorSettings.personTripColor;
}


void
GNEPersonTrip::updateGeometry() {
    updatePlanGeometry();
}


Position
GNEPersonTrip::getPositionInView() const {
    return getPlanPositionInView();
}


std::string
GNEPersonTrip::getParentName() const {
    return getParentDemandElements().front()->getID();
}


Boundary
GNEPersonTrip::getCenteringBoundary() const {
    return getPlanCenteringBoundary();
}


void
GNEPersonTrip::splitEdgeGeometry(const double /*splitPosition*/, const GNENetworkElement* /*originalElement*/, const GNENetworkElement* /*newElement*/, GNEUndoList* /*undoList*/) {
    // geometry of this element cannot be splitted
}


void
GNEPersonTrip::drawGL(const GUIVisualizationSettings& s) const {
    drawPlanGL(checkDrawPersonPlan(), s, s.colorSettings.personTripColor, s.colorSettings.selectedPersonPlanColor);
}


void
GNEPersonTrip::computePathElement() {
    computePlanPathElement();
}


void
GNEPersonTrip::drawLanePartialGL(const GUIVisualizationSettings& s, const GNEPathManager::Segment* segment, const double offsetFront) const {
    drawPlanLanePartial(checkDrawPersonPlan(), s, segment, offsetFront, s.widthSettings.personTripWidth, s.colorSettings.personTripColor, s.colorSettings.selectedPersonPlanColor);
}


void
GNEPersonTrip::drawJunctionPartialGL(const GUIVisualizationSettings& s, const GNEPathManager::Segment* segment, const double offsetFront) const {
    drawPlanJunctionPartial(checkDrawPersonPlan(), s, segment, offsetFront, s.widthSettings.personTripWidth, s.colorSettings.personTripColor, s.colorSettings.selectedPersonPlanColor);
}


GNELane*
GNEPersonTrip::getFirstPathLane() const {
    return getFirstPlanPathLane();
}


GNELane*
GNEPersonTrip::getLastPathLane() const {
    return getLastPlanPathLane();
}


std::string
GNEPersonTrip::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        // specific person plan attributes
        case SUMO_ATTR_MODES:
            return joinToString(myModes, " ");
        case SUMO_ATTR_VTYPES:
            return joinToString(myVTypes, " ");
        case SUMO_ATTR_LINES:
            return joinToString(myLines, " ");
        default:
            return getPlanAttribute(key);
    }
}


double
GNEPersonTrip::getAttributeDouble(SumoXMLAttr key) const {
    return getPlanAttributeDouble(key);
}


Position
GNEPersonTrip::getAttributePosition(SumoXMLAttr key) const {
    return getPlanAttributePosition(key);
}


void
GNEPersonTrip::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    switch (key) {
        case SUMO_ATTR_MODES:
        case SUMO_ATTR_VTYPES:
        case SUMO_ATTR_LINES:
            GNEChange_Attribute::changeAttribute(this, key, value, undoList);
            break;
        default:
            setPlanAttribute(key, value, undoList);
            break;
    }
}


bool
GNEPersonTrip::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        // specific person plan attributes
        case SUMO_ATTR_MODES: {
            SVCPermissions dummyModeSet;
            std::string dummyError;
            return SUMOVehicleParameter::parsePersonModes(value, myTagProperty.getTagStr(), "", dummyModeSet, dummyError);
        }
        case SUMO_ATTR_VTYPES:
            return canParse<std::vector<std::string> >(value);
        case SUMO_ATTR_LINES:
            return canParse<std::vector<std::string> >(value);
        default:
            return isPlanValid(key, value);
    }
}


bool
GNEPersonTrip::isAttributeEnabled(SumoXMLAttr key) const {
    return isPlanAttributeEnabled(key);
}


std::string
GNEPersonTrip::getPopUpID() const {
    return getTagStr();
}


std::string
GNEPersonTrip::getHierarchyName() const {
    return getPlanHierarchyName();
}


const Parameterised::Map&
GNEPersonTrip::getACParametersMap() const {
    return getParametersMap();
}

// ===========================================================================
// private
// ===========================================================================

void
GNEPersonTrip::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        // specific person plan attributes
        case SUMO_ATTR_MODES:
            myModes = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
            break;
        case SUMO_ATTR_VTYPES:
            myVTypes = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
            break;
        case SUMO_ATTR_LINES:
            myLines = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
            break;
        default:
            setPlanAttribute(key, value);
            break;
    }
}


void
GNEPersonTrip::setMoveShape(const GNEMoveResult& moveResult) {
    // change both position
    myArrivalPosition = moveResult.newFirstPos;
    // update geometry
    updateGeometry();
}


void
GNEPersonTrip::commitMoveShape(const GNEMoveResult& moveResult, GNEUndoList* undoList) {
    undoList->begin(this, "arrivalPos of " + getTagStr());
    // now adjust start position
    setAttribute(SUMO_ATTR_ARRIVALPOS, toString(moveResult.newFirstPos), undoList);
    undoList->end();
}


GNEPersonTrip::GNEPersonTrip(GNENet* net, SumoXMLTag tag, GUIIcon icon, GNEDemandElement* personParent, const std::vector<GNEJunction*>& junctions,
                             const std::vector<GNEEdge*>& edges, const std::vector<GNEAdditional*>& additionals, double arrivalPosition,
                             const std::vector<std::string>& types, const std::vector<std::string>& modes, const std::vector<std::string>& lines) :
    GNEDemandElement(personParent, net, GLO_PERSONTRIP, tag, GUIIconSubSys::getIcon(icon),
                     GNEPathManager::PathElement::Options::DEMAND_ELEMENT, junctions, edges, {}, additionals, {personParent}, {}),
GNEDemandElementPlan(this, -1, arrivalPosition),
myVTypes(types),
myModes(modes),
myLines(lines) {
}

/****************************************************************************/
