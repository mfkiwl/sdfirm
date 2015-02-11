#include "acpi_int.h"

uint8_t acpi_checksum_calc(uint8_t *buffer, uint32_t length)
{
	uint8_t sum = 0;

	while (length--) {
		sum += ACPI_DECODE8(buffer);
		buffer++;
	}

	return -sum;
}

void acpi_encode_generic_address(struct acpi_generic_address *generic_address,
				 uint8_t space_id,
				 uint64_t address64,
				 uint16_t bit_width)
{
	ACPI_ENCODE64(&generic_address->address, address64);
	ACPI_ENCODE8(&generic_address->space_id, space_id);
	ACPI_ENCODE8(&generic_address->bit_width,
		     (uint8_t)(bit_width > 255 ? (255) : bit_width));
	ACPI_ENCODE8(&generic_address->bit_offset, 0);
	ACPI_ENCODE8(&generic_address->access_width, 0); /* Access width ANY */
}

void acpi_state_push(union acpi_state **head, union acpi_state *state)
{
	state->common.next = *head;
	*head = state;

	return;
}

union acpi_state *acpi_state_pop(union acpi_state **head)
{
	union acpi_state *state;

	state = *head;
	if (state)
		*head = state->common.next;

	return state;
}

static void __acpi_object_init(struct acpi_object_header *object,
			       uint8_t type, acpi_release_cb release)
{
	object->descriptor_type = type;
	acpi_reference_set(&object->reference_count, 1);
	object->release = release;
}

struct acpi_object_header *acpi_object_create(uint8_t type,
					      acpi_size_t size,
					      acpi_release_cb release)
{
	struct acpi_object_header *object;

	object = acpi_os_allocate_zeroed(size);
	if (object)
		__acpi_object_init(object, type, release);

	return object;
}

void acpi_object_delete(struct acpi_object_header *object)
{
	if (!object)
		return;

	if (!acpi_reference_dec_and_test(&object->reference_count)) {
		if (object->release)
			object->release(object);
		acpi_os_free(object);
	}
}

static void __acpi_state_init(union acpi_state *state, uint8_t type,
			      acpi_release_cb release)
{
	state->common.object_type = type;
	state->common.next = NULL;
	state->common.release_state = release;
}

static void acpi_state_release(struct acpi_object_header *object)
{
	union acpi_state *state =
		ACPI_CAST_PTR(union acpi_state, object);

	if (state->common.release_state)
		state->common.release_state(object);
}

union acpi_state *acpi_state_create(uint8_t type,
				    acpi_size_t size,
				    acpi_release_cb release)
{
	struct acpi_object_header *object;
	union acpi_state *state = NULL;

	object = acpi_object_create(ACPI_DESC_TYPE_STATE,
				    size,
				    acpi_state_release);
	state = ACPI_CAST_PTR(union acpi_state, object);
	if (state)
		__acpi_state_init(state, type, release);

	return state;
}
