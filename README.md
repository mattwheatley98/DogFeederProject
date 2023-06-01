# Dog Feeder Project

Using a repurposed cardboard box, this project is a fully-functioning dog feeder that dispenses dog food. The feeder dispenses dog food based on a user-set number of servo motor actuations called "cycles", where more cycles results in more dog food dispensed. Feedings can be triggered manually through a keypad input or automatically based on a user-set AM or PM time that matches the time retrieved from an NTP server over WiFi. An OLED I2C display shows information for dog feeding times while also having other features that reflect user input.

The main features of this project include:
1. **Servo Motor Dog Food Dispensing.** When a feeding is triggered manually, or when a set time is reached, a servo motor will actuate to reveal an opening where food will be dispensed from. Concurrently, an additional servo motor inside of the dispenser will actuate a bumper to push food through the opening.
2. **GUI With an OLED I2C Display.** The main menu on this display shows the current day and time, as well as the current AM and PM feeding times that were set with the keypad. There is a graphical interface for when the "C" key is pressed to set the number of cycles, as well as instructions on how to clear and set cycles. During feedings, the display will indicate that a feeding is occurring, as well as how many feeding cycles there are left to complete.
3. **Keypad for Time and Feeding Cycle Input.** When the "A" key is pressed, users can enter an AM time down to the second that when reached, will trigger a feeding. The "B" key does the same thing, but for setting a PM time. When the "C" key is pressed, users can set how many feeding cycles (up to 20) that will be completed when a feeding is triggered--more cycles means more food dispensed. By pressing the "\*" key, users can trigger a manual feeding.
4. **Custom Wiring.** All of the wiring used in this project was measured between the ESP32 and the mounted component on the container (and crimped accordingly) to allow for much cleaner wiring. The number of visible wires on the outside of the container is minimal compared to the interior.

Please refer below for demonstrations of each feature:

## Servo Motor Dog Food Dispensing
Dispenser Bottom View  | Dispenser Interior View
------------- | -------------
<video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/a7e2594f-a638-4d8a-a059-5182f93cad6d" width = 350 height = 700>  |  <video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/1b1ed19d-edc2-4e5a-a4d0-94e60f004345" width = 350 height = 700>

Dispenser Bottom View With Food | Dispenser Interior View With Food
------------- | -------------
<video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/5355436c-a451-480f-abcc-7063f2a4b282" width = 350 height = 700>  |  <video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/41929ad5-1d2b-4b6f-8303-fa8b9582ef92" width = 350 height = 700>

## Gui With an OLED I2C Display
Main Display  | Feeding Display
------------- | -------------
<video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/338d4b50-2ee8-41c4-a095-ec81d2848037" width = 350 height = 700>  |  <video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/cc475929-43db-46f0-a914-dc871d3ef205" width = 350 height = 700>

## Keypad for Time and Feeding Cycle Input
AM Time Input  | PM Time Input
------------- | ------------- 
<video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/6cca6f23-390f-4df5-a4c6-4cd70d6571af" width = 350 height = 700>  |  <video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/09fd565d-faba-40c9-8f9b-f44c6ff0ac68" width = 350 height = 700>

Timed Feeding  | Cycle Input
------------- | -------------
<video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/eeeee752-39c6-48b5-a4c0-4f23549c6279" width = 350 height = 700>  |  <video src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/a7f8c359-3b06-465d-bc76-71eb2b5f7f2c" width = 350 height = 700>

## Custom Wiring
Front View  | Back View
------------- | -------------
<img src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/8ceba52b-055f-46bd-bf47-3d52b0bd453a" width = 350 height = 700>  |  <img src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/02386abf-e8d4-494d-9c91-f922450209e8" width = 350 height = 700>

Interior View  | Bottom View
------------- | -------------
<img src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/d84f628a-f0d1-4f96-bef3-596bb5e7c91b" width = 350 height = 700>  |  <img src="https://github.com/mattwheatley98/DogFeederProject/assets/113391095/9c88be1c-c0b7-434d-b9e4-c116a7f9234e" width = 350 height = 700>
