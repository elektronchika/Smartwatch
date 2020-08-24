EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LC_lib:S1D13C00 U?
U 1 1 5F4D205F
P 5500 3500
AR Path="/5F4D205F" Ref="U?"  Part="1" 
AR Path="/5F450EA8/5F4D205F" Ref="U1"  Part="1" 
F 0 "U1" H 6694 3546 50  0000 L CNN
F 1 "S1D13C00" H 6694 3455 50  0000 L CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5500 3500 50  0001 C CNN
F 3 "" H 5500 3500 50  0001 C CNN
	1    5500 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2065
P 6250 4650
AR Path="/5F4D2065" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2065" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 6250 4400 50  0001 C CNN
F 1 "GND" H 6255 4477 50  0000 C CNN
F 2 "" H 6250 4650 50  0001 C CNN
F 3 "" H 6250 4650 50  0001 C CNN
	1    6250 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D206B
P 9000 5500
AR Path="/5F4D206B" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D206B" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 9000 5250 50  0001 C CNN
F 1 "GND" V 9005 5372 50  0000 R CNN
F 2 "" H 9000 5500 50  0001 C CNN
F 3 "" H 9000 5500 50  0001 C CNN
	1    9000 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2071
P 4000 3950
AR Path="/5F4D2071" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2071" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 4000 3700 50  0001 C CNN
F 1 "GND" V 4005 3822 50  0000 R CNN
F 2 "" H 4000 3950 50  0001 C CNN
F 3 "" H 4000 3950 50  0001 C CNN
	1    4000 3950
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D2077
P 7500 5350
AR Path="/5F4D2077" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D2077" Ref="C12"  Part="1" 
F 0 "C12" H 7615 5396 50  0000 L CNN
F 1 "3.3u" H 7615 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7538 5200 50  0001 C CNN
F 3 "~" H 7500 5350 50  0001 C CNN
	1    7500 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D207D
P 5600 5350
AR Path="/5F4D207D" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D207D" Ref="C8"  Part="1" 
F 0 "C8" H 5715 5396 50  0000 L CNN
F 1 "100n" H 5715 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5638 5200 50  0001 C CNN
F 3 "~" H 5600 5350 50  0001 C CNN
	1    5600 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2083
P 5600 5500
AR Path="/5F4D2083" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2083" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 5600 5250 50  0001 C CNN
F 1 "GND" H 5605 5327 50  0000 C CNN
F 2 "" H 5600 5500 50  0001 C CNN
F 3 "" H 5600 5500 50  0001 C CNN
	1    5600 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2089
P 7500 5500
AR Path="/5F4D2089" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2089" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 7500 5250 50  0001 C CNN
F 1 "GND" H 7505 5327 50  0000 C CNN
F 2 "" H 7500 5500 50  0001 C CNN
F 3 "" H 7500 5500 50  0001 C CNN
	1    7500 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D208F
P 6150 5350
AR Path="/5F4D208F" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D208F" Ref="C10"  Part="1" 
F 0 "C10" H 6265 5396 50  0000 L CNN
F 1 "1u" H 6265 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6188 5200 50  0001 C CNN
F 3 "~" H 6150 5350 50  0001 C CNN
	1    6150 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 5200 6150 4650
$Comp
L power:GND #PWR?
U 1 1 5F4D2096
P 6150 5500
AR Path="/5F4D2096" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2096" Ref="#PWR011"  Part="1" 
F 0 "#PWR011" H 6150 5250 50  0001 C CNN
F 1 "GND" H 6155 5327 50  0000 C CNN
F 2 "" H 6150 5500 50  0001 C CNN
F 3 "" H 6150 5500 50  0001 C CNN
	1    6150 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F4D209C
