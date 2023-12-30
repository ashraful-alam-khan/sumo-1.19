/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.dev/sumo
// Copyright (C) 2003-2023 German Aerospace Center (DLR) and others.
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
/// @file    GUIMessageWindow.h
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @date    Tue, 25 Nov 2003
///
// A logging window for the gui
/****************************************************************************/
#pragma once
#include <config.h>

#include <string>
#include <utils/foxtools/fxheader.h>
#include <utils/gui/events/GUIEvent.h>
#include <utils/gui/windows/GUIMainWindow.h>
#include <utils/iodevices/OutputDevice.h>


// ===========================================================================
// class declarations
// ===========================================================================
class GUIGlObject;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GUIMessageWindow
 * @brief A logging window for the gui
 *
 * This class displays messages incoming to the gui from either the load or
 *  the run thread.
 *
 * The text is colored in dependence to its type (messages: green, warnings: yellow,
 *  errors: red)
 *
 * Each time a new message is passed, the window is reopened.
 */
class GUIMessageWindow : public FXText {
    FXDECLARE(GUIMessageWindow)
public:
    /** @brief Constructor
     *
     * @param[in] parent The parent window
     */
    GUIMessageWindow(FXComposite* parent, GUIMainWindow* mainWindow);

    /// @brief Destructor
    ~GUIMessageWindow();

    /// @brief set cursor position over a certain line
    virtual void setCursorPos(FXint pos, FXbool notify = FALSE);

    /** @brief Adds new text to the window
     *
     * The type of the text is determined by the first parameter
     *
     * @param[in] eType The type of the event the message was generated by
     * @param[in] msg The message
     * @see GUIEventType
     */
    void appendMsg(GUIEventType eType, const std::string& msg);

    /// @brief Adds a a separator to this log window
    void addSeparator();

    /// @brief Clears the window
    void clear();

    /// @brief register message handlers
    void registerMsgHandlers();

    /// @brief unregister message handlers
    void unregisterMsgHandlers();

    /// @brief switch locate links on and off
    static void enableLocateLinks(const bool val) {
        myLocateLinks = val;
    }

    /// @brief ask whether locate links is enabled
    static bool locateLinksEnabled() {
        return myLocateLinks;
    }

    /// @brief switch locate links on and off
    static void setBreakPointOffset(SUMOTime val) {
        myBreakPointOffset = val;
    }

    /// @brief ask whether locate links is enabled
    static SUMOTime getBreakPointOffset() {
        return myBreakPointOffset;
    }

    /// @brief handle keys
    long onKeyPress(FXObject* o, FXSelector sel, void* data);

    /// @brief The text colors used
    static FXHiliteStyle* getStyles();

protected:
    /// @brief FOX needs this
    FOX_CONSTRUCTOR(GUIMessageWindow)

private:
    /// @brief class MsgOutputDevice
    class MsgOutputDevice : public OutputDevice {

    public:
        /// @brief constructor
        MsgOutputDevice(GUIMessageWindow* msgWindow, GUIEventType type) :
            myMsgWindow(msgWindow),
            myType(type) { }

        /// @brief destructor
        ~MsgOutputDevice() { }

    protected:
        /// @brief get Output Stream
        std::ostream& getOStream() {
            return myStream;
        }
        /// @brief write hook
        void postWriteHook() {
            myMsgWindow->appendMsg(myType, myStream.str());
            myStream.str("");
        }

    private:
        /// @brief pointer to message Windows
        GUIMessageWindow* myMsgWindow;

        /// @brief output string stream
        std::ostringstream myStream;

        /// @brief type of event
        GUIEventType myType;
    };

    /// @brief get active string object
    const GUIGlObject* getActiveStringObject(const FXString& text, const FXint pos, const FXint lineS, const FXint lineE) const;

    /// @brief get time string object
    SUMOTime getTimeString(const FXString& text, const FXint pos, const FXint lineS, const FXint lineE) const;

    /// @brief fill styles
    void fillStyles();

    /// @brief main window
    GUIMainWindow* myMainWindow;

    /// @brief whether messages are linked to the GUI elements
    static bool myLocateLinks;

    /// @brief Offset when creating breakpoint by clicking on time links
    static SUMOTime myBreakPointOffset;

    /// @brief The text colors used
    static FXHiliteStyle* myStyles;

    /// @brief The instances of message retriever encapsulations
    OutputDevice* myErrorRetriever, *myDebugRetriever, *myGLDebugRetriever, *myMessageRetriever, *myWarningRetriever;
};
