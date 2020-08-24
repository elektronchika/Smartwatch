EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Connector_Generic:Conn_01x08 J1
U 1 1 5F4D90C4
P 5050 2750
F 0 "J1" H 4968 3267 50  0000 C CNN
F 1 "Conn_01x08" H 4968 3176 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5050 2750 50  0001 C CNN
F 3 "~" H 5050 2750 50  0001 C CNN
	1    5050 2750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J2
U 1 1 5F4DCCCD
P 5050 3650
F 0 "J2" H 4968 4067 50  0000 C CNN
F 1 "Conn_01x06" H 4968 3976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5050 3650 50  0001 C CNN
F 3 "~" H 5050 3650 50  0001 C CNN
	1    5050 3650
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J4
U 1 1 5F4DDCD9
P 7450 3550
F 0 "J4" H 7530 3542 50  0000 L CNN
F 1 "Conn_01x08" H 7530 3451 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7450 3550 50  0001 C CNN
F 3 "~" H 7450 3550 50  0001 C CNN
	1    7450 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J3
U 1 1 5F4DF732
P 7450 2450
F 0 "J3" H 7530 2442 50  0000 L CNN
F 1 "Conn_01x10" H 7530 2351 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 7450 2450 50  0001 C CNN
F 3 "~" H 7450 2450 50  0001 C CNN
	1    7450 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J5
U 1 1 5F4E0E59
P 7450 4550
F 0 "J5" H 7530 4542 50  0000 L CNN
F 1 "Conn_01x08" H 7530 4451 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7450 4550 50  0001 C CNN
F 3 "~" H 7450 4550 50  0001 C CNN
	1    7450 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2850 5250 2850
$Comp
L power:GND #PWR025
U 1 1 5F4F71D2
P 6850 3000
F 0 "#PWR025" H 6850 2750 50  0001 C CNN
F 1 "GND" H 6855 2827 50  0000 C CNN
F 2 "" H 6850 3000 50  0001 C CNN
F 3 "" H 6850 3000 50  0001 C CNN
	1    6850 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3000 6850 2650
Wire Wire Line
	6850 2650 7250 2650
$Comp
L power:VDD #PWR023
U 1 1 5F4F950F
P 5450 1600
F 0 "#PWR023" H 5450 1450 50  0001 C CNN
F 1 "VDD" H 5467 1773 50  0000 C CNN
F 2 "" H 5450 1600 50  0001 C CNN
F 3 "" H 5450 1600 50  0001 C CNN
	1    5450 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2750 5250 2750
Wire Wire Line
	5250 2550 5450 2550
Connection ~ 5450 2550
Wire Wire Line
	5450 2550 5450 2750
Wire Wire Line
	5250 2450 5450 2450
Wire Wire Line
	5450 2450 5450 2550
$Comp
L power:GND #PWR024
U 1 1 5F4F443F
P 5450 3200
F 0 "#PWR024" H 5450 2950 50  0001 C CNN
F 1 "GND" H 5455 3027 50  0000 C CNN
F 2 "" H 5450 3200 50  0001 C CNN
F 3 "" H 5450 3200 50  0001 C CNN
	1    5450 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2950 5450 2950
Wire Wire Line
	5450 2950 5450 3050
Wire Wire Line
	5250 3050 5450 3050
Connection ~ 5450 3050
Wire Wire Line
	5450 3050 5450 3200
Wire Wire Line
	5250 3150 5700 3150
Wire Wire Line
	5700 2650 5250 2650
Text Label 5550 3150 0    50   ~ 0
VIN
Text Label 5550 2850 0    50   ~ 0
+5V
Text Label 5550 2650 0    50   ~ 0
RESET
Text Label 5400 3750 0    50   ~ 0
MOSI
Text Label 5400 3650 0    50   ~ 0
MISO
Text Label 5400 3550 0    50   ~ 0
IRQ
Text Label 5400 3850 0    50   ~ 0
SCLK
Text Label 5400 3950 0    50   ~ 0
#SS
Wire Wire Line
	5700 3950 5250 3950
Wire Wire Line
	5700 3850 5250 3850
Wire Wire Line
	5700 3750 5250 3750
Wire Wire Line
	5700 3650 5250 3650
Wire Wire Line
	5700 3550 5250 3550
Text GLabel 5700 3550 2    50   Input ~ 0
IRQ
Text GLabel 5700 3650 2    50   Input ~ 0
MISO
Text GLabel 5700 3750 2    50   Input ~ 0
MOSI
Text GLabel 5700 3850 2    50   Input ~ 0
SCLK
Text GLabel 5700 3950 2    50   Input ~ 0
#SS
$Comp
L Device:R R3
U 1 1 5F34EF6A
P 5450 2050
F 0 "R3" H 5520 2096 50  0000 L CNN
F 1 "10R" H 5520 2005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5380 2050 50  0001 C CNN
F 3 "~" H 5450 2050 50  0001 C CNN
	1    5450 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1600 5450 1900
Wire Wire Line
	5450 2200 5450 2450
Connection ~ 5450 2450
$EndSCHEMATC
