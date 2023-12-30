#include <stdio.h>
#include <stdlib.h>
#include <include/fractions.h>

int main()
{
    struct fraction *v_int;
    struct fraction *sensor_par;
    struct fraction *r2, *r3;
    struct fraction *max_read;
    struct fraction *analog_input;

    struct fraction *input_voltage;
    struct fraction *sensor_voltage;
    struct fraction *temperature;
    struct fraction *scaling;
    
    max_read = fraction_create(1024, 1);
    v_int = fraction_create(1093, 1000);
    sensor_par = fraction_create(1, 100);
    r2 = fraction_create(2176, 100);
    r3 = fraction_create(979, 100);
    analog_input = fraction_create(862, 1);

    scaling = fraction_clone(ZERO_FRACTION);
    input_voltage = fraction_clone(ZERO_FRACTION);
    sensor_voltage = fraction_clone(ZERO_FRACTION);
    temperature = fraction_clone(ZERO_FRACTION);

    // Compute scaling
    fraction_divide(scaling, r2, r3);
    fraction_sum(scaling, ONE_FRACTION, scaling);

    // Convert the analog reading to input voltage
    fraction_divide(input_voltage, v_int, max_read);
    fraction_multiply(input_voltage, analog_input, input_voltage);

    // Convert input voltage on voltage on the sensor
    fraction_multiply(sensor_voltage, scaling, input_voltage);

    fraction_divide(temperature, sensor_voltage, sensor_par);
    {
        struct fraction temp;
        fraction_from_llint(&temp, 273);
        fraction_subtract(temperature, temperature, &temp);
    }

    printf("Temperature: %.2f\n", fraction_to_double(temperature));
    
    return 0;
}