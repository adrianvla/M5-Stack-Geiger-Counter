void draw();

void signalHandler();

uint16_t getDanger(int);

uint16_t ConvertToRGB565(uint32_t);

unsigned int MovingAverage(unsigned int*);

void nullRad();


//wifi
boolean restoreConfig();

boolean checkConnection();

void startWebServer();

void setupMode();

String makePage(String, String);

String urlDecode(String);