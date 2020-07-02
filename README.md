# Telink_mesh_sdk_VSC
SDK for Telink 825x to coding ang building in VScode
 
Building tutorial:
  1. Download and set up TC32 toolchain, follow this link:
  
    * Window: https://github.com/Ai-Thinker-Open/Telink_825X_SDK/blob/master/start_windows.md
    
    * Linux: https://github.com/Ai-Thinker-Open/Telink_825X_SDK/blob/master/start_linux.md
    
  3. Clone sdk:
  
    git clone https://github.com/tuanldchainos/Telink_mesh_sdk_VSC
    
  2. Open one random project, in Example folder or Project folder:
  
    cd example/blink
    
  3. Open terminal here:
  
    make
    
  4. Load file .bin to chipset:
  
    * USB: make flash
    
    * EVK: using BDT tool
