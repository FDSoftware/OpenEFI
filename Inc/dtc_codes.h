/** @file */

#ifndef DTC_CODES_H
#define DTC_CODES_H

// All DTC's codes used by OpenEFI

// DTC init:
#define NEW_DTC new uint8_t[5]

// System:

#define DTC_SYSTEM_VOLTAGE_MALFUNCTION      { 0x50, 0x30, 0x35, 0x36, 0x30 } // or P0560
#define DTC_SYSTEM_VOLTAGE_UNSTABLE         { 0x50, 0x30, 0x35, 0x36, 0x31 } // or P0561
#define DTC_SYSTEM_VOLTAGE_LOW              { 0x50, 0x30, 0x35, 0x36, 0x32 } // or P0562
#define DTC_SYSTEM_VOLTAGE_HIGH             { 0x50, 0x30, 0x35, 0x36, 0x33 } // or P0563
#define DTC_MEMORY_CHECKSUM_ERROR           { 0x50, 0x30, 0x36, 0x30, 0x31 } // or P0601
#define DTC_ECM_PROGRAMMING_ERROR           { 0x50, 0x30, 0x36, 0x30, 0x32 } // or P0602
#define DTC_ECM_PROCESSOR_FAULT             { 0x50, 0x30, 0x36, 0x30, 0x36 } // or P0606
#define DTC_ECM_SINC_FAULT                  { 0x50, 0x31, 0x31, 0x37, 0x37 } // or P1177

// Ignition / Injection

#define DTC_MISFIRE_EMERGENCY_STOP          { 0x50, 0x30, 0x33, 0x36, 0x33 } // or P0363
#define DTC_TURBO_PRESSURE_LOW              { 0x50, 0x31, 0x32, 0x34, 0x37 } // or P1247
#define DTC_TURBO_NO_PRESSURE               { 0x50, 0x31, 0x32, 0x34, 0x38 } // or P1248
#define DTC_RPM_LIMITER_REACHED             { 0x50, 0x31, 0x32, 0x37, 0x30 } // or P1270

#define DTC_COIL_A_MALFUNCTION              { 0x50, 0x30, 0x33, 0x35, 0x31 } // or P0351
#define DTC_COIL_B_MALFUNCTION              { 0x50, 0x30, 0x33, 0x35, 0x32 } // or P0352
#define DTC_COIL_C_MALFUNCTION              { 0x50, 0x30, 0x33, 0x35, 0x33 } // or P0353
#define DTC_COIL_D_MALFUNCTION              { 0x50, 0x30, 0x33, 0x35, 0x34 } // or P0354

#define DTC_INJECTOR_CIL1_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x31 } // or P1201
#define DTC_INJECTOR_CIL2_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x32 } // or P1202
#define DTC_INJECTOR_CIL3_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x33 } // or P1203
#define DTC_INJECTOR_CIL4_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x34 } // or P1204
#define DTC_INJECTOR_CIL5_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x35 } // or P1205
#define DTC_INJECTOR_CIL6_OPEN              { 0x50, 0x31, 0x32, 0x30, 0x36 } // or P1206

#define DTC_INJECTOR_PRESSURE_FAULT         { 0x50, 0x31, 0x32, 0x30, 0x39 } // or P1209
#define DTC_INJECTOR_PRESSURE_NOT_DETECTED  { 0x50, 0x31, 0x32, 0x31, 0x32 } // or P1212

#define DTC_IGNITION_CIL1_ACTIVATION_FAULT  { 0x50, 0x31, 0x33, 0x37, 0x31 } // or P1371
#define DTC_IGNITION_CIL2_ACTIVATION_FAULT  { 0x50, 0x31, 0x33, 0x37, 0x32 } // or P1372
#define DTC_IGNITION_CIL3_ACTIVATION_FAULT  { 0x50, 0x31, 0x33, 0x37, 0x33 } // or P1373
#define DTC_IGNITION_CIL4_ACTIVATION_FAULT  { 0x50, 0x31, 0x33, 0x37, 0x34 } // or P1374
#define DTC_IGNITION_CIL5_ACTIVATION_FAULT  { 0x50, 0x31, 0x33, 0x37, 0x35 } // or P1375
#define DTC_IGNITION_CIL6_ACTIVATION_FAULT  { 0x50, 0x31, 0x32, 0x37, 0x36 } // or P1376

