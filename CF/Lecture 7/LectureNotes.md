# Lecture 7
# Concurrent and Real-Time systems

Hassan Gomaa SBN-13 978-0201525779
ISBN-10 : 020152

## Cruise control

Decomposition of the system into critical and non critical tasks, making sure that the non-critical tasks does not interfere with critical tasks.

Using state machines to  control the car

Device to device driver signals (Vær ekstra opmærksom)

### Async object

Triggers even or discrete data fllow

Slide 16 blue cirkel Fifo buffer to hold data send by tasks, making sure that a task can take the resource when it is needed.

Be careful when not adding a buffer, the argument as to why it was a good idea to do it bufferless have to be really strong.

When applying buffers, it is important to calculate the needed buffer time to avoid data loss.