P 9000 5350
AR Path="/5F4D209C" Ref="R?"  Part="1" 
AR Path="/5F450EA8/5F4D209C" Ref="R2"  Part="1" 
F 0 "R2" H 9070 5396 50  0000 L CNN
F 1 "1k" H 9070 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8930 5350 50  0001 C CNN
F 3 "~" H 9000 5350 50  0001 C CNN
	1    9000 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20A2
P 5950 1450
AR Path="/5F4D20A2" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20A2" Ref="C9"  Part="1" 
F 0 "C9" H 6065 1496 50  0000 L CNN
F 1 "3.3u" H 6065 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5988 1300 50  0001 C CNN
F 3 "~" H 5950 1450 50  0001 C CNN
	1    5950 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D20A8
P 5950 1600
AR Path="/5F4D20A8" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D20A8" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 5950 1350 50  0001 C CNN
F 1 "GND" H 5955 1427 50  0000 C CNN
F 2 "" H 5950 1600 50  0001 C CNN
F 3 "" H 5950 1600 50  0001 C CNN
	1    5950 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3850 4350 3850
Wire Wire Line
	2750 3550 4350 3550
$Comp
L Device:Crystal Y?
U 1 1 5F4D20CA
P 8250 5000
AR Path="/5F4D20CA" Ref="Y?"  Part="1" 
AR Path="/5F450EA8/5F4D20CA" Ref="Y1"  Part="1" 
F 0 "Y1" H 8250 5268 50  0000 C CNN
F 1 "Crystal" H 8250 5177 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_3215-2Pin_3.2x1.5mm" H 8250 5000 50  0001 C CNN
F 3 "~" H 8250 5000 50  0001 C CNN
	1    8250 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20D0
P 8500 5350
AR Path="/5F4D20D0" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20D0" Ref="C14"  Part="1" 
F 0 "C14" H 8615 5396 50  0000 L CNN
F 1 "10p" H 8615 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8538 5200 50  0001 C CNN
F 3 "~" H 8500 5350 50  0001 C CNN
	1    8500 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20D6
P 8000 5350
AR Path="/5F4D20D6" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20D6" Ref="C13"  Part="1" 
F 0 "C13" H 8115 5396 50  0000 L CNN
F 1 "10p" H 8115 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8038 5200 50  0001 C CNN
F 3 "~" H 8000 5350 50  0001 C CNN
	1    8000 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 5000 8000 5000
Connection ~ 8000 5000
Wire Wire Line
	8000 5000 8000 5200
Wire Wire Line
	8400 5000 8500 5000
Connection ~ 8500 5000
Wire Wire Line
	8500 5000 8500 5200
$Comp
L power:GND #PWR?
U 1 1 5F4D20E2
P 8000 5500
AR Path="/5F4D20E2" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D20E2" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 8000 5250 50  0001 C CNN
F 1 "GND" H 8005 5327 50  0000 C CNN
F 2 "" H 8000 5500 50  0001 C CNN
F 3 "" H 8000 5500 50  0001 C CNN
	1    8000 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D20E8
P 8500 5500
AR Path="/5F4D20E8" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D20E8" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 8500 5250 50  0001 C CNN
F 1 "GND" H 8505 5327 50  0000 C CNN
F 2 "" H 8500 5500 50  0001 C CNN
F 3 "" H 8500 5500 50  0001 C CNN
	1    8500 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20EE
P 3600 5350
AR Path="/5F4D20EE" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20EE" Ref="C3"  Part="1" 
F 0 "C3" H 3715 5396 50  0000 L CNN
F 1 "1u" H 3715 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3638 5200 50  0001 C CNN
F 3 "~" H 3600 5350 50  0001 C CNN
	1    3600 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20F4
P 4000 5350
AR Path="/5F4D20F4" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20F4" Ref="C4"  Part="1" 
F 0 "C4" H 4115 5396 50  0000 L CNN
F 1 "1u" H 4115 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4038 5200 50  0001 C CNN
F 3 "~" H 4000 5350 50  0001 C CNN
	1    4000 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D20FA
