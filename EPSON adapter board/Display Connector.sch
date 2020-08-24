EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L power:GND #PWR?
U 1 1 5F50FC7B
P 8900 2450
AR Path="/5F50FC7B" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F50FC7B" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F50FC7B" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 8900 2200 50  0001 C CNN
F 1 "GND" V 8905 2322 50  0000 R CNN
F 2 "" H 8900 2450 50  0001 C CNN
F 3 "" H 8900 2450 50  0001 C CNN
	1    8900 2450
	0    1    1    0   
$EndComp
Text Label 8900 2050 0    50   ~ 0
XRST
Text Label 8900 1750 0    50   ~ 0
VST
Text Label 8900 1850 0    50   ~ 0
VCK
Text Label 8900 2550 0    50   ~ 0
HST
Text Label 8900 2650 0    50   ~ 0
HCK
Text Label 8900 1950 0    50   ~ 0
ENB
Text Label 8900 2750 0    50   ~ 0
RED0
Text Label 8900 2850 0    50   ~ 0
RED1
Text Label 8900 2950 0    50   ~ 0
GRN0
Text Label 8900 3050 0    50   ~ 0
GRN1
Text Label 8900 3150 0    50   ~ 0
BLU0
Text Label 8900 3250 0    50   ~ 0
BLU1
Text Label 8900 2150 0    50   ~ 0
VCOM
Text Label 8900 2250 0    50   ~ 0
XFRP
Text Label 8900 2350 0    50   ~ 0
VMDL
Text Label 8900 1550 0    50   ~ 0
VMDH
$Comp
L Connector_Generic:Conn_01x21 J7
U 1 1 5F5105CB
P 9450 2550
F 0 "J7" H 9530 2592 50  0000 L CNN
F 1 "Conn_01x21" H 9530 2501 50  0000 L CNN
F 2 "lC_lib:Molex-503566-2100" H 9450 2550 50  0001 C CNN
F 3 "~" H 9450 2550 50  0001 C CNN
	1    9450 2550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x24 J6
U 1 1 5F5166CA
P 3950 2650
F 0 "J6" H 4030 2642 50  0000 L CNN
F 1 "Conn_01x24" H 4030 2551 50  0000 L CNN
F 2 "lC_lib:FH34SRJ-24S-0.5SH(99)" H 3950 2650 50  0001 C CNN
F 3 "~" H 3950 2650 50  0001 C CNN
	1    3950 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1550 9250 1550
Wire Wire Line
	8900 1750 9250 1750
Wire Wire Line
	8900 1850 9250 1850
Wire Wire Line
	8900 1950 9250 1950
Wire Wire Line
	8900 2050 9250 2050
Wire Wire Line
	8900 2150 9250 2150
Wire Wire Line
	8900 2250 9250 2250
Wire Wire Line
	8900 2350 9250 2350
Wire Wire Line
	8900 2450 9250 2450
Wire Wire Line
	8900 2550 9250 2550
Wire Wire Line
	8900 2650 9250 2650
Wire Wire Line
	8900 2750 9250 2750
Wire Wire Line
	8900 2850 9250 2850
Wire Wire Line
	8900 2950 9250 2950
Wire Wire Line
	8900 3050 9250 3050
Wire Wire Line
	8900 3150 9250 3150
Wire Wire Line
	8900 3250 9250 3250
Wire Wire Line
	8900 3450 9250 3450
NoConn ~ 9250 1650
NoConn ~ 9250 3350
NoConn ~ 9250 3550
Text Label 8900 3450 0    50   ~ 0
VCOM
$Comp
L power:GND #PWR?
U 1 1 5F52C65B
P 3400 1550
AR Path="/5F52C65B" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F52C65B" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F52C65B" Ref="#PWR026"  Part="1" 
F 0 "#PWR026" H 3400 1300 50  0001 C CNN
F 1 "GND" V 3405 1422 50  0000 R CNN
F 2 "" H 3400 1550 50  0001 C CNN
F 3 "" H 3400 1550 50  0001 C CNN
	1    3400 1550
	0    1    1    0   
$EndComp
Text Label 3400 2150 0    50   ~ 0
XRST
Text Label 3400 1850 0    50   ~ 0
VST
Text Label 3400 1950 0    50   ~ 0
VCK
Text Label 3400 2650 0    50   ~ 0
HST
Text Label 3400 2750 0    50   ~ 0
HCK
Text Label 3400 2050 0    50   ~ 0
ENB
Text Label 3400 2850 0    50   ~ 0
RED0
Text Label 3400 2950 0    50   ~ 0
RED1
Text Label 3400 3050 0    50   ~ 0
GRN0
Text Label 3400 3150 0    50   ~ 0
GRN1
Text Label 3400 3250 0    50   ~ 0
BLU0
Text Label 3400 3350 0    50   ~ 0
BLU1
Text Label 3400 2250 0    50   ~ 0
VCOM
Text Label 3400 2350 0    50   ~ 0
XFRP
Text Label 3400 2450 0    50   ~ 0
VMDL
Text Label 3400 1650 0    50   ~ 0
VMDH
Wire Wire Line
	3400 1650 3750 1650
