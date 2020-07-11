oscOscilloscopeExample
===============

An example that reads data from an OSC stream and plots it in an oscilloscope window. 

## Usage:

- Edit oscInputSettings.xml
  - Change the port to listen to your OSC stream 
  - Change the OSC addresses to match the incoming OSC addresses
  - Reload oscInpuSettings by hitting (capital) 'P'
- Edit ofxOscilloscopeSettings.xml 
  - Change how the oscilloscope is displayed if desired
  - Note that the plotId should match the <output> field in oscInputSettings.xml
  - Reload the settings with 'L'
- Run