P 5150 5350
AR Path="/5F4D20FA" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D20FA" Ref="C7"  Part="1" 
F 0 "C7" H 5265 5396 50  0000 L CNN
F 1 "1u" H 5265 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5188 5200 50  0001 C CNN
F 3 "~" H 5150 5350 50  0001 C CNN
	1    5150 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D2100
P 4750 5350
AR Path="/5F4D2100" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D2100" Ref="C6"  Part="1" 
F 0 "C6" H 4865 5396 50  0000 L CNN
F 1 "1u" H 4865 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4788 5200 50  0001 C CNN
F 3 "~" H 4750 5350 50  0001 C CNN
	1    4750 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D2106
P 4400 5350
AR Path="/5F4D2106" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D2106" Ref="C5"  Part="1" 
F 0 "C5" H 4515 5396 50  0000 L CNN
F 1 "1u" H 4515 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4438 5200 50  0001 C CNN
F 3 "~" H 4400 5350 50  0001 C CNN
	1    4400 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5200 5600 4900
Wire Wire Line
	5250 4900 5250 4650
$Comp
L power:GND #PWR?
U 1 1 5F4D210E
P 5150 5500
AR Path="/5F4D210E" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D210E" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 5150 5250 50  0001 C CNN
F 1 "GND" H 5155 5327 50  0000 C CNN
F 2 "" H 5150 5500 50  0001 C CNN
F 3 "" H 5150 5500 50  0001 C CNN
	1    5150 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2114
P 4000 5500
AR Path="/5F4D2114" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2114" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 4000 5250 50  0001 C CNN
F 1 "GND" H 4005 5327 50  0000 C CNN
F 2 "" H 4000 5500 50  0001 C CNN
F 3 "" H 4000 5500 50  0001 C CNN
	1    4000 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D211A
P 3600 5500
AR Path="/5F4D211A" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D211A" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 3600 5250 50  0001 C CNN
F 1 "GND" H 3605 5327 50  0000 C CNN
F 2 "" H 3600 5500 50  0001 C CNN
F 3 "" H 3600 5500 50  0001 C CNN
	1    3600 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4650 5150 5200
Wire Wire Line
	4350 4250 3600 4250
Wire Wire Line
	3600 4250 3600 5200
Wire Wire Line
	4750 4650 4750 4900
Wire Wire Line
	4750 4900 4000 4900
Wire Wire Line
	4000 4900 4000 5200
Wire Wire Line
	5050 4650 5050 5600
Wire Wire Line
	4400 5600 4400 5500
Wire Wire Line
	4750 5500 4750 5600
Wire Wire Line
	4400 5200 4400 5000
Wire Wire Line
	4400 5000 4850 5000
Wire Wire Line
	4850 5000 4850 4650
Wire Wire Line
	4750 5200 4750 5100
Wire Wire Line
	4950 5100 4950 4650
Connection ~ 4750 5600
Wire Wire Line
	4750 5600 4400 5600
Wire Wire Line
	4750 5600 5050 5600
Wire Wire Line
	4750 5100 4950 5100
Wire Wire Line
	4000 2750 4350 2750
Wire Wire Line
	4000 2850 4350 2850
Wire Wire Line
	4000 2950 4350 2950
Wire Wire Line
	4000 3050 4350 3050
Wire Wire Line
	4000 3150 4350 3150
Wire Wire Line
	4000 3250 4350 3250
Wire Wire Line
	4000 3350 4350 3350
Wire Wire Line
	4000 3450 4350 3450
Wire Wire Line
	4000 3650 4350 3650
Wire Wire Line
	4000 3750 4350 3750
Wire Wire Line
	4000 2250 4750 2250
Wire Wire Line
	4750 2250 4750 2350
Wire Wire Line
	4000 2150 4850 2150
Wire Wire Line
	4850 2150 4850 2350
Wire Wire Line
	4000 2050 4950 2050
Wire Wire Line
	4950 2050 4950 2350
