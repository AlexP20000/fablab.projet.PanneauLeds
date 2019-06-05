This is the API documentation.

# Web app client API

All the requests are made using the GET method unless noted otherwise.

## Auto sleep
Control if and when the screen will automatically turn off and on.

- **URL**
  `/sched`

- **URL Parameters**
  `enable`: If set to `0` this functionality will be disabled. Otherwise it will be enabled.
  
  `offhour`: The hour at which the screen will turn off.
  
  `onhour`: The hour at which the screen will turn on.

- **Notes**
   The format for hour is in for of "HH:MM". Make sure to take into account the URL percent encoding.

- **Examples**
  Disable this functionality: `/sched?enable=0`
