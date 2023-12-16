# Surface-Bound-Odorant-Treadmill

One of the important subsets of odors sources used in olfactory navigation is surface-bound sources, which can broadly take the form of point sources or trails. Here, we present an automated treadmill design capable of dynamically printing odor trails of arbitrary lengths and configurations, and with closed-loop control of speed based on the animal’s movement. This design is presented and the utility demonstrated in a corresponding paper studying surface-bound odor trail interactions in locusts (in submission).The uploaded design incorporates some improvements not reflected in the version used in the published paper (i.e. the modification of some components to reduce paper jams, and quality of life improvements). The design is provided in four directories, containing the mechanical components, the electrical component design, the software and firmware for operation, and matlab scripts for parsing the output files.

![Photo of assembled treadmill](/DocumentationMedia/TreadmillPublishedDesign.png)

The treadmill is designed to enable long-duration interactions with surface-bound odor trails of arbitrary length. Below you can see a video of a locust engaged in a bout of trail following with an odor trail, where the treadmill advances once the locust approaches the front of the behavior chamber, revealing more trail for the locust to interact wtih.

![Video of locust following an odor trail](/DocumentationMedia/zigzagTrailFollowing.mp4)

The treadmill is comprised of a series of modules sequentially connected and mounted on an 80-20 frame enabling different slopes to be configured. The design of the mechanical components is provided as an .f3d file, openable using Fusion 360, which is available in [educational](https://www.autodesk.com/campaigns/education/fusion-360-education), or [professional or free versions](https://www.autodesk.com/products/fusion-360/personal/compare). 

In the mechanical design file, the design is organized by modules, with the exception of the 'behavior chamber' and 'driver' modules, which are merged in the present design. The organizational structure is arranged to reflect all modules independently for any future revisions. Further details regarding each of the design categories is provided below.

![Treadmill modules, and top view of behavior chamber](/DocumentationMedia/TreadmillModules.png)

## Mechanical Components
The base of the treadmill is the angle-adjustable frame made of extruded aluminum. The design uses 15mm aluminum extrusions 

The treadmill frame is made of extruded aluminum. The recommended parts list for the frame is

<ins>Misumi</ins>
| Description		| Part Number 		| Quantity|
| ------------- 	|:-------------:	| -----:|
| Aluminum Extrusions 	| HFS5-2020-1500 	| 6 	|
| col 2 is      	| HBLFSNF5-C-SEC 	|   $12 |
| Square Nuts (100 Pack)| HNKK5-5      		|   2 	|
| Hinges 		| HHPSN5-SET      	|   2 	|

<ins>Grainger</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| Bolts M5-0.8 10mm (50 Pack)	| 53GJ15	 	| 6 	|
| Bolts M5-0.8 35mm (50 Pack) 	| 53GJ20		| 6 	|

<ins>Amazon</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| Aluminum 20mm Brace (20 Pack)	| [20mm m5 compatible](https://www.amazon.com/Bracket-Aluminum-Brackets-Extrusion-Profile/dp)	| 3 	|

If cutting yourself, a chopsaw and an aluminum cutting blade (e.g. Tomax aluminum blade) will make accurate cuts to the correct length substantially faster and easier.

The remaining structural assembly consists of 


## Electrical Components
asdf

## Software Components
asdf

### Embedded Firmware
asdf

### Python Software Script
asdf

# Considerations for building
types of printer (reference whatshisface), stepper vs encoder, etc.

# Suggested directions for future expansion
two printheads
inkjet module
stepper motor controls
reward printer (behavior chamber front slot will need to be expanded)




