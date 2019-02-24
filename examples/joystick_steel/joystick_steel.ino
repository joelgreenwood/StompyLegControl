/*
        Joystick firmware for Stompy  
        Created by Joel Greenwood, February 20, 2019.
        Released into the public domain -- so help you God.
*/

#define USE_COMANDO

#include <comando.h>
#include <Bounce.h>

// analog pins
#define X 14
#define Y 15
#define Z 16
#define SPEED 17
#define HEIGHT 18
#define AUX_1 19
#define AUX_2 20

// button pins
#define JOY_BUTTON_PIN 0
#define TOP_RIGHT_SWITCH_PIN 1
#define TOP_MIDDLE_SWITCH_PIN 2
//#define MOVE_BODY_WALK 2
//#define LOCK_SPEED_HEIGHT 2
#define ISOLATE_LEGS 3
#define LEG_MODE 7
#define AUX_BUTTON_1 22
#define AUX_BUTTON_2 23
#define LEG_1 8
#define LEG_2 9
#define LEG_3 10
#define LEG_4 11
#define LEG_5 12
#define LEG_6 21

// LED pins
#define LED_1 4
#define LED_2 5
#define LED_3 6

#define ANALOGRESOLUTION 8 // resolution of analog reads
#define ANLOGWRITEF 100000 // LED pwm frequency
#define REPORT_INTERVAL 50 // minimum report interval
#define ANALOG_THRESHOLD 5 // difference in value between last reading to trigger a report

byte led_pins[] = {LED_1, LED_2, LED_3};
int n_leds = sizeof(led_pins) / sizeof(led_pins[0]);

bool report_flag; // used to determine if a report is needed
elapsedMillis since_report; // timer to enforce minimum report interval

Comando com = Comando(Serial); // create comando instance and initialize for a stream over serial
CommandProtocol cmd = CommandProtocol(com); // create comando protocol 

// comando command names
#define CMD_RESET 0
#define CMD_AXIS 1
#define CMD_BUTTON 2
#define CMD_LED 3

// comando axis names
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_SPEED 3
#define AXIS_HEIGHT 4
#define AXIS_AUX_1 5
#define AXIS_AUX_2 6

// comando button names
#define BUTTON_JOY 0
#define BUTTON_BODY_WALK 1
#define BUTTON_SPEED_HEIGHT 2
#define BUTTON_ISOLATE_LEGS 3
#define BUTTON_LEG_MODE 4
#define BUTTON_AUX_1 5
#define BUTTON_AUX_2 6
#define BUTTON_LEG_1 7
#define BUTTON_LEG_2 8
#define BUTTON_LEG_3 9
#define BUTTON_LEG_4 10
#define BUTTON_LEG_5 11
#define BUTTON_LEG_6 12

// define the structure for analog channels
struct Axis {
 	byte pin;
 	byte value;
 	byte name;
 	bool report;
};

// initiate each analog channel with the analog structure
Axis axis[] = {
	{.pin=X, .value=0, .name=AXIS_X, .report=true},
	{.pin=Y, .value=0, .name=AXIS_Y, .report=true},
	{.pin=Z, .value=0, .name=AXIS_Z, .report=true},
	{.pin=SPEED, .value=0, .name=AXIS_SPEED, .report=true},
	{.pin=HEIGHT, .value=0, .name=AXIS_HEIGHT, .report=true},
	{.pin=AUX_1, .value=0, .name=AXIS_AUX_1, .report=true},
	{.pin=AUX_2, .value=0, .name=AXIS_AUX_2, .report=true},
};

int n_axis = sizeof(axis) / sizeof(Axis); // determine the number of analog channels for for loops.

// define the structure for button channels
struct Button {
	byte pin;
	byte value;
	byte name;
	bool report;
	Bounce bouncer;
};

// initiate each button with the button structure
Button buttons[] = {
	{.pin=JOY_BUTTON_PIN,     	.value=0, .name=BUTTON_JOY, 		.report=true, .bouncer=Bounce(JOY_BUTTON_PIN, 40)},
	{.pin=TOP_RIGHT_SWITCH_PIN, 	.value=0, .name=BUTTON_BODY_WALK, 	.report=true, .bouncer=Bounce(TOP_RIGHT_SWITCH_PIN, 40)},
	{.pin=TOP_MIDDLE_SWITCH_PIN,.value=0, .name=BUTTON_SPEED_HEIGHT,.report=true, .bouncer=Bounce(TOP_MIDDLE_SWITCH_PIN, 40)},
	{.pin=ISOLATE_LEGS, 	.value=0, .name=BUTTON_ISOLATE_LEGS,.report=true, .bouncer=Bounce(ISOLATE_LEGS, 40)},
	{.pin=LEG_MODE, 		.value=0, .name=BUTTON_LEG_MODE, 	.report=true, .bouncer=Bounce(LEG_MODE, 40)},
	{.pin=AUX_BUTTON_1, 	.value=0, .name=BUTTON_AUX_1, 		.report=true, .bouncer=Bounce(AUX_BUTTON_1, 40)},
	{.pin=AUX_BUTTON_2, 	.value=0, .name=BUTTON_AUX_2, 		.report=true, .bouncer=Bounce(AUX_BUTTON_2, 40)},
	{.pin=LEG_1, .value=0, .name=BUTTON_LEG_1, .report=true, .bouncer=Bounce(LEG_1, 40)},
	{.pin=LEG_2, .value=0, .name=BUTTON_LEG_2, .report=true, .bouncer=Bounce(LEG_2, 40)},
	{.pin=LEG_3, .value=0, .name=BUTTON_LEG_3, .report=true, .bouncer=Bounce(LEG_3, 40)},
	{.pin=LEG_4, .value=0, .name=BUTTON_LEG_4, .report=true, .bouncer=Bounce(LEG_4, 40)},
	{.pin=LEG_5, .value=0, .name=BUTTON_LEG_5, .report=true, .bouncer=Bounce(LEG_5, 40)},
	{.pin=LEG_6, .value=0, .name=BUTTON_LEG_6, .report=true, .bouncer=Bounce(LEG_6, 40)},

};

