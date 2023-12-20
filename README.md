# Surface-Bound-Odorant-Treadmill

One of the important subsets of odors sources used in olfactory navigation is surface-bound sources, which can broadly take the form of point sources or trails. Here, we present an automated treadmill design capable of dynamically printing odor trails of arbitrary lengths and configurations, and with closed-loop control of speed based on the animal’s movement. This design is presented and the utility demonstrated in a corresponding paper studying surface-bound odor trail interactions in locusts (in submission).The uploaded design incorporates some improvements not reflected in the version used in the published paper (i.e. the modification of some components to reduce paper jams, and quality of life improvements). The design is provided in four directories, containing the mechanical components, the electrical component design, and the software and firmware for operation.

![Photo of assembled treadmill](/DocumentationMedia/TreadmillPublishedDesign.png)

The treadmill is designed to enable long-duration interactions with surface-bound odor trails of arbitrary length. Below you can see a video of a locust engaged in a bout of trail following with an odor trail, where the treadmill advances once the locust approaches the front of the behavior chamber, revealing more trail for the locust to interact wtih.

![Video of locust following an odor trail](/DocumentationMedia/zigzagTrailFollowing.mp4)

The treadmill is comprised of a series of modules sequentially connected and mounted on an 80-20 frame enabling different slopes to be configured. The design of the mechanical components is provided as an .f3d file, openable using Fusion 360, which is available in [educational](https://www.autodesk.com/campaigns/education/fusion-360-education), or [professional or free versions](https://www.autodesk.com/products/fusion-360/personal/compare).

In the mechanical design file, the design is organized by modules, with the exception of the 'behavior chamber' and 'driver' modules, which are merged in the present design. The organizational structure is arranged to reflect all modules independently for any future revisions. Further details regarding each of the design categories is provided below.

![Treadmill modules, and top view of behavior chamber](/DocumentationMedia/TreadmillModules.png)

## Mechanical Components
The components of each treadmill module are broken out below. Each module section contains a list of the parts not 3D printed or laser cut from acrylic, as well as suggestions to ease assembly.

### Frame
The treadmill frame is made of 20mm extruded aluminum (2020). The recommended parts list for the frame is

<ins>Misumi</ins>
| Description				| Part Number 		| Quantity|
| ------------- 			|:-------------:	| -----:|
| Aluminum Extrusions 			| HFS5-2020-1500 	| 6 	|
| Aluminum 20mm Brace - optional	| HBLFSNF5-C-SEC 	|   8 	|
| Square Nuts (100 Pack)		| HNKK5-5      		|   2 	|
| Hinges 				| HHPSN5-SET      	|   2 	|

<ins>Grainger</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| Bolts M5-0.8 10mm (50 Pack)	| 53GJ15	 	| 5 	|

<ins>Amazon</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| Aluminum 20mm Brace (20 Pack)	| [20mm m5 compatible](https://a.co/d/7hRMjW2)	| 3 	|

If cutting yourself, a chopsaw and an aluminum cutting blade (e.g. [Tomax aluminum blade](https://a.co/d/3ZllS9o)) will will greatly ease making accurate and fast cuts compared to a hacksaw.

The HBLFSNF5 braces are optional because while they were used to mount the acrylic components of the evaporation tunnel to the aluminum frame, it is just as practical to mount to the alumium extrusion directly. They are included as an option, but are not suggested.

### Paper Towel Dispenser
The paper towel dispenser is constructed of two vertical mounting bars. The paper towel is hung between the bars on a 1/4" threaded rod, with tension applied to the side of the mount using a compressed spring adjustable via a pair of wingnuts. The position of the two bars (and thereby the alignment of the paper towel with the rig) can be adjusted by advancing or retracting one side or the other using the adjustment screws on the sides. 

<ins>Amazon</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| MGN12H 300mm linear rail guide| [MGN12H Rail](https://a.co/d/gDsAsOn)	| 2 	|
| MGN12H Carriage Block		| [MGN12H Carriage Block(https://a.co/d/gDsAsOn)	| 4 	|
| Spring Set			| [SP 9702](https://a.co/d/3PLaYax)	| 1 	|
| Bearing set			| [6202-2RS](https://a.co/d/721F6t8)	| 4 	|

<ins>Hardware Store (or Amazon, etc.)</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| 1/4" Threaded Rod 1 foot long	| na			| 3 	|
| 1/4" Nuts			| na			| 20 	|
| 1/4" Wingnuts			| na			| 3 	|

<ins>Grainger</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| 1/4" Fender Washers		| 35YU76		| 2 	|

<ins>Assembly of left, right alignment adjustments.</ins>
In the following assembly guides 3D printed components are placed in quotes and referred to by their body names in the Fusion 360 file.

The alignment assemblies consist of a 1' threaded rod, with the following parts threaded on in sequence, pictured below: 
 - Assembly 1: hex nut - "AlignmentScrewInner" - 6202 bearing - "AlignmentScrewInner" - hex nut 
 - Assembly 2: hex nut - spring - hex nut 
 - Assembly 3: hex nut - "AlignmentScrewInner" - 6202 bearing - "AlignmentScrewInner" - hex nut 
 - Assembly 4: hex nut - wing nut

![Left/right alignment assemblies](/DocumentationMedia/AlignmentAssembly.png)

Assemblies 1 and 3 hold the threaded rod and allow rotation.
Assembly 2 is clamped by the "AlignmentBoltClamp" that moves the vertical mounting bar forward or backwards as the threaded rod is rotated. The spring compressed between the two bolts serves to prevent backlash and reduce wobble in the assembly; this is pictured below.
Assembly 4 provides an easy way to rotate the threaded rod, by locking the wing nut in place using the hex nut.

![Vertical bar linkage - Assembly 2](/DocumentationMedia/AntiBacklashAssembly.png)

<ins>Assembly of paper towel suspension rod.</ins>
The alignment assemblies consist of a 1' threaded rod, with the following parts threaded on in sequence, pictured below: 
 - Assembly 1: wing nut - "PaperTowelHolderBrace" - hex nut 
 - Assembly 2: hex nut - "PaperTowelInsertInner" - hex nut 
 - Assembly 3: hex nut - hex nut - "PaperTowelInsertInner" - fender washer - spring - fender washer - hex nut - hex nut
 - Assembly 4: hex nut - "PaperTowelHolderBrace" - hex nut 

![Paper towel suspension rod](/DocumentationMedia/PaperTowelRod.png)

Assembly 1 holds the threaded rod, but allows the brace to be removed for changing the paper towel roll.
Assembly 2 holds the inner insert for the bearing on which the paper towel pivots.
Assembly 3 holds the inner insert for the second bearing on which the paper towel pivots. In addition, the fender washer compressed against the "TowelInsertOuter" provides friction which tensions the paper towel. By adjusting compression on the spring (or switching to a different spring) the paper towel tension can be tuned to keep the paper towel taut, but minimally burden the driver motor.
Assembly 4 holds the threaded rod.

### Print Carriage Assembly
The print assembly is centered around the print carriage assembly removed from an HP Photosmart C6180 inkjet printer. The inkjet printing assembly is removed, and the "PrintCarriageInsert" is inserted in its place. 

![Print carriage insert](/DocumentationMedia/PrintCarriageInsert.png)

The linear encoder from the print carriage PCB is accessed by connecting to the quadrature encoder using the ribbon cable. This was achieved by producing a breakout board on a piece of FR-4 Copper Clad board, soldering on a compatible ribbon cable mount, and soldering on male 2.54 header pins (pictured below). However, soldering wires directly instead of the pins would reduce risk of accidental disconnection. The traces on the print carriage PCB can also be soldered to directly to avoid needing to construct an adapter. 

![Print carriage adapter board](/DocumentationMedia/PrintCarriageInsert.png)

<ins>Amazon</ins>
| Description			| Part Number 								| Quantity|
| ------------- 		|:-------------:							| -----:|
| Copper-Clad Board		| [FR-4](https://a.co/d/aHfhtKA)					| 1 	|

<ins>Digikey</ins>
| Description		| Part Number 				| Quantity|
| ------------- 	|:-------------:			| -----:|
| Ribbon Cable Mount	| [X](X)				| 1 	|

### Print System Assembly
The pressurized print system is comprised of four parts connected in sequence:
 - Air pressure regulator
 - Print solution reservoir
 - Solenoid valve
 - Printhead

<ins>Printhead assembly</ins>
The printhead construction is the most challenging to fabricate. A 15mm segment is cut from the nylon rod, and drilled using a 6mm bit. One end is tapped to M6 to accomodate the brass printhead, and the other is drilled with a 1/4" bit and tapped to 1/4"-28 to accomodate the adapter for the 1/8" tubing. The printhead threads are wrapped with PTFE tape, and screwed into the nylon segment, taking care that excess PTFE tape is removed prior to insertion. 

Note: If a lathe is available it will greatly ease accurate drilling of the hole, and cutting of the segments. 

Note 2: If the holes are centered (e.g. drilled on a lathe), multiple printheads can be made to ease cleaning and clearing of any clogs that may occur.

<ins>Full assembly</ins>
The regulator is assembled by connecting 1/4" push connect fittings into the inlet and outlet ports. The inlet port is connected to a pressurized air system, and the outlet is connected to the 1/4" tubing. A length of 1/8" Kynar Flex tubing is coated with superglue, and rapidly inserted 5-10cm into the 1/4" tubing, and allowed to dry. The 1/8" tubing is then connected to the Odor Reservoir Cap, and secured in place using the 1/4"-28 adapter with the tubing protruding just inside the cap. A second length of tubing is inserted to the bottom of the flask and similarly secured. The other end of the second length of tubing is then inserted into the inlet port of the Clippard solenoid valve and secured. The outlet port of the solenoid is connected to a third length of tubing, sufficient to run from the position of the solenoid to the print carriage at all points of travel. This will depend on the location where the solenoid is mounted. The third length of tubing is then connected to the printhead using the same 1/4"-28 adapters, and the printhead is inserted into the print carriage to the depth where it just contacts the surface of the paper towel.

<ins>Clippard</ins>
| Description		| Part Number 																| Quantity|
| ------------- 	|:-------------:															| -----:|
| Solenoid valve	| [NR3-2-12, Imperial, Vac to 30psi, 2-way NC, In line ports, 1/4-28 UNF port, 12V](https://www.clippard.com/part/NR3-2-12)		| 1	|

<ins>Amazon</ins>
| Description			| Part Number 								| Quantity|
| ------------- 		|:-------------:							| -----:|
| Nylon Rod 			| [Nylon Rod, 1/2" diameter, 1' length](https://a.co/d/fmmiPHx)		| 1	|
| Brass Printhead Set (4)	| [0.1mm x M6 thread](https://a.co/d/aySFkyX)				| 1 	|

<ins>Cole Parmer</ins>
| Description					| Part Number 																| Quantity|
| ------------- 				|:-------------:															| -----:|
| Odor Reservoir Cap				| [EW-12018-03](https://www.coleparmer.com/i/cole-parmer-vaplock-solvent-delivery-cap-two-1-4-28-ports-gl45-1-ea/1201803)		| 1	|
| 1/8" Tubing to 1/4"-28 adapter (10 pack)	| [EW-12020-08](https://www.coleparmer.com/i/cole-parmer-vaplock-fitting-blue-pp-w-yellow-etfe-straight-compression-to-threaded-adapter-1-8-od-x-1-4-28-unf-m-10-pk/1202008)		| 1|

<ins>VWR</ins>
| Description					| Part Number 																| Quantity|
| ------------- 				|:-------------:															| -----:|
|Transfer Tubing, Kynar Flex™ 2750 PVDF, 1/16" ID x 1/8" OD; 50 ft |  [MFLX95100-01](https://us.vwr.com/store/product/39208964/masterflex-transfer-tubing-kynar-flextm-2750-pvdf-avantor) | 1 |

<ins>Grainger</ins>
| Description					| Part Number 																| Quantity|
| ------------- 				|:-------------:															| -----:|
|PVC Tubing 1/4"OD 1/8"ID |  [2LPT1](https://www.grainger.com/product/TYGON-Tubing-Tygon-S3-B-44-3-2LPT1) | 1 |
|PTFE Tape|  [3PDL5](https://www.grainger.com/product/ANTI-SEIZE-TECHNOLOGY-ANTI-SEIZE-TECH-Thread-Sealant-3PDL5) | 1 |

### Evaporation Tunnel
The plexiglass panels of the evaporation tunnel should be cut, the 3D printed components printed, and all assembled according to the .f3d file. The top tube mount should be connected to the exhaust tubing, and routed to an exhaust line. If additional flow is needed the exhaust fan can be used to increase the exhaust flow.

<ins>Amazon</ins>
| Description			| Part Number 					| Quantity|
| ------------- 		|:-------------:				| -----:|
| Exhaust Fan (optional)	| [MGN12H Rail](https://a.co/d/gDsAsOn)		| 1 	|
| Exhaust Tubing 		| [MGN12H Rail](https://a.co/d/gDsAsOn)		| 1 	|

<ins>Hardware Store (or Amazon, etc.)</ins>
| Description			| Part Number 		| Quantity	|
| ------------- 		|:-------------:	| -----:	|
| 1/4" Acrylic Sheet Clear 	| na			| na 		|

### Behavior Chamber
The plexiglass panels should be cut, the 3D printed components printed, and all assembled according to the .f3d file. The top and bottom panels of the behavior chamber are made of glass sheets; the side of the bottom sheet (and optionally the top sheet) should be trimmed to fit the width of the chamber (249mm) (see .f3d file). The top cover panel can be attached to the front of the behavior chamber using two strips of tape, positioned on the edges past the side of the behavior chamber. 

The inner sides of the behavior chamber acrylic sheets can be coated in fluon; this is most easily done by removing them from the behavior chamber, laying them flat on a counter, and applying the fluon using the brush dispenser in the parts list below. Before reapplying, the sides should be thoroughly cleaned using hot soap and water, and allowed to dry.

<ins>Hardware Store (or Amazon, etc.)</ins>
| Description			| Part Number 		| Quantity	|
| ------------- 		|:-------------:	| -----:	|
| 3/32" Glass Sheet (10" x 12")	| [Shape Glass](https://www.homedepot.com/p/10-in-x-12-in-x-0-09375-in-Clear-Glass-91012/300068325)			| 2 		|
| 1/8" Acrylic Sheet White	| na			| na 		|
| 1/4" Acrylic Sheet Clear	| na			| na 		|

<ins>Amazon</ins>
| Description			| Part Number 		| Quantity	|
| ------------- 		|:-------------:	| -----:	|
| Fluon				| [byFormica Fluon Plus](https://a.co/d/d54PUUA) | 1 		|

### Driver
The plexiglass panels should be cut, the 3D printed components printed, and all assembled according to the .f3d file. The driver and guide rod assemblies, as well as the nylon bushings, were removed from the same HP Photosmart C6180 inkjet printer that supplied the print carriage. One repair was made by replacing the motor with a larger 12V motor removed from [this](https://a.co/d/cd3eFye) peristaltic pump assembly. However, a motor such as [this](https://a.co/d/8RF5Nmk) should also be compatible with the mounting bracket, and the wide range of speeds would enable a fine tuning of maximum speed against torque for the driver assembly.

### Camera Assembly
The aluminum extrusion sections should be cut, the 3D printed components printed, and all assembled according to the .f3d file. The logitech webcam base can be mounted using the 1/4" threaded rod and nut, enabling easy positioning above the behavior chamber.

<ins>Amazon</ins>
| Description			| Part Number 					| Quantity|
| ------------- 		|:-------------:				| -----:|
| Logitech C920S		| [C920S](https://a.co/d/djdxm9s)		| 1 	|

<ins>Hardware Store (or Amazon, etc.)</ins>
| Description			| Part Number 		| Quantity|
| ------------- 		|:-------------:	| -----:|
| 1/4" Threaded Rod 1 inch long	| na			| 1 	|
| 1/4" Nuts			| na			| 1	|

### Backlight
The backlight is comprised of a 1/4" thick plexiglass base upon which IR and white LED strips are alternately layed. The LED strips are soldered together such that either the white (visible) or IR (non-visible) backlight can be used. The light diffuser panels should be cut according to the .f3d file, and can be inserted at multiple levels in the diffuser mount, up to a maximum of 6 panels. 1-2 sheets of paper should be layed over the top of diffuser panels to better distribute the illumination.

<ins>Hardware Store (or Amazon, etc.)</ins>
| Description				| Part Number 						| Quantity|
| ------------- 			|:-------------:					| -----:|
| 1/4" Acrylic Sheet Clear		| na							| 1 	|
| IR LED Strip				| [SMD3528 850nm LED Strip](https://a.co/d/do7MLOz)	| 1	|
| White LED Strip			| [SMD5050 White LED Strip](https://a.co/d/cHSYJyx)	| 1	|
| Light Diffuser Sheet			| [Diffuser](https://www.homedepot.com/p/OPTIX-23-75-in-x-47-75-in-Clear-Plastic-Acrylic-Prismatic-Ceiling-Light-Panel-1A20083A/100550880)	| na	|
| White Printer Paper (8.5"x14")	| [Paper](https://a.co/d/dwqtg2p)			| 1	|

<ins>Grainger</ins>
| Description			| Part Number 		| Quantity	|
| ------------- 		|:-------------:	| -----:	|
| Bolts M5-0.8 35mm (50 Pack) 	| 53GJ20		| 1 		|

## Electrical Components
The circuit layout and PCB design are provided in the form of a KiCAD project. A parts list is provided below:

<ins>Parts List</ins>
| Description		| Part Number 			| Quantity|
| ------------- 	|:-------------:		| -----:|
| Teensy 3.5			| Teensy 3.5		| 1 	|
| 5V Regulator			| L7805CV Through Hole	| 2 	|
| Optoisolator			| 6N137 Through Hole	| 10 	|
| Hex Inverter			| SN74HC14N Through Hole| 2 	|
| TIP41C Transistor		| Tip41C Through Hole	| 2 	|
| Diode				| 1N4002 Through Hole	| 2 	|
| Filter Capacitor		| 4.7uF Through Hole	| 11 	|
| Limit Switch Bias Resistor 1	| 100 Ohm SMD	 	| 1 	|
| Limit Switch Bias Resistor 2	| 1.2 KOhm SMD	 	| 1 	|
| Optoisolator Filter Cap	| 1uF SMD		| 10 	|
| Optoisolator LED Resistor	| 360 Ohm SMD		| 10 	|
| Optoisolator Bias Resistor	| X Ohm SMD		| 10 	|

Note: Only two of the four capacitor slots around the voltage regulator are populated. The extra slots can be used if additional filtering is required.

## Software Components
The software is provided in the form of a python script and an embedded program for the Teensy 3.5. 

### Embedded Firmware
The embedded firmware can be uploaded using the [Arduino IDE](https://www.arduino.cc/en/software). 

### Python Software Script
The python software script contains several values that must be configured on the experimental computer in order for communication with the microcontroller (and thereby control over the treadmill) to be achieved. Namely, the serial port and camera input must be specified correctly. To do this 

cap = cv2.VideoCapture(0) in camMain can be set to different camera inputs by changing the '0' to a different number corresponding to the desired input.

ser = serial.Serial('COM4', 2000000, writeTimeout=0) in serMain can be configured to read from a different com port, e.g. 'COM3', and should be configured to match the serial port on which the treadmill is connected.

In addition, the threshold for binarizing the image in order to track the locust must be configured by adjusting the threshold value in the calcSpeed function. Note that this will change when switching between visible and IR backlighting. This can be done by modifying the value of '20' in the below code line to a value found to effectively isolate the animal on the treadmill with the given backlight. 

bw = cv2.threshold(gray, 20, 255, cv2.THRESH_BINARY_INV)[1]

If switching between visible and IR conditions, two versions of the above line can be written, and the unused one commented out prior to running the treadmill.

# Considerations for building
If you are considering building this treadmill there are several considerations that you may want to take into account. The first is whether greater flexibility of odorants, or the width and precision of the trail are more important for your purposes. If some odorants that are incompatible with an inkjet cartridge will be used, then the pressurized reservoir system described in the Print System Assembly section will be the most suitable. However, if the odor panel is limited to odorants that would be compatible with a print cardtridge, then repurposing a print cartridge may provide more consistent and smaller trails. Such a method was used by Dr. Venkatesh Murthy to produce odor trails for mice (see odor trails sections in the [DeepLabCut paper](https://www.nature.com/articles/s41593-018-0209-y#Ack1) by Mathis et al.).

Another consideration is whether to use the optical encoder system used here for the treadmill and print carriage positions. While highly precise, stepper motors likely would produce sufficiently accurate positioning, and may be easier to obtain and configure. This is especially useful if considering expanding the treadmill by adding a second printhead or a system for depositing rewards on the trail (see Suggested Directions for Future Modifications and Expansions) given that the Teensy 3.5 only has two hardware quadrature decoders, both of which are already allocated to the belt encoder and print carriage encoders. Furthermore, if hardware quadrature decoding is not required it would also be possible to utilize the faster Teensy 4.x series microcontrollers or other alternatives.

# Suggested Directions for Future Modifications and Expansions
- Second printhead
- Use of an inkjet module for higher precision trails (albeit with a more constrained odor panel)
- Use of stepper motors instead of the PID controlled print carriage assembly.
- Reward depositor or printer (behavior chamber front slot will need to be raised to avoid rewards being scraped off the paper towel before entering the behavior chamber)




