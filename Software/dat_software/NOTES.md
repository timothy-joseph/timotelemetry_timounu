### Dynamic Memory allocation

The rule "Dynamic allocation of memory is forbidden" has not been followed as
of writing this note, but the rule "Do not use dynamic memory allocation after
initialization" has been followed, except in one place.

Explanation:

- Besides the `can_receive_thread`, all other threads are created using the
  standard ESP's FreeRTOS `xTaskCreatePinnedToCore` which uses dynamic memory
  allocation (however, the stack's size does not change during operation)
  instead of `xTaskCreateStaticPinnedToCore`
- The esp idf's "release/v5.1" i2c driver uses dynamic memory allocation to
  create the command links

### MQTT packet

A mqtt packet is formed like this:

- 4 bytes representing the timestamp since logging began (us)
- 2 bytes representing the can id (made-up can id if it's not actually a can
  message)
- 1 byte representing data size
- a maximum of 8 data bytes

with 500-700 can messages per second => 60000 to 84000 bits per second of
packet data only. I might need to make the sim module's uart run at a higher
baud rate (higher than 400000)

The packet is in big-endian format.

### Driver configuration

When i set out to create the drivers for all of the peripherals of the DAT
system, i wanted them to be configurable in a similar way to how the drivers of
the esp-idf are configured, however due to time constraints, i have chosen to
make them configurable in another way: similar to how suckless software is
configured. All (configurable) components/drivers have a `component_config.h`
file which contains macros and/or variables to configure the driver's used
pins, stack sizes, thread priorities, queue sizes etc. These macros/variables
are explained briefly in comments above them and/or have suggestive names

### Sim driver

The sim driver has 3 components:

1. One to send commands
2. One to receive responses, interpret them and call callback functions (that
are defined within the same source file [this is another part that was rushed;
initially i wanted a system that binds responses to external callback
functions]);
3. One that enqueues the "AT+CGPSINFO" command every ??? (configurable) ms in
order to send it to the mqtt server

### RTC (MAX31331) chip's SDA and SCL pins

I accidentally swapped the SDA and SCL pins within the schematic when designing
it. That's why the pins are the swapped in software.

### Component initialization dependencies

rtc driver doesn't depend on anything
lora driver doesn't depend on anything
sim driver depends on rtc driver
microsd driver depends on rtc driver, sim driver
logging driver depends on sim driver, microsd driver, rtc driver, lora driver
can driver depends on logging

additionally the rtc driver has only one function that depends on the can
driver and so does the sim driver.

So the component initialization order is as follows:
1. can driver
2. rtc driver
3. sim driver
3.5 wait for the sim module to send the RDY flag (with a timeout)
4. lora driver
5. microsd driver and file
6. logging driver
7. can thread

`rtc_start_periodic_can_msg()` needs to be started from inside the receive
thread of the can driver

It might be a good idea to wait for the sim module to be ready before
initializing the microsd component. So the places that will wait for the sim
module are:

1. The `sim_commands_queue_thread` thread
2. `dat_setup()`

These places will have a timeout

\*: the module already `sim_commands_queue_thread()` already waits for the RDY
flag to be sent so it doesn't send any commands to the module before it's
initialized, but i think it's a good idea to wait in the `dat_setup()` function
also so the whole system is somewhat synced