int n_buttons = sizeof(buttons) / sizeof(Button);  // determine the number of buttons

#ifdef USE_COMANDO
void on_reset(CommandProtocol *cmd);
void on_led(CommandProtocol *cmd);
#endif

void setup() {
	analogReadResolution(ANALOGRESOLUTION);
	analogWriteFrequency(LED_1, ANLOGWRITEF);
	analogWriteFrequency(LED_2, ANLOGWRITEF);
	Serial.begin(9600);
#ifdef USE_COMANDO
	com.register_protocol(0,cmd);
	cmd.register_callback(CMD_RESET, on_reset);
	cmd.register_callback(CMD_LED, on_led);
#else
  Serial.print("Found Axes:"); Serial.println(n_axis);
  Serial.print("Found Buttons:"); Serial.println(n_buttons);
#endif

	for (int i=0; i<n_buttons; i++) {
		pinMode(buttons[i].pin, INPUT_PULLUP);
	}
	analogWrite(LED_1, 5);
	analogWrite(LED_2, 5);
	analogWrite(LED_3, 5);
}

void loop() {

	com.handle_stream(); // update comando.

	// read all analog values and flag for report if value has changed more than the threashold
	for (int i=0; i<n_axis; i++) {
		byte current_value = analogRead(axis[i].pin);
		if (abs(axis[i].value - current_value) >= ANALOG_THRESHOLD) {
			axis[i].value = current_value;
			axis[i].report = true;
			report_flag = true;
		}
	}
	
	// read all buttons and flag if there is a value change
	for (int i=0; i<n_buttons; i++) {
		//byte current_value = 0;
		if (buttons[i].bouncer.update()) {
			//current_value = buttons[i].risingEdge();
			buttons[i].value = buttons[i].bouncer.risingEdge();
			buttons[i].report = true;
			report_flag = true;
		}
		//byte current_value = buttons[i].bouncer.read();
		//byte current_value = digitalRead(buttons[i].pin);
		// if (current_value != buttons[i].value) {
		// 	buttons[i].value = current_value;
		// 	buttons[i].report = true;
		// 	report_flag = true;
		// }
	}

	// If there hasn't been a report recently (set by report interval) then report any changes.
	if ((since_report >= REPORT_INTERVAL) && (report_flag == true)) {
		since_report = 0; 		// reset the report interval counter
		report_flag = false; 	// reset the make report flag
		
		for (int i=0; i<n_axis; i++) {
			if (axis[i].report == true) {
#ifdef USE_COMANDO
				cmd.start_command(CMD_AXIS);
				cmd.add_arg((byte)axis[i].name);
				cmd.add_arg(axis[i].value);
				cmd.finish_command();
#else
        Serial.print("Axis,");
        Serial.print(axis[i].name);
        Serial.print(",");
        Serial.println(axis[i].value);
#endif
				axis[i].report = false;
			}
		}

		for (int i=0; i<n_buttons; i++) {
			if (buttons[i].report == true) {
#ifdef USE_COMANDO
				cmd.start_command(CMD_BUTTON);
				cmd.add_arg((byte)buttons[i].name);
				cmd.add_arg(1 - buttons[i].value);
				cmd.finish_command();
#else
        Serial.print("Button,");
        Serial.print(buttons[i].name);
        Serial.print(",");
        Serial.println(buttons[i].value);
#endif
				buttons[i].report = false;
			}
		}
	} 
}

#ifdef USE_COMANDO
void on_reset(CommandProtocol *cmd) {

	since_report = REPORT_INTERVAL;

	for (int i=0; i<n_axis; i++) {
		axis[i].report = true;
	}

	for (int i=0; i<n_buttons; i++) {
		buttons[i].report = true;
	}	

  report_flag = true;
}

void on_led(CommandProtocol *cmd) {
	if(!cmd->has_arg()) {
		return;
	}
	byte led_index = cmd->get_arg<byte>();
	if (led_index >= n_leds) {
		return;
	}
	if(!cmd->has_arg()) {
		return;
	}
	byte led_value = cmd->get_arg<byte>();

	analogWrite(led_pins[led_index], led_value);
}
#endif

