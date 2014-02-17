#include "iio.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	struct iio_context *ctx;

	printf("Creating local IIO context\n");
	ctx= iio_create_local_context();
	if (!ctx)
		return EXIT_FAILURE;

	printf("IIO context created: %s\n", iio_context_get_name(ctx));

	unsigned int nb_devices = iio_context_get_devices_count(ctx);
	printf("IIO context has %u devices:\n", nb_devices);

	unsigned int i;
	for (i = 0; i < nb_devices; i++) {
		const struct iio_device *dev = iio_context_get_device(ctx, i);
		const char *name = iio_device_get_name(dev);
		printf("\t%s: %s\n", iio_device_get_id(dev), name ?: "" );

		unsigned int nb_channels = iio_device_get_channels_count(dev);
		printf("\t\t%u channels found:\n", nb_channels);

		unsigned int j;
		for (j = 0; j < nb_channels; j++) {
			struct iio_channel *ch = iio_device_get_channel(dev, j);
			const char *type_name;

			switch (iio_channel_get_type(ch)) {
			case IIO_CHANNEL_INPUT:
				type_name = "input";
				break;
			case IIO_CHANNEL_OUTPUT:
				type_name = "output";
				break;
			default:
				type_name = "unknown";
				break;
			}

			name = iio_channel_get_name(ch);
			printf("\t\t\t%s: %s (%s)\n",
					iio_channel_get_id(ch), name ?: "",
					type_name);

			unsigned int nb_attrs = iio_channel_get_attrs_count(ch);
			if (!nb_attrs)
				continue;

			printf("\t\t\t%u channel-specific attributes found:\n",
					nb_attrs);

			unsigned int k;
			for (k = 0; k < nb_attrs; k++) {
				printf("\t\t\t\tattr %u: %s\n", k,
						iio_channel_get_attr(ch, k));
			}
		}

		unsigned int nb_attrs = iio_device_get_attrs_count(dev);
		if (!nb_attrs)
			continue;

		printf("\t\t%u device-specific attributes found:\n", nb_attrs);
		for (j = 0; j < nb_attrs; j++) {
			printf("\t\t\tattr %u: %s\n", j,
					iio_device_get_attr(dev, j));
		}
	}

	iio_context_destroy(ctx);
	return EXIT_SUCCESS;
}
