## Cynthia - A VSTi Synthesizer

### Senior Capstone Project
### Northern Michigan University
### Fall 2025

#### Description

- Cynthia is a VSTi synthesizer meant to be hosted inside a DAW such as Ableton Live or Logic Pro, but can also be used as a standalone application (currently for Windows only). To download the standalone version, go to "Actions", click on the most recent successful workflow, scroll to the very bottom, and under artifacts you will find the synth executable available for download.

- This synthesizer offers a minimal yet intuitive interface.

- To use this synthesizer you will need an external MIDI keyboard.

- My primary reasons for pursuing this project were to gain hands on experience in developing a real-time system, advance my knowledge of DSP principles and algorithms, and learn the common architectures, design patterns, and best-practices used in software synthesizer development.

#### DSP Modules/Features
Cynthia has the following DSP modules:
    
- Wavetable Oscillator
    - Multi-channel wavetable with wave morphing and detuning parameters.
    - Custom implementation based on the JUCE Wavetable Oscillator tutorial. 

- State-Variable Filter
    - LP, BP, HP
    - Uses JUCE's StateVariableTPTFilter

- Envelope
    - Attack, Decay, Sustain, Release
    - Uses JUCE's ADSR class.
    - This envelope ramps linearly.

- LFO
    - Subclasses the main audio oscillator, and therefore is capable of creating unique LFO waveforms through wave morphing and detuning.
    - This LFO only performs amplitude modulation in its current iteration.        
    - Has the standard LFO rate and LFO depth parameters.       
    - LFO rate allowed to range from 0 Hz to 500 Hz, and by extending beyond sub-audio range (0 Hz - 20 Hz), this synth can produce "ring-modulation" like behaviors. 



    



    
