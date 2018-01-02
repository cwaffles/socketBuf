/*
Header file containing general macros
*/

#ifndef GENERAL_H
#define GENERAL_H

#define SENSOR_COUNT 16
#define LISTEN_QUANTUM 500
#define BUFFER_SIZE 8

/* Return codes */
typedef enum status_enum {
	STATUS_OK        =  0,
	STATUS_ERROR     = -1,
	STATUS_BAD_PARAM = -2,
	STATUS_CAN_FILTERED	= -3,
	STATUS_NO_DATA   = -4
} status_t;

typedef double canbus_t; //datatype used internally by the CAN Bus

typedef struct canbusData
{
	bool isValid;
	time_t timestamp;
	canbus_t data[SENSOR_COUNT];
	uint8_t len;
} cbd_t;

#endif