Wire Wire Line
	3400 1850 3750 1850
Wire Wire Line
	3400 1950 3750 1950
Wire Wire Line
	3400 2050 3750 2050
Wire Wire Line
	3400 2150 3750 2150
Wire Wire Line
	3400 2250 3750 2250
Wire Wire Line
	3400 2350 3750 2350
Wire Wire Line
	3400 2450 3750 2450
Wire Wire Line
	3400 1550 3750 1550
Wire Wire Line
	3400 2650 3750 2650
Wire Wire Line
	3400 2750 3750 2750
Wire Wire Line
	3400 2850 3750 2850
Wire Wire Line
	3400 2950 3750 2950
Wire Wire Line
	3400 3050 3750 3050
Wire Wire Line
	3400 3150 3750 3150
Wire Wire Line
	3400 3250 3750 3250
Wire Wire Line
	3400 3350 3750 3350
Wire Wire Line
	3400 3550 3750 3550
Text Label 3400 3550 0    50   ~ 0
VCOM
$Comp
L power:GND #PWR?
U 1 1 5F5368CE
P 3400 2550
AR Path="/5F5368CE" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F5368CE" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F5368CE" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 3400 2300 50  0001 C CNN
F 1 "GND" V 3405 2422 50  0000 R CNN
F 2 "" H 3400 2550 50  0001 C CNN
F 3 "" H 3400 2550 50  0001 C CNN
	1    3400 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 2550 3750 2550
NoConn ~ 3750 1750
NoConn ~ 3750 3450
$Comp
L power:GND #PWR?
U 1 1 5F54360B
P 3400 3850
AR Path="/5F54360B" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F54360B" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F54360B" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 3400 3600 50  0001 C CNN
F 1 "GND" V 3405 3722 50  0000 R CNN
F 2 "" H 3400 3850 50  0001 C CNN
F 3 "" H 3400 3850 50  0001 C CNN
	1    3400 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3850 3750 3850
$Comp
L power:GND #PWR?
U 1 1 5F546A30
P 3400 3750
AR Path="/5F546A30" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F546A30" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F546A30" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 3400 3500 50  0001 C CNN
F 1 "GND" V 3405 3622 50  0000 R CNN
F 2 "" H 3400 3750 50  0001 C CNN
F 3 "" H 3400 3750 50  0001 C CNN
	1    3400 3750
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3750 3750 3750
$Comp
L power:GND #PWR?
U 1 1 5F547A51
P 3400 3650
AR Path="/5F547A51" Ref="#PWR?"  Part="1" 
AR Path="/5F450EA8/5F547A51" Ref="#PWR?"  Part="1" 
AR Path="/5F5073FF/5F547A51" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 3400 3400 50  0001 C CNN
F 1 "GND" V 3405 3522 50  0000 R CNN
F 2 "" H 3400 3650 50  0001 C CNN
F 3 "" H 3400 3650 50  0001 C CNN
	1    3400 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3650 3750 3650
Text GLabel 8900 1550 0    50   Input ~ 0
VMDH
Text GLabel 8900 1750 0    50   Input ~ 0
VST
Text GLabel 8900 1850 0    50   Input ~ 0
VCK
Text GLabel 8900 1950 0    50   Input ~ 0
ENB
Text GLabel 8900 2050 0    50   Input ~ 0
XRST
Text GLabel 8900 2150 0    50   Input ~ 0
VCOM
Text GLabel 8900 2250 0    50   Input ~ 0
XFRP
Text GLabel 8950 2350 0    50   Input ~ 0
VMDL
Text GLabel 8900 2550 0    50   Input ~ 0
HST
Text GLabel 8900 2650 0    50   Input ~ 0
HCK
Text GLabel 8900 2750 0    50   Input ~ 0
RED0
Text GLabel 8900 2850 0    50   Input ~ 0
RED1
Text GLabel 8900 2950 0    50   Input ~ 0
GRN0
Text GLabel 8900 3050 0    50   Input ~ 0
GRN1
Text GLabel 8900 3150 0    50   Input ~ 0
BLU0
Text GLabel 8900 3250 0    50   Input ~ 0
BLU1
$EndSCHEMATC