Wire Wire Line
	4000 1950 5050 1950
Wire Wire Line
	5050 1950 5050 2350
Text Label 4000 1950 0    50   ~ 0
XRST
Text Label 4000 2050 0    50   ~ 0
VST
Text Label 4000 2150 0    50   ~ 0
VCK
Text Label 4000 2250 0    50   ~ 0
HST
Text Label 4000 2750 0    50   ~ 0
HCK
Text Label 4000 2850 0    50   ~ 0
ENB
Text Label 4000 2950 0    50   ~ 0
RED0
Text Label 4000 3050 0    50   ~ 0
RED1
Text Label 4000 3150 0    50   ~ 0
GRN0
Text Label 4000 3250 0    50   ~ 0
GRN1
Text Label 4000 3350 0    50   ~ 0
BLU0
Text Label 4000 3450 0    50   ~ 0
BLU1
Text Label 4000 3650 0    50   ~ 0
VCOM
Text Label 4000 3750 0    50   ~ 0
XFRP
Text Label 4000 4050 0    50   ~ 0
VMDL
Text Label 4000 4150 0    50   ~ 0
VMDH
NoConn ~ 5350 4650
NoConn ~ 5450 4650
NoConn ~ 5550 4650
NoConn ~ 5650 4650
NoConn ~ 5750 4650
NoConn ~ 5850 4650
NoConn ~ 5950 4650
NoConn ~ 6050 4650
NoConn ~ 6650 3750
NoConn ~ 6650 3650
NoConn ~ 6650 3050
NoConn ~ 6650 3150
NoConn ~ 6650 3250
NoConn ~ 6650 3350
NoConn ~ 6650 3450
NoConn ~ 6650 3550
Wire Wire Line
	4000 3950 4350 3950
Wire Wire Line
	5250 4900 5600 4900
$Comp
L Device:R R?
U 1 1 5F4D2166
P 7050 4500
AR Path="/5F4D2166" Ref="R?"  Part="1" 
AR Path="/5F450EA8/5F4D2166" Ref="R1"  Part="1" 
F 0 "R1" H 6980 4454 50  0000 R CNN
F 1 "10k" H 6980 4545 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6980 4500 50  0001 C CNN
F 3 "~" H 7050 4500 50  0001 C CNN
	1    7050 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	6650 4250 6800 4250
Wire Wire Line
	7050 4250 7050 4350
$Comp
L power:VDD #PWR?
U 1 1 5F4D216E
P 5600 4900
AR Path="/5F4D216E" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D216E" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 5600 4750 50  0001 C CNN
F 1 "VDD" H 5617 5073 50  0000 C CNN
F 2 "" H 5600 4900 50  0001 C CNN
F 3 "" H 5600 4900 50  0001 C CNN
	1    5600 4900
	1    0    0    -1  
$EndComp
Connection ~ 5600 4900
$Comp
L power:VDD #PWR?
U 1 1 5F4D2175
P 5750 1050
AR Path="/5F4D2175" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2175" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 5750 900 50  0001 C CNN
F 1 "VDD" H 5767 1223 50  0000 C CNN
F 2 "" H 5750 1050 50  0001 C CNN
F 3 "" H 5750 1050 50  0001 C CNN
	1    5750 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 1050 5750 1250
Connection ~ 5750 1250
Wire Wire Line
	5750 1250 5750 2350
$Comp
L power:VDD #PWR?
U 1 1 5F4D217E
P 7500 4500
AR Path="/5F4D217E" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D217E" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 7500 4350 50  0001 C CNN
F 1 "VDD" H 7517 4673 50  0000 C CNN
F 2 "" H 7500 4500 50  0001 C CNN
F 3 "" H 7500 4500 50  0001 C CNN
	1    7500 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5200 7500 4750
Wire Wire Line
	6650 4150 7250 4150
Wire Wire Line
	7250 4150 7250 4750
Wire Wire Line
	7250 4750 7500 4750
