#define DEFAULT_IP "localhost:10000"
#define MAX_PACKET_LENGTH 4096 // arbitrary 

// Experiment Control Messages 0-1000 
#define CONTROL_SIMULATION_STARTED 10
#define CONTROL_SIMULATION_FINISHED 11

// Haptics Messages 1000-2000
#define HAPTIC_DATA_STREAM 1000
#define HAPTICS_TOOL_CONNECTED 1001
#define HAPTICS_EFFECT_MAGNET 1002
#define HAPTICS_EFFECT_STICKSLIP 1003
#define HAPTICS_EFFECT_SURFACE 1004
#define HAPTICS_EFFECT_VIBRATION 1005
#define HAPTICS_EFFECT_VISCOSITY 1006
#define HAPTICS_SET_STIFFNESS 1007

// Graphics Messages are 2000-3000 
/*#define GRAPHICS_COLORF 2000
#define GRAPHICS_COLORB 2001
#define GRAPHICS_DISPLAY_LIST 2002
#define GRAPHICS_FOG 2003
#define GRAPHICS_FONT 2004
#define GRAPHICS_GENERIC_ARRAY 2005
#define GRAPHICS_IMAGE 2006
#define GRAPHICS_MULTI_IMAGE 2007
#define GRAPHICS_POINT_ARRAY 2008
#define GRAPHICS_RENDER_OPTIONS 2009
#define GRAPHICS_SEGMENT_ARRAY 2010
#define GRAPHICS_TRIANGLE_ARRAY 2011
#define GRAPHICS_VERTEX_ARRAY_OPTIONS 2012
#define GRAPHICS_VERTEX_ARRAY 2013
#define GRAPHICS_VIDEO 2014*/
#define GRAPHICS_COLOR_FTOB 2015
#define GRAPHICS_COLOR_BTOF 2016
#define GRAPHICS_LOOKAT 2017
#define GRAPHICS_DRAW_FRAME 2018
#define GRAPHICS_DRAW_WIRE_BOX 2019
#define GRAPHICS_DRAW_SPHERE 2020
#define GRAPHICS_DRAW_ARROW 2021
#define GRAPHICS_DRAW_SOLID_TORUS 2022
#define GRAPHICS_CREATE_PLANE 2023
#define GRAPHICS_CREATE_PLANE2 2024
#define GRAPHICS_CREATE_MAP 2025
#define GRAPHICS_CREATE_DISK 2026
#define GRAPHICS_CREATE_PANEL 2027
#define GRAPHICS_CREATE_PANEL2 2028
#define GRAPHICS_CREATE_BOX 2029
#define GRAPHICS_CREATE_TEAPOT 2030
#define GRAPHICS_CREATE_CYLINDER 2031
#define GRAPHICS_CREATE_PIPE 2032
#define GRAPHICS_CREATE_SPHERE 2033
#define GRAPHICS_CREATE_ELLIPSOID 2034
#define GRAPHICS_CREATE_RING 2035
#define GRAPHICS_CREATE_RING_SECTION 2036
#define GRAPHICS_CREATE_CONE 2037
#define GRAPHICS_CREATE_SQUARE_PYRAMID 2038
#define GRAPHICS_CREATE_ARROW 2039
#define GRAPHICS_CREATE_CIRCULAR_ARROW 2040
#define GRAPHICS_CREATE_BEZIER_PATCH 2041
//#define GRAPHICS_MESH 2042
//#define GRAPHICS_MULTIMESH 2043
//#define GRAPHICS_MULTIPOINT 2044
//#define GRAPHICS_MULTISEGMENT 2045
//#define GRAPHICS_SHAPE_BOX 2046
//#define GRAPHICS_SHAPE_CYLINDER 2047
//#define GRAPHICS_SHAPE_ELLIPSOID 2048
//#define GRAPHICS_SHAPE_LINE 2049
#define GRAPHICS_SHAPE_SPHERE 2050
#define GRAPHICS_SHAPE_TORUS 2051
//#define GRAPHICS_VOXEL_OBJECT 2052

// MSG_HEADER
typedef struct {
  int serial_no;
  int msg_type;
  double reserved;
  double timestamp;
} MSG_HEADER;

typedef struct {
  MSG_HEADER header;
} M_CONTROL_SIMULATION_STARTED;

typedef struct {
  MSG_HEADER header;
} M_CONTROL_SIMULATION_FINISHED;

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
} M_HAPTIC_DATA_STREAM;

typedef struct {
  MSG_HEADER header;
} M_HAPTICS_TOOL_CONNECTED;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
} M_HAPTICS_EFFECT_MAGNET;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
} M_HAPTICS_EFFECT_STICKSLIP;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
} M_HAPTICS_EFFECT_SURFACE;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
} M_HAPTICS_EFFECT_VIBRATION;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
} M_HAPTICS_EFFECT_VISCOSITY;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
  double stiffness;
} M_HAPTICS_SET_STIFFNESS;

