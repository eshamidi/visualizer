#include <stdlib.h>
#include <stdint.h>

typedef enum {
	EFFECT_ON,
	EFFECT_OFF,
	EFFECT_PROPERTY
} protocol_message_type_e;

typedef enum {
	EFFECT_NAME
} protocol_message_effect_type_e;

typedef enum {
	PROPERTY_X_LOCATION,
	PROPERTY_Y_LOCATION,
	PROPERTY_WIDTH,
	PROPERTY_HEGIHT
} protocol_message_effect_property_e;

typedef struct {
	protocol_message_effect_property_e property;
	int32_t                            value;
} protocol_message_effect_property_t;

typedef struct {
	protocol_message_type_e            message_type;
	protocol_message_effect_type_e     effect_type;
	protocol_message_effect_property_t property;
} protocol_message_t;
