# Sony CRT Remote Controller

Control a Sony CRT TV using an infrared (IR) LED. It utilizes the `IRremote` library to send Sony-specific IR signals.

**Tested with:** KV-29FQ75D, KV-1434M3.

**Features:**

-   **Control various TV functions:** Power, volume, channel, playback, input source, and more.
-   **Predefined Sony codes:** Includes a comprehensive list of IR codes for common Sony CRT TV functions.
-   **Custom code support:** Send arbitrary IR codes in hexadecimal format.
-   **Multiple command execution:** Send sequences of commands separated by commas.
-   **Debug mode:** Provides feedback on the sent codes for troubleshooting. (Shows which code is sent)
-   **Interactive menu:** Access a menu via the Serial Monitor to list commands, view hex codes, and toggle debug mode.

**Hardware Requirements:**

-   Arduino board (e.g., Uno, Nano, Mega)
-   IR LED (Infrared Light Emitting Diode)
-   Jumper wires

**Wiring:**

-   IR + : Pin 3 (`IR_SEND_PIN`)
-   IR GND : GND

**Software Requirements:**

-   Arduino IDE
-   `IRremote` library (Install through the Library Manager in the Arduino IDE)

**Usage:**

1.  Upload the sketch to your Arduino board.
2.  Open the Serial Monitor (Tools > Serial Monitor) and set the baud rate to 9600.
3.  Use the following commands in the Serial Monitor:
    -   **Numbers 1 to 45:** Send the corresponding predefined Sony TV code.
    -   **`l`:** List all predefined codes with their hexadecimal values.
    -   **`m`:** Display the command menu.
    -   **`d`:** Toggle debug mode on/off.
    -   **`0xXXXX`:** Send a custom IR code in hexadecimal format (replace `XXXX` with the code).
    -   **Multiple commands:** Separate command numbers or hex codes with commas (e.g., `1,18,0x290`).

**Example:**

To turn on the TV and then increase the volume:

```
750,490 

```

**Customization:**

-   **`IR_SEND_PIN`:** Change this constant if you are using a different pin for the IR LED.
-   **`REPEAT_COUNT`:** Adjust the number of times the IR code is sent for each command.
-   **`DELAY_BETWEEN`:** Modify the delay between repeated IR code transmissions.
-   **`BIT_LENGTH`:** This is the bit length used for the Sony protocol. Usually, it should be 12.
-   **`COMMAND_DELAY`:** Change the delay between multiple commands.
-   **`SONY_CODES`:** Add or modify IR codes for your specific Sony TV model. You can find code lists online or use an IR receiver to capture codes from your original remote.

**Screenshot:**
  

  

![Menu](https://github.com/user-attachments/assets/d3661e12-ef58-4178-915b-4c4eb8427eb1)