typedef struct {
  MSG_HEADER header;
  float glFloatColor;
} M_GRAPHICS_COLOR_FTOB;

typedef struct {
  MSG_HEADER header;
  char glByteColor;
} M_GRAPHICS_COLOR_BTOF;

typedef struct {
  MSG_HEADER header;
  double a_eye[3];
  double a_at[3];
  double a_up[3];
} M_GRAPHICS_LOOKAT;

typedef struct {
  MSG_HEADER header;
  double scale;
  double lengthScale;
  double thicknessScale;
} M_GRAPHICS_DRAW_FRAME;

typedef struct {
  MSG_HEADER header;
  double xMin;
  double xMax;
  double yMin;
  double yMax;
  double zMin;
  double zMax;
} M_GRAPHICS_DRAW_WIRE_BOX;

typedef struct {
  MSG_HEADER header;
  double radius;
  double numSlices;
  double numStacks;
} M_GRAPHICS_DRAW_SPHERE;

typedef struct {
  MSG_HEADER header; 
  double arrowStart[3];
  double arrowTip[3];
  double a_width;
} M_GRAPHICS_DRAW_ARROW;

typedef struct {
  MSG_HEADER header;
  double innerRadius;
  double outerRadius;
  int sides;
  int rings;
} M_GRAPHICS_DRAW_SOLID_TORUS;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  double position[3];
  double rotation[9]; //row major order of matrix
  float color[4]; //GLFloat
} M_GRAPHICS_CREATE_PLANE;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  double position[3];
  double rotation[9]; // row major order of 3d matrix 
  float colorTopLeft[4];
  float colorTopRight[4];
  float colorBottomLeft[4];
  float colorBottomRight[4];
} M_GRAPHICS_CREATE_PLANE2;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  unsigned int numSidesX;
  unsigned int numSidesY;
  double position[3];
  double rotation[9];
  double color[4];
} M_GRAPHICS_CREATE_MAP;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double radiusX;
  double radiusY;
  unsigned int numSlices;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_DISK;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  double radiusCorners;
  int numSegmentsPerCorner;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_PANEL;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  double cornerTopLeftRadius;
  double cornerTopRightRadius;
  double cornerBottomLeftRadius;
  double cornerBottomRightRadius;
  int numSegmentsPerCorner;
  double position[3];
  double rotation[9];
  float colorTopLeft[4];
  float colorTopRight[4];
  float colorBottomLeft[4];
  float colorBottomRight[4];
} M_GRAPHICS_CREATE_PANEL2;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double lengthX;
  double lengthY;
  double lengthZ;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_BOX;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double size;
  int resolution;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_TEAPOT;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128]; 
  double height;
  double radius;
  unsigned int numSides;
  unsigned int numHeightSegments;
  unsigned int numRings;
  int includeTop;
  int includeBottom;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_CYLINDER;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double height;
  double innerRadius;
  double outerRadius;
  unsigned int numSides;
  unsigned int numHeightSegments;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_PIPE;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double radius;
  unsigned int numSlices;
  unsigned int numStacks;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_SPHERE;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double radiusX;
  double radiusY;
  double radiusZ;
  unsigned int numSlices;
  unsigned int numStacks;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_ELLIPSOID;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double innerRadius;
  double outerRadius;
  unsigned int numSides;
  unsigned int numRings;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_RING;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double innerRadius0;
  double innerRadius1;
  double outerRadius;
  double coverageAngleDeg;
  int includeExtremityFaces;
  unsigned int numSides;
  unsigned int numRings;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_RING_SECTION;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double height;
  double radiusBototm;
  double radiusTop;
  unsigned int numSides;
  unsigned int numHeightSegments;
  unsigned int numRings;
  int includeBottom;
  int includeTop;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_CONE;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double height;
  double baseSize;
  int includeBottom;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_SQUARE_PYRAMID;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double a_length;
  double radiusShaft;
  double lengthTip;
  double radiusTip;
  int includeTipsAtBothExtremities;
  unsigned int numSides;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_ARROW;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double innerRadius0;
  double innerRadius1;
  double outerRadius;
  double lengthTip;
  double radiusTip;
  double coverageAngleDeg;
  int includeTipsAtBothExtremities;
  unsigned int numSides;
  unsigned int numRings;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_CIRCULAR_ARROW;

typedef struct {
  MSG_HEADER header;
  char meshObjectName[128];
  double controlPoints[3];
  int numDivisions;
  double position[3];
  double rotation[9];
  float color[4];
} M_GRAPHICS_CREATE_BEZIER_PATCH;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
  double radius;
  double localPosition[3];
  float color[4];
  int reserved[6];
} M_GRAPHICS_SHAPE_SPHERE;

typedef struct {
  MSG_HEADER header;
  char objectName[128];
  double innerRadius;
  double outerRadius;
} M_GRAPHICS_SHAPE_TORUS;