Connection ~ 7500 4750
Wire Wire Line
	7500 4750 7500 4500
Wire Wire Line
	7050 4650 7050 4750
Wire Wire Line
	7050 4750 7250 4750
Connection ~ 7250 4750
$Comp
L Device:C C?
U 1 1 5F4D218D
P 6800 5350
AR Path="/5F4D218D" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D218D" Ref="C11"  Part="1" 
F 0 "C11" H 6915 5396 50  0000 L CNN
F 1 "100n" H 6915 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6838 5200 50  0001 C CNN
F 3 "~" H 6800 5350 50  0001 C CNN
	1    6800 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D2193
P 6800 5500
AR Path="/5F4D2193" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D2193" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 6800 5250 50  0001 C CNN
F 1 "GND" H 6805 5327 50  0000 C CNN
F 2 "" H 6800 5500 50  0001 C CNN
F 3 "" H 6800 5500 50  0001 C CNN
	1    6800 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 4250 6800 5200
Connection ~ 6800 4250
Wire Wire Line
	6800 4250 7050 4250
Text Label 2750 3550 0    50   ~ 0
DISPVDD
Text Label 3200 3850 0    50   ~ 0
VCOMVDD
Wire Wire Line
	2450 4050 2750 4050
Wire Wire Line
	2050 4150 2850 4150
$Comp
L Device:C C?
U 1 1 5F4D21B6
P 2450 5350
AR Path="/5F4D21B6" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D21B6" Ref="C2"  Part="1" 
F 0 "C2" H 2565 5396 50  0000 L CNN
F 1 "1u" H 2565 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2488 5200 50  0001 C CNN
F 3 "~" H 2450 5350 50  0001 C CNN
	1    2450 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D21BC
P 2450 5500
AR Path="/5F4D21BC" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D21BC" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 2450 5250 50  0001 C CNN
F 1 "GND" H 2455 5327 50  0000 C CNN
F 2 "" H 2450 5500 50  0001 C CNN
F 3 "" H 2450 5500 50  0001 C CNN
	1    2450 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F4D21C2
P 2050 5350
AR Path="/5F4D21C2" Ref="C?"  Part="1" 
AR Path="/5F450EA8/5F4D21C2" Ref="C1"  Part="1" 
F 0 "C1" H 2165 5396 50  0000 L CNN
F 1 "1u" H 2165 5305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2088 5200 50  0001 C CNN
F 3 "~" H 2050 5350 50  0001 C CNN
	1    2050 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F4D21C8
P 2050 5500
AR Path="/5F4D21C8" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D21C8" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 2050 5250 50  0001 C CNN
F 1 "GND" H 2055 5327 50  0000 C CNN
F 2 "" H 2050 5500 50  0001 C CNN
F 3 "" H 2050 5500 50  0001 C CNN
	1    2050 5500
	1    0    0    -1  
$EndComp
Connection ~ 2450 4050
Connection ~ 2050 4150
Wire Wire Line
	6650 2750 6750 2750
Wire Wire Line
	6750 2750 6750 2250
Wire Wire Line
	5950 2350 5950 2250
Wire Wire Line
	5950 2250 5850 2250
Wire Wire Line
	6050 2350 6050 2250
Wire Wire Line
	6050 2250 5950 2250
Wire Wire Line
	6150 2350 6150 2250
Wire Wire Line
	6150 2250 6050 2250
Wire Wire Line
	6250 2350 6250 2250
Wire Wire Line
	6250 2250 6150 2250
$Comp
L power:GND #PWR?
U 1 1 5F4D21DC
P 7050 2450
AR Path="/5F4D21DC" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F4D21DC" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 7050 2200 50  0001 C CNN
F 1 "GND" H 7055 2277 50  0000 C CNN
F 2 "" H 7050 2450 50  0001 C CNN
F 3 "" H 7050 2450 50  0001 C CNN
	1    7050 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2350 5550 1250