// Sensors:

#define DTC_CPK_MALFUNCTION                 { 0x50, 0x30, 0x33, 0x33, 0x35 } // or P0335
#define DTC_CPK_MALFUNCTION_INTERMITTENT    { 0x50, 0x30, 0x33, 0x33, 0x39 } // or P0339

#define DTC_FUEL_LEVEL_MALFUNCTION          { 0x50, 0x30, 0x34, 0x36, 0x30 } // or P0460
#define DTC_COOLING_FAN_A_MALFUNCTION       { 0x50, 0x30, 0x34, 0x38, 0x30 } // or P0480

#define DTC_IAT_SENSOR_INTERMITTENT         { 0x50, 0x31, 0x31, 0x30, 0x39 } // or P1109
#define DTC_IAT_SENSOR_HIGH                 { 0x50, 0x31, 0x31, 0x31, 0x33 } // or P1111
#define DTC_IAT_SENSOR_LOW                  { 0x50, 0x31, 0x31, 0x31, 0x32 } // or P1112
#define DTC_IAT_SENSOR_OPEN                 { 0x50, 0x31, 0x31, 0x31, 0x33 } // or P1113

// TODO: este sensor no tiene DTC para open?
#define DTC_ECT_SENSOR_LOW                  { 0x50, 0x31, 0x31, 0x31, 0x34 } // or P1114
#define DTC_ECT_SENSOR_HIGH                 { 0x50, 0x31, 0x31, 0x31, 0x35 } // or P1115
#define DTC_ECT_SENSOR_INTERMITTENT         { 0x50, 0x31, 0x31, 0x31, 0x37 } // or P1117
#define DTC_ECT_OUT_OF_RANGE                { 0x50, 0x31, 0x31, 0x31, 0x36 } // or P1116

#define DTC_MAP_SENSOR_LOW                  { 0x50, 0x31, 0x31, 0x31, 0x38 } // or P1118
#define DTC_MAP_SENSOR_HIGH                 { 0x50, 0x31, 0x31, 0x32, 0x35 } // or P1119

#define DTC_TPS_SENSOR_INTERMITTENT         { 0x50, 0x31, 0x31, 0x32, 0x35 } // or P1125
#define DTC_TPS_OUT_OF_RANGE                { 0x50, 0x31, 0x31, 0x32, 0x33 } // or P1123
#define DTC_TPS_SELF_TEST_FAULT             { 0x50, 0x31, 0x31, 0x32, 0x34 } // or P1124
#define DTC_TPS_A_MALFUNCTION               { 0x50, 0x30, 0x31, 0x32, 0x30 } // or P0120
#define DTC_TPS_DUAL_MALFUNCTION            { 0x50, 0x31, 0x31, 0x32, 0x36 } // or P1126

#define DTC_FUEL_STEPPER_MALFUNCTION        { 0x50, 0x31, 0x31, 0x35, 0x39 } // or P1159

#define DTC_CAM_SENSOR_MALFUNCTION          { 0x50, 0x31, 0x31, 0x37, 0x34 } // or P1174
#define DTC_CAM_CONTROL_FAULT               { 0x50, 0x31, 0x31, 0x37, 0x35 } // or P1175
#define DTC_CAM_CALIBRATION_FAULT           { 0x50, 0x31, 0x31, 0x37, 0x36 } // or P1176

#define DTC_ALTERNATOR_LOAD_HIGH            { 0x50, 0x31, 0x32, 0x34, 0x34 } // or P1244
#define DTC_ALTERNATOR_LOAD_LOW             { 0x50, 0x31, 0x32, 0x34, 0x35 } // or P1245
#define DTC_ALTERNATOR_LOAD_FAULT           { 0x50, 0x31, 0x32, 0x34, 0x36 } // or P1246

#endif