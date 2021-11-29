# Compact Word Processor

This is a parallel project which is meant to explore writing and building possibilities with various small form-factor platforms.

## Keyboard layout

![keyboard layout](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/layout.png)

There are 8 mounting screw holes 3mm in diameter each and the corner radius is 3mm. The top case or lid may be attached via magnets when closed.
The top layer and keyboard switch plate may be 3D printed, CNC milled, or resin cast as one unit. If 3D printed or cast, the switch plate and top border are provied as one [STL file](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/switchplate.stl).

If the switch plate itself is to be made from a separate material from the top, such as metal, there's a separate [STL file](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/switchtop.stl). The switch top is an interface with the lid and also provides an aesthetic boundary for the keys while in use. It should look like this when made from its [DXF file](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/switchtop.dxf).
![switch top template](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/switchtop.svg)

The cut template for the key switch plate is provided as a separate [DXF file](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/switchplate.dxf) when not cast or milled together with the top. It should look like this when complete.
![switch laser cut template](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/switchlasertpl.svg)

The chassis [DXF file](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/case.dxf) is just a closed loop matching the perimeter. The final thickness should be built with layers of thinner material if this part is laser cut. The edges will be round coming from a CNC mill. The top and bottom edges are 8mm wide and the left and right edges are 5mm wide.
![case cut template](https://raw.githubusercontent.com/cypnk/WordProcessor/master/hardware/compact/caselasertpl.svg)

The bottom plate may be 3D printed or cast together with the case using its [STL file](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/bottomcase.stl) or laser cut or CNC milled with its [DXF file](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/bottom.dxf), which is attached to the case loop above and should look like this when complete.
![bottom plate cut template](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/bottomlasertpl.svg)

The top cover or lid has its own [STL file](https://github.com/cypnk/WordProcessor/blob/master/hardware/compact/topcover.stl) for 3D printing or casting, matching the angled slope of the switch top layer. If laser cut or CNC milled, the top cover may be built up using the case and bottom DXF templates above since they both have the same outer dimensions. Unlike 3D printed or cast versions, this means the lid will be flat on both top and bottom surfaces.

## Shortcuts

Possible shortcuts for navigation:

| Key / Combo	| Function		|
|---------------|-----------------------|
| Menu + Menu		| Power on / off |
| Menu + H		| Start of line |
| Menu + E		| End of line |
| Next / Previous	| Cursor right / left |
| Shift + Next / Previous | Navigate between words |
| Menu + Next / Previous | Navigate between lines |
| Shift + Menu + Next / Previous | Scroll page down / up |

Possible shortcuts for editing:
| Key / Combo	| Function 		|
|---------------|-----------------------|
| Next / Previous + Letter	| Alt character, symbols, or accented letter |
| Shift + Erase | Delete forward |
| Menu + K | Delete line |
