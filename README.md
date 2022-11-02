# BYD battery

Reverse engineering a 11kWh BYD HVM battery system, connected to a Fronius Gen24 Symo 5.0 Hybrid inverter

## Blockshema

Open .odg with e.g LibreOffice Draw

## Logfiles

Open with HHD Device Monitoring Studio
<https://www.hhdsoftware.com/device-monitoring-studio>

## Modbus RTU Registers

Open .ods file with your favourite spreadsheet tool (e.g. LibreOffice Calc)
The Battery has registers that the Fronius Inverter requests to read. There is also a write event that periodically appears (see logs)
During startup more bus activity is present compared to sitting idle.

Registers used:
101:["SI", 1],
103:["BY", "D ", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
119:["BY", "D ", "Ba", "tt", "er", "y-", "Bo", "x ", "Pr", "em", "iu", "m ", "HV", 0, 0, 0],
135:["5.", "0", 0, 0, 0, 0, 0, 0, "3.", "16", 0, 0, 0, 0, 0, 0],
151:["P0", "30", "T0", "20", "Z2", "00", "81", "03", "48", "80", "  ", "  ", 0, 0, 0, 0],
167:[1, 0],
201:[0, 0, 11059, 10240, 10240, 2336, 1600, 13312, 10, 13312, 10, 0, 0],
301:[1, 0, 128, 470, 10948, 514, 0, 8352, 0, 0, 2058, 0, 60, 70, 0, 0, 16, 22741, 0, 0, 13, 52064, 80, 9900],
401:[0]*20,
1001:[0]*100,
12289:[0]*768})

By setting up modbus registers like above, a battery will appear with 5% SOC in the Fronius inverters app.

![alt text](https://github.com/dalathegreat/byd_battery/blob/main/pictures/Battery.png)

## Pictures

Internal pictures from when BYD box was opened

## byd_be_connect

Windows program for connecting to BYD battery and doing firmware update and diagnostics over WIFI

## Manuals

Usefull manuals from BYD webpage.
