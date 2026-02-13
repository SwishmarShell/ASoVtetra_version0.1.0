# ASoVtetra

## Version
Current version: v0.1.0

## Features in This Version
- Loads a PNG image and renders it on the Windows desktop.  
  (Recommended bit depth: 32-bit)

- The window can be moved and resized freely.  
  Pressing the **F1 key** will fix the window in place and render the image at the **topmost layer** of the desktop.
  - Rendering switches to Direct2D layered rendering.
  - Even when this program is selected, clicking on the displayed image will interact with the window behind it, not with this program.

## Example Use Cases
- When capturing the desktop, you can keep an original logo or artwork fixed at the topmost layer without interfering with your work.
- In OBS, you can freely move and fix the image at any position.
- During desktop work, you can display a motivational character image with adjustable transparency, keeping it visible without being intrusive.

## System Requirements & How to Launch
### Recommended Environment
- Windows 11 64bit (x64)
- Windows 11 32bit (x86)

### How to Launch
- Download the ZIP file for your environment, extract it, and run  
  **“ASoVtetra_version0.1.0.exe”** inside the folder.

## License
This project is released under the MIT License.  
Redistribution, modification, and commercial use are permitted under the condition that the following copyright notice and the MIT License text are retained.

Copyright (c) 2026 Swishmar.Shell

## Credit
If you use this project, a credit such as  
**“Based on work by Swishmar.Shell”**  
would be sincerely appreciated.

## Notes
- This software was developed on Windows 11 64bit (x64).  
  Operation on environments such as Windows 10 (x64) has not been verified, and functionality outside the recommended environment is not guaranteed.
- Future versions may include features using ONNX Runtime.  
  In that case, the software will be available **only for 64bit (x64) operating systems**.