Wire Wire Line
	5550 1250 5750 1250
Wire Wire Line
	5450 2350 5450 1250
Wire Wire Line
	5450 1250 5550 1250
Connection ~ 5550 1250
Wire Wire Line
	5350 2350 5350 1250
Wire Wire Line
	5350 1250 5450 1250
Connection ~ 5450 1250
Wire Wire Line
	5750 1250 5950 1250
Wire Wire Line
	5950 1250 5950 1300
Wire Wire Line
	5650 2350 5650 2150
Wire Wire Line
	5650 2150 7050 2150
Wire Wire Line
	7050 2050 5250 2050
Connection ~ 6250 2250
Connection ~ 6150 2250
Connection ~ 6050 2250
Connection ~ 5950 2250
Wire Wire Line
	5850 2250 5850 2350
Wire Wire Line
	7050 2250 7050 2450
Connection ~ 6750 2250
Wire Wire Line
	6750 2250 6250 2250
Wire Wire Line
	6750 2250 7050 2250
Wire Wire Line
	5250 2050 5250 2350
Wire Wire Line
	5150 2350 5150 1950
Wire Wire Line
	5150 1950 7050 1950
Wire Wire Line
	6650 2850 7050 2850
Wire Wire Line
	6650 2950 7050 2950
Text Label 6750 2950 0    50   ~ 0
MOSI
Text Label 6750 2850 0    50   ~ 0
MISO
Text Label 6750 2150 0    50   ~ 0
IRQ
Text Label 6750 2050 0    50   ~ 0
SCLK
Text Label 6750 1950 0    50   ~ 0
#SS
Wire Wire Line
	6650 4050 8000 4050
Wire Wire Line
	8000 4050 8000 5000
Wire Wire Line
	6650 3950 8500 3950
Wire Wire Line
	8500 3950 8500 5000
Wire Wire Line
	9000 3850 9000 5200
Wire Wire Line
	6650 3850 9000 3850
Text GLabel 4000 1950 0    50   Input ~ 0
XRST
Text GLabel 4000 2050 0    50   Input ~ 0
VST
Text GLabel 4000 2150 0    50   Input ~ 0
VCK
Text GLabel 4000 2250 0    50   Input ~ 0
HST
Text GLabel 4000 2750 0    50   Input ~ 0
HCK
Text GLabel 4000 2850 0    50   Input ~ 0
ENB
Text GLabel 4000 2950 0    50   Input ~ 0
RED0
Text GLabel 4000 3050 0    50   Input ~ 0
RED1
Text GLabel 4000 3150 0    50   Input ~ 0
GRN0
Text GLabel 4000 3250 0    50   Input ~ 0
GRN1
Text GLabel 4000 3350 0    50   Input ~ 0
BLU0
Text GLabel 4000 3450 0    50   Input ~ 0
BLU1
Text GLabel 4000 3650 0    50   Input ~ 0
VCOM
Text GLabel 4000 3750 0    50   Input ~ 0
XFRP
Text GLabel 1900 4050 0    50   Input ~ 0
VMDL
Text GLabel 1900 4150 0    50   Input ~ 0
VMDH
Text GLabel 7050 1950 2    50   Input ~ 0
#SS
Text GLabel 7050 2050 2    50   Input ~ 0
SCLK
Text GLabel 7050 2150 2    50   Input ~ 0
IRQ
Text GLabel 7050 2850 2    50   Input ~ 0
MISO
Text GLabel 7050 2950 2    50   Input ~ 0
MOSI
Wire Wire Line
	2050 4150 1900 4150
Wire Wire Line
	1900 4050 2450 4050
$Comp
L Connector:TestPoint TP1
U 1 1 5F3437C4
P 2850 4050
F 0 "TP1" H 2908 4168 50  0000 L CNN
F 1 "TestPoint" H 2908 4077 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 3050 4050 50  0001 C CNN
F 3 "~" H 3050 4050 50  0001 C CNN
	1    2850 4050
	1    0    0    -1  
