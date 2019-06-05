This is the API documentation.

# Web app client API

## **`/sched`**
Control if and when the screen will automatically turn off and on.  
Arguments:
- `enable`: If set to `0` this functionality will be disabled. Otherwise it will be enabled.
- `offhour`: The hour at which the screen will turn off. The format is "HH:MM". Make sure to take into account the URL percent encoding.
- `onhour`: The hour at which the screen will turn on. The format is "HH:MM". Make sure to take into account the URL percent encoding.

Example:  `/sched?enable=0`
