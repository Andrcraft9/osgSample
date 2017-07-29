//
// Example of GLSL Cook-Torrance shader.
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	"libExt.h"

#ifdef	MACOSX
	#include	<GLUT/glut.h>
#else
	#include	<glut.h>
#endif

#include	<stdio.h>
#include	<stdlib.h>

#include	"libTexture.h"
#include	"Vector3D.h"
#include	"Vector2D.h"
#include	"Vector4D.h"
#include	"Data.h"
#include	"GlslProgram.h"

Vector3D	eye   ( 7, 5, 7 );			// camera position
Vector3D	light ( 5, 0, 4 );			// light position
float	 	angle = 0;
Vector3D	rot ( 0, 0, 0 );
int			mouseOldX = 0;
int			mouseOldY = 0;
unsigned	lookupMap;					// Beckman lookup texture
float		r0        = 0.5;
float		roughness = 0.15;

GlslProgram	program;

/////////////////////////////////////////////////////////////////////////////////

void	createLookupMap ()		// create Beckman texture (dot(n,h), m)
{
	int		width  = 128;
	int		height = 128;
	float * buf    = new float [4*width * height];
	float * ptr    = buf;
	
	for ( float nh = 0.0; nh < 1.0; nh += 1.0 / width )
		for ( float m = 0.0; m < 1.0; m += 1.0 / height )
		{
			double	m2  = m * m + 1e-7;
			double	nh2 = nh * nh + 1e-7;
			double  ex  = - (1.0 - nh2)/(nh2 * m2);
			double	d   = exp ( ex ) / (m2*nh2*nh2);
			
			*ptr ++ = (float) d;
			*ptr ++ = 1.0;
			*ptr ++ = 1.0;
			*ptr ++ = 1.0;
		}
		
	glEnable        ( GL_TEXTURE_2D );
	glGenTextures   ( 1, &lookupMap );
	glBindTexture   ( GL_TEXTURE_2D, lookupMap );
	glTexImage2D    ( GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, buf );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glBindTexture   ( GL_TEXTURE_2D, 0 );
	
	delete buf;
}

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );

	glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

												// draw the light
	program.unbind ();

	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();

	glTranslatef       ( light.x, light.y, light.z );
	glColor4f          ( 1, 1, 1, 1 );
	glutSolidSphere    ( 0.1f, 15, 15 );
	glPopMatrix        ();

	glMatrixMode   ( GL_MODELVIEW );
	glPushMatrix   ();

    glRotatef    ( rot.x, 1, 0, 0 );
    glRotatef    ( rot.y, 0, 1, 0 );
    glRotatef    ( rot.z, 0, 0, 1 );

	glBindTexture   ( GL_TEXTURE_2D, lookupMap );
	
	program.bind   ();

	glutSolidTeapot ( 2.5 );
    
	program.unbind ();

	glBindTexture   ( GL_TEXTURE_2D, 0 );
    glPopMatrix ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
   glViewport     ( 0, 0, (GLsizei)w, (GLsizei)h );
   glMatrixMode   ( GL_PROJECTION );
   											// factor all camera ops into projection matrix
   glLoadIdentity ();
   gluPerspective ( 60.0, (GLfloat)w/(GLfloat)h, 1.0, 60.0 );
   gluLookAt      ( eye.x, eye.y, eye.z,	// eye
					0, 0, 0,				// center
					0.0, 0.0, 1.0 );		// up

   glMatrixMode   ( GL_MODELVIEW );
   glLoadIdentity ();
}

void motion ( int x, int y )
{
    rot.y -= ((mouseOldY - y) * 180.0f) / 200.0f;
    rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
    rot.x  = 0;

    if ( rot.z > 360 )
		rot.z -= 360;

	if ( rot.z < -360 )
		rot.z += 360;

    if ( rot.y > 360 )
		rot.y -= 360;

	if ( rot.y < -360 )
        rot.y += 360;

    mouseOldX = x;
    mouseOldY = y;

	glutPostRedisplay ();
}

void mouse ( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN )
    {
        mouseOldX = x;
        mouseOldY = y;
	}
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '+' )
		roughness += 0.02;
	else
	if ( key == '-' )
		roughness -= 0.02;
	else
	if ( key == '*' )
		r0 += 0.02;
	else
	if ( key == '/' )
		r0 -= 0.02;
}

void	animate ()
{
	angle  = 0.004f * glutGet ( GLUT_ELAPSED_TIME );

	light.x = 3*cos ( angle );
	light.y = 4*sin ( 1.4 * angle );
	light.z = 4 + 0.5 * sin ( angle / 3 );

	program.bind ();
	program.setUniformVector ( "eyePos",    Vector4D ( eye,   1 ) );
	program.setUniformVector ( "lightPos",  Vector4D ( light, 1 ) );
	program.setUniformFloat  ( "r0",        r0 );
	program.setUniformFloat  ( "roughness", roughness );
	program.unbind ();

	glutPostRedisplay ();
}

int main ( int argc, char * argv [] )
{
								// initialize glut
	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 500, 500 );


								// create window
	glutCreateWindow ( "Cook-Torrance Lighting Model" );

								// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );
	glutMouseFunc    ( mouse   );
	glutMotionFunc   ( motion  );
	glutIdleFunc     ( animate );

	init           ();
	initExtensions ();

	assertExtensionsSupported ( "GL_ARB_shading_language_100 GL_ARB_shader_objects" );

	if ( !program.loadShaders ( "cook-torrance.vsh", "cook-torrance.fsh" ) )
	{
		printf ( "Error loading shaders:\n%s\n", program.getLog ().c_str () );

		return 3;
	}

	program.bind        ();
	program.setTexture  ( "lookupMap",  0 );
	program.unbind      ();
	
	createLookupMap ();
	
	printf ( "Use:\n\t+ and - keys to change roughness,\n\t* and / to change R0\n" );
	
    glutMainLoop ();

	return 0;
}
