#define DEFAULT_IP "localhost:10000"
#define MAX_PACKET_LENGTH 8192 // arbitrary 
#define MAX_STRING_LENGTH 128  // also arbitrary

// Experiment Control Messages 0-1000 
#define GET_MESSAGE_COUNT 1
#define GET_MESSAGE_TIMESTAMP 2
#define TRIAL_START 3
#define TRIAL_END 4
#define START_RECORDING 5
#define STOP_RECORDING 6

// Haptics Messages 1000-2000
#define HAPTIC_DATA_STREAM 1000
#define HAPTICS_SET_ENABLED 1001
#define HAPTICS_SET_STIFFNESS 1008
#define HAPTICS_BOUNDING_PLANE 1009 
#define HAPTICS_CONSTANT_FORCE_FIELD 1010
#define HAPTICS_VISCOSITY_FIELD 1011
#define HAPTICS_REMOVE_FIELD_EFFECT 1012

// Graphics Messages are 2000-3000 
#define GRAPHICS_SET_ENABLED 2000
#define GRAPHICS_CHANGE_BG_COLOR 2001
#define GRAPHICS_MOVING_DOTS 2014
#define GRAPHICS_SHAPE_SPHERE 2050
#define GRAPHICS_SHAPE_TORUS 2051

// MSG_HEADER
typedef struct {
  int serial_no;
  int msg_type;
  double reserved;
  double timestamp;
} MSG_HEADER;

typedef struct {
  MSG_HEADER header;
  double posX;
  double posY;
  double posZ;
  double velX;
  double velY;
  double velZ;
  double forceX;
  double forceY;
  double forceZ;
  char collisions[4][MAX_STRING_LENGTH]; // 4 object collisions at a time
} M_HAPTIC_DATA_STREAM;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  int enabled;
} M_HAPTICS_SET_ENABLED;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  double stiffness;
} M_HAPTICS_SET_STIFFNESS;

typedef struct {
  MSG_HEADER header;
  double bWidth;
  double bHeight;
} M_HAPTICS_BOUNDING_PLANE;

typedef struct {
  MSG_HEADER header;
  char effectName[MAX_STRING_LENGTH];
  double direction;
  double magnitude;
} M_HAPTICS_CONSTANT_FORCE_FIELD;

typedef struct {
  MSG_HEADER header;
  char effectName[MAX_STRING_LENGTH];
  double viscosityMatrix[9];
} M_HAPTICS_VISCOSITY_FIELD;

typedef struct {
  MSG_HEADER header;
  char effectName[MAX_STRING_LENGTH];
} M_HAPTICS_REMOVE_FIELD_EFFECT;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  int enabled;
} M_GRAPHICS_SET_ENABLED;

typedef struct {
  MSG_HEADER header;
  float color[4]; 
} M_GRAPHICS_CHANGE_BG_COLOR;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  int numDots;
  double coherence;
  double direction;
  double magnitude;
} M_GRAPHICS_MOVING_DOTS;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  double radius;
  double localPosition[3];
  float color[4];
} M_GRAPHICS_SHAPE_SPHERE;

typedef struct {
  MSG_HEADER header;
  char objectName[MAX_STRING_LENGTH];
  double innerRadius;
  double outerRadius;
  double localPosition[3];
  float color[4];
} M_GRAPHICS_SHAPE_TORUS;