$EndComp
Connection ~ 2850 4050
Wire Wire Line
	2850 4050 3200 4050
$Comp
L Connector:TestPoint TP2
U 1 1 5F345295
P 2850 4150
F 0 "TP2" H 2792 4176 50  0000 R CNN
F 1 "TestPoint" H 2792 4267 50  0000 R CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 3050 4150 50  0001 C CNN
F 3 "~" H 3050 4150 50  0001 C CNN
	1    2850 4150
	-1   0    0    1   
$EndComp
Connection ~ 2850 4150
Wire Wire Line
	2850 4150 4350 4150
Text Label 3600 4250 0    50   ~ 0
VC3
Text Label 4000 4900 0    50   ~ 0
VC2
Text Label 5150 5100 0    50   ~ 0
VC1
Text Label 4400 5000 0    50   ~ 0
CP3
Text Label 4750 5100 0    50   ~ 0
CP2
Text Label 4500 5600 0    50   ~ 0
CP1
Text Label 6150 5100 0    50   ~ 0
VD1
Text Label 6800 4250 0    50   ~ 0
#RESET
Text Label 6800 3950 0    50   ~ 0
OSC2
Text Label 6800 4050 0    50   ~ 0
OSC1
Text Label 6800 3850 0    50   ~ 0
TEST
$Comp
L Connector:TestPoint TP4
U 1 1 5F35520E
P 9500 2000
F 0 "TP4" H 9558 2118 50  0000 L CNN
F 1 "TestPoint" H 9558 2027 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 9700 2000 50  0001 C CNN
F 3 "~" H 9700 2000 50  0001 C CNN
	1    9500 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP3
U 1 1 5F357773
P 9000 2000
F 0 "TP3" H 9058 2118 50  0000 L CNN
F 1 "TestPoint" H 9058 2027 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 9200 2000 50  0001 C CNN
F 3 "~" H 9200 2000 50  0001 C CNN
	1    9000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR?
U 1 1 5F358195
P 8750 2000
AR Path="/5F358195" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F358195" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 8750 1850 50  0001 C CNN
F 1 "VDD" H 8767 2173 50  0000 C CNN
F 2 "" H 8750 2000 50  0001 C CNN
F 3 "" H 8750 2000 50  0001 C CNN
	1    8750 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F359625
P 9500 2000
AR Path="/5F359625" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F359625" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 9500 1750 50  0001 C CNN
F 1 "GND" H 9505 1827 50  0000 C CNN
F 2 "" H 9500 2000 50  0001 C CNN
F 3 "" H 9500 2000 50  0001 C CNN
	1    9500 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 2000 8750 2100
Wire Wire Line
	8750 2100 9000 2100
Wire Wire Line
	9000 2100 9000 2000
$Comp
L Connector:TestPoint TP5
U 1 1 5F360E0C
P 10000 2000
F 0 "TP5" H 10058 2118 50  0000 L CNN
F 1 "TestPoint" H 10058 2027 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 10200 2000 50  0001 C CNN
F 3 "~" H 10200 2000 50  0001 C CNN
	1    10000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F360E16
P 10000 2000
AR Path="/5F360E16" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F360E16" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 10000 1750 50  0001 C CNN
F 1 "GND" H 10005 1827 50  0000 C CNN
F 2 "" H 10000 2000 50  0001 C CNN
F 3 "" H 10000 2000 50  0001 C CNN
	1    10000 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4050 2450 5200
Wire Wire Line
	2050 4150 2050 5200
Connection ~ 3200 4050
Wire Wire Line
	3200 4050 3200 3850
Wire Wire Line
	3200 4050 4350 4050
Connection ~ 2750 4050
Wire Wire Line
	2750 4050 2750 3550
Wire Wire Line
	2750 4050 2850 4050
$EndSCHEMATC
