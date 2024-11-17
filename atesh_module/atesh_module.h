/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                     atesh_module
  vendor:                 devekusu_devices
  version:                8.0.0
  name:                   JUCE extended GUI classes
  description:            Miscellaneous GUI classes for specialised tasks.
  website:                http://www.juce.com/juce
  license:                AGPLv3/Commercial
  minimumCppStandard:     17

  dependencies:           juce_gui_basics
  OSXFrameworks:
  iOSFrameworks:
  WeakiOSFrameworks:      UserNotifications
  WeakMacOSFrameworks:    UserNotifications

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/
#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "LiveConstant.h"

#include "AteParameters.h"
#include "algorithm.h"
#include "FifoGUIBuffer.h"

#include "AteRotary.h"
#include "waveformVisualiser.h"
#include "AteshToolBar.h"

#include "AteLookNFeel.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"
