# Dog Feeder Project

Using a repurposed cardboard box, this project is a fully-functioning dog feeder that dispenses dog food. The feeder dispenses dog food based on a user-set number of servo motor actuations called "cycles", where more cycles results in more dog food dispensed. Feedings can be triggered manually through a keypad input or automatically based on a user-set AM or PM time that matches the time retrieved from an NTP server over WiFi. An OLED I2C display shows information for dog feeding times while also having other features that reflect user input.

This project utilizes a repurposed cardboard box to create a fully-functioning dog feeder. The feeder dispenses dog food based on a user-set number of servo motor actuations, known as 'cycles'. Feedings can be triggered manually through a keypad input or automatically based on a user-set AM or PM time that matches the time retrieved from an NTP server over WiFi. An OLED I2C display provides essential information regarding dog feeding times, along with additional features that reflect user input.

The main features of this project include:
1. **Servo Motor Dog Food Dispensing.** When a feeding is triggered manually, or when a set time is reached, a servo motor will actuate to reveal an opening where food will be dispensed from. Concurrently, an additional servo motor inside of the dispenser will actuate a bumper to push food through the opening.
2. **GUI With an OLED I2C Display.** The main menu on this display shows the current day and time, as well as the current AM and PM feeding times that were set with the keypad. There is a graphical interface for when the "C" key is pressed to set the number of cycles, as well as instructions on how to clear and set cycles. During feedings, the display will indicate that a feeding is occurring, as well as how many feeding cycles there are left to complete.
3. **Keypad for Time and Feeding Cycle Input.** When the "A" key is pressed, users can enter an AM time down to the second that when reached, will trigger a feeding. The "B" key does the same thing, but for setting a PM time. When the "C" key is pressed, users can set how many feeding cycles (up to 20) that will be completed when a feeding is triggered--more cycles means more food dispensed. By pressing the "\*" key, users can trigger a manual feeding.
4. **Custom Wiring.** All of the wiring used in this project was measured between the ESP32 and the mounted component on the container (and crimped accordingly) to allow for much cleaner wiring. The number of visible wires on the outside of the container is minimal compared to the interior.

Please refer below for demonstrations of each feature:

## Servo Motor Dog Food Dispensing
Bottom View  | Top View
------------- | -------------
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>

Bottom View With Food | Top View With Food
------------- | -------------
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>

## Gui With an OLED I2C Display
Main Display  | Feeding Display
------------- | -------------
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>

## Keypad for Time and Feeding Cycle Input
AM Time Input  | PM Time Input
------------- | ------------- 
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>

Cycle Input  | Clear Cycle Input (Cannot Be Above Twenty)
------------- | -------------
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>

## Custom Wiring
Inside View  | Outside View
------------- | -------------
<img src="GIF HERE" width = 350 height = 700>  |  <img src="GIF HERE" width = 350 height = 700>
