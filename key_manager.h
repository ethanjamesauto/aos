#define KEYBOARD_PORT 0x60
#define BUFFER_SIZE 256

class KeyManager {
	char buffer[BUFFER_SIZE];
	int buffer_loc;
	
	char key_pressed;
		
	public:
		
	int chars_left;
	
	KeyManager();
	
	void update();
	
	char getKeyPress();
};