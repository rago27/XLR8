# Software


## Initial Design

We tried to create a custom controller for the robot to allow for better control. For example, the default controller which had been provided by the ERC did not allow for the robot to turn while simultaneously driving forward. It had to be brought to a stop before it could turn.


## Custom Design

 First, we tried sliders to allow the pilot to precisely control the RPM values of the individual motors. This would allow the bot to take corners in one continuous motion. We successfully made the webpage for the sliders but couldn’t figure out a way to transfer the input values from the webpage to the ESP as it required considerable modification of the given web code, and none of us had any web-dev experience. So, we coded a backup controller which relied on buttons only. However, it had a more significant number of buttons than the default controller for increased flexibility. Rather than a single button to control both motors, we would control each one individually. 

## Conclusion
 
 This made the robot a lot more responsive, making navigating the confined spaces of the XLR8 track without hitting the barriers a lot easier. Thus, we gained a competitive advantage by developing a custom controller.
